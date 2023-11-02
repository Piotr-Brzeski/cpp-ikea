//
//  dirigera.cpp
//  tradfri
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "system_tester.h"
#include <cpp-ikea/dirigera.h>
#include <cpp-log/log.h>
#include <iostream>

#include "configuration.h"

void list_devices() {
	auto client = ikea::http_connection();
	client.add_header(std::string("Authorization: Bearer ") + dirigera_configuration::access_token);
	client.get(std::string("https://") + dirigera_configuration::ip + ":8443/v1/devices/");
	auto response = client.response();
	std::cout << response << std::endl;
}


int main() {
	try {
		//list_devices();
//		auto log = logger::start(logger::cout());
		auto system = ikea::dirigera(dirigera_configuration::ip, dirigera_configuration::access_token);
		auto test = tester(system);
		test.start();
		return 0;
	}
	catch(std::exception &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	catch(...) {
		std::cerr << "Exception." << std::endl;
	}
	return 1;
}
