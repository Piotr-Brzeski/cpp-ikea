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

}

http_connection::http_connection() {
	error_buffer()[0] = '\0';
	check(::curl_global_init(CURL_GLOBAL_ALL));
	m_curl = ::curl_easy_init();
	if(m_curl == nullptr) {
		::curl_global_cleanup();
		throw exception("cURL fatal error");
	}
	check(::curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, error_buffer()));
	check(::curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_cb));
	check(::curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_response));
	// Allow self-signed certificates
	check(::curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0));
	// Allow any hostname
	check(::curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0));
}


http_connection::~http_connection() {
	clear_headers();
	::curl_easy_cleanup(m_curl);
	::curl_global_cleanup();
}


void http_connection::add_header(std::string const& header) {
	auto headers = ::curl_slist_append(m_headers, header.c_str());
	if(headers == nullptr) {
		throw exception("cURL add header failed");
	}
	m_headers = headers;
	error_buffer()[0] = '\0';
	check(::curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers));
}

void http_connection::clear_headers() {
	::curl_slist_free_all(m_headers);
	m_headers = nullptr;
}

void http_connection::set_data(std::string const& data) {
	error_buffer()[0] = '\0';
	check(::curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str()));
	check(::curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, data.size()));
}

void http_connection::set_url(std::string const& url) {
	error_buffer()[0] = '\0';
	check(::curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str()));
}

http_connection::status_code http_connection::get() {
	return send_request();
}

http_connection::status_code http_connection::get(std::string const& url) {
	set_url(url);
	return send_request();
}

std::string const& http_connection::get_response(std::string const& url) {
	auto status = get(url);
	if(status == 200) {
		return m_response;
	}
	throw exception("HTTP GET failed with status " + std::to_string(status));
}

http_connection::status_code http_connection::post() {
	return send_request();
}

http_connection::status_code http_connection::post(std::string const& url, std::string const& data) {
	set_url(url);
	set_data(data);
	return post();
}

http_connection::status_code http_connection::patch() {
	error_buffer()[0] = '\0';
	check(::curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "PATCH"));
	return send_request();
}

http_connection::status_code http_connection::patch(std::string const& url, std::string const& data) {
	set_url(url);
	set_data(data);
	return patch();
}

void http_connection::send_patch(std::string const& url, std::string const& data) {
	auto status = patch(url, data);
	if(status == 202) {
		return;
	}
	throw exception("HTTP PATCH failed with status " + std::to_string(status));
}

http_connection::status_code http_connection::send_request() {
	error_buffer()[0] = '\0';
	m_response.clear();
	check(::curl_easy_perform(m_curl));
	long response_code = 0;
	check(::curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &response_code));
	return static_cast<status_code>(response_code);
}

