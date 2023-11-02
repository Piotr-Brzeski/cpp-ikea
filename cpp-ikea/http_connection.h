//
//  http_connection.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 brzeski.net. All rights reserved.
//

#pragma once

#include <string>

struct curl_slist;

namespace ikea {

class http_connection {
public:
	using status_code = unsigned int;
	
	http_connection();
	~http_connection();
	
	http_connection(http_connection const&) = delete;
	http_connection(http_connection&&) = delete;
	
	void add_header(std::string const& header);
	void clear_headers();
	
	void set_data(std::string const& data);
	void set_url(std::string const& url);
	
	status_code get();
	status_code get(std::string const& url);
	std::string const& get_response(std::string const& url);
	
	status_code post();
	status_code post(std::string const& url, std::string const& data);
	
	status_code patch();
	status_code patch(std::string const& url, std::string const& data);
	void send_patch(std::string const& url, std::string const& data);
	
	std::string const& response() const {
		return m_response;
	}
	
private:
	status_code send_request();
	void*         m_curl = nullptr;
	::curl_slist* m_headers = nullptr;
	std::string   m_response;
};

}

