//
//  http_connection.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 brzeski.net. All rights reserved.
//

#include "http_connection.h"
#include "exception.h"
#include <cpp-log/log.h>
#include <curl-ws/curl.h>

using namespace ikea;

namespace {

std::size_t write_cb(void* data, std::size_t size, std::size_t nmemb, void* context) {
	auto& result = *static_cast<std::string*>(context);
	auto data_size = size*nmemb;
	result.append(static_cast<char const*>(data), data_size);
	return data_size;
}

}

http_connection::http_connection(std::string const& access_token) {
	error_buffer()[0] = '\0';
	m_get = ::curl_easy_init();
	m_patch = ::curl_easy_init();
	if(m_get == nullptr || m_patch == nullptr) {
		::curl_easy_cleanup(m_patch);
		::curl_easy_cleanup(m_get);
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
