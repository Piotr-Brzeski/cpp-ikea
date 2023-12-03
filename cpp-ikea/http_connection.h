//
//  http_connection.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 brzeski.net. All rights reserved.
//

#pragma once

#include "curl_connection.h"

namespace ikea {

class http_connection : public curl_connection {
public:
	http_connection(std::string const& access_token);
	~http_connection();
	
	http_connection(http_connection const&) = delete;
	http_connection(http_connection&&) = delete;
	
	std::string const& get(std::string const& url);
	void set_patch_url(std::string const& url);
	void patch(std::string const& data);
	
private:
	void*         m_get = nullptr;
	void*         m_patch = nullptr;
	::curl_slist* m_get_headers = nullptr;
	::curl_slist* m_patch_headers = nullptr;
	std::string   m_response;
	std::string   m_get_url;
	std::string   m_patch_url;
};

}
