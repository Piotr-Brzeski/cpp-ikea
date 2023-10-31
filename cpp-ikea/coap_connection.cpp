//
//  coap_connection.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "coap_connection.h"
#include <cpp-coap/client.h>
#include <memory>

using namespace ikea;

coap_connection::coap_connection(std::string const& ip, std::string const& identity, std::string const& key) {
	auto coap_client = std::make_unique<coap::client>();
	auto coap_session = std::make_unique<coap::session>(*coap_client, ip.c_str(), 5684, identity, key);
	m_coap_session = coap_session.release();
	m_coap_client = coap_client.release();
}

coap_connection::~coap_connection() {
	delete m_coap_session;
	delete m_coap_client;
}

std::string coap_connection::get(std::string const& uri) {
	return m_coap_session->get(uri);
}

void coap_connection::put(std::string const& uri, std::string const& data) {
	m_coap_session->put(uri, data);
}
