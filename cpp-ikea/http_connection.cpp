//
//  http_connection.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 brzeski.net. All rights reserved.
//

#include "http_connection.h"
#include "exception.h"
#include <curl/curl.h>
#include <cpp-log/log.h>

using namespace ikea;

namespace {

char* error_buffer() {
	thread_local char buffer[CURL_ERROR_SIZE];
	return buffer;
}

std::size_t write_cb(void* data, std::size_t size, std::size_t nmemb, void* context) {
	auto& result = *static_cast<std::string*>(context);
	auto data_size = size*nmemb;
	result.append(static_cast<char const*>(data), data_size);
	return data_size;
}


void check(CURLcode result) {
	if(result == CURLE_OK) {
		return;
	}
	auto error_message = "cURL error " + std::to_string(result) + ": ";
	if(error_buffer()[0] == '\0') {
		error_message += ::curl_easy_strerror(result);
	}
	else {
		error_message += error_buffer();
		if(error_message.back() == '\n') {
			error_message.pop_back();
		}
	}
	throw exception(error_message);
}

void add_header(::curl_slist*& headers, std::string const& header) {
	auto new_headers = ::curl_slist_append(headers, header.c_str());
	if(new_headers == nullptr) {
		throw exception("cURL add header failed");
	}
	headers = new_headers;
}

void configure(CURL* curl, ::curl_slist* headers) {
	error_buffer()[0] = '\0';
	check(::curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer()));
	// Allow self-signed certificates
	check(::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0));
	// Allow any hostname
	check(::curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0));
	check(::curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers));
}

long send_request(CURL* curl) {
	error_buffer()[0] = '\0';
	check(::curl_easy_perform(curl));
	long status = 0;
	check(::curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status));
	return status;
}

}

http_connection::http_connection(std::string const& access_token) {
	error_buffer()[0] = '\0';
	check(::curl_global_init(CURL_GLOBAL_ALL));
	m_get = ::curl_easy_init();
	m_patch = ::curl_easy_init();
	if(m_get == nullptr || m_patch == nullptr) {
		::curl_easy_cleanup(m_patch);
		::curl_easy_cleanup(m_get);
		::curl_global_cleanup();
		throw exception("cURL fatal error");
	}
	add_header(m_get_headers, "Authorization: Bearer " + access_token);
	add_header(m_patch_headers, "Authorization: Bearer " + access_token);
	add_header(m_patch_headers, "Content-Type: application/json");
	configure(m_get, m_get_headers);
	configure(m_patch, m_patch_headers);
	check(::curl_easy_setopt(m_get, CURLOPT_WRITEFUNCTION, write_cb));
	check(::curl_easy_setopt(m_get, CURLOPT_WRITEDATA, &m_response));
	check(::curl_easy_setopt(m_patch, CURLOPT_CUSTOMREQUEST, "PATCH"));
}

http_connection::~http_connection() {
	::curl_easy_cleanup(m_patch);
	::curl_easy_cleanup(m_get);
	::curl_slist_free_all(m_patch_headers);
	::curl_slist_free_all(m_get_headers);
	::curl_global_cleanup();
}

std::string const& http_connection::get(std::string const& url) {
	if(url != m_get_url) {
		error_buffer()[0] = '\0';
		check(::curl_easy_setopt(m_get, CURLOPT_URL, url.c_str()));
		m_get_url = url;
	}
	m_response.clear();
	auto status = send_request(m_get);
	auto status_str = std::to_string(status);
	logger::log("HTTPS GET " + m_get_url + " : [" + status_str + "] " + m_response);
	if(status == 200) {
		return m_response;
	}
	throw exception("HTTP GET failed with status " + status_str);
}

void http_connection::set_patch_url(std::string const& url) {
	if(url != m_patch_url) {
		error_buffer()[0] = '\0';
		check(::curl_easy_setopt(m_patch, CURLOPT_URL, url.c_str()));
		m_patch_url = url;
	}
}

void http_connection::patch(std::string const& data) {
	error_buffer()[0] = '\0';
	check(::curl_easy_setopt(m_patch, CURLOPT_POSTFIELDS, data.c_str()));
	check(::curl_easy_setopt(m_patch, CURLOPT_POSTFIELDSIZE, data.size()));
	auto status = send_request(m_patch);
	auto status_str = std::to_string(status);
	logger::log("HTTPS PATCH " + m_patch_url + " " + data + " : [" + status_str + "]");
	if(status == 202) {
		return;
	}
	throw exception("HTTP PATCH failed with status " + status_str);
}
