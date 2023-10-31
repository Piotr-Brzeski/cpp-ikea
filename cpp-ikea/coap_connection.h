//
//  coap_connection.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <string>

namespace coap {
	class client;
	class session;
}

namespace ikea {

class coap_connection {
public:
	coap_connection(std::string const& ip, std::string const& identity, std::string const& key);
	~coap_connection();
	
	coap_connection(coap_connection const&) = delete;
	coap_connection(coap_connection&&) = delete;
	
	std::string get(std::string const& uri);
	void put(std::string const& uri, std::string const& data);
	
private:
	coap::client*  m_coap_client = nullptr;
	coap::session* m_coap_session = nullptr;
};

}
