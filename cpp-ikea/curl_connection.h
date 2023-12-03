//
//  curl_connection.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-30.
//  Copyright © 2023 brzeski.net. All rights reserved.
//

#pragma once

#include <string>

extern "C" {
struct curl_slist;
}

namespace ikea {

class curl_connection {
public:
	curl_connection();
	~curl_connection();
	
protected:
	static char* error_buffer();
	static void check(int result);
	static void add_header(::curl_slist*& headers, std::string const& header);
	static void configure(void* curl, ::curl_slist* headers);
	static long send_request(void* curl);
};

}
