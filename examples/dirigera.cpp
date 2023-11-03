//
//  dirigera.cpp
//  tradfri
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "system_tester.h"
#include <cpp-ikea/system.h>
#include <cpp-ikea/dirigera.h>
#include <cpp-log/log.h>
#include <iostream>
#include "configuration.h"

int main() {
	try {
		auto log = logger::start(logger::cout());
		auto system = ikea::system<ikea::dirigera>(dirigera_configuration::ip, dirigera_configuration::access_token);
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
