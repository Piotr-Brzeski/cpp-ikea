//
//  tradfri.cpp
//  tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "system_tester.h"
#include <cpp-ikea/system.h>
#include <cpp-ikea/tradfri.h>
#include <cpp-log/log.h>
#include <iostream>
#include "configuration.h"

int main(int argc, const char * argv[]) {
	try {
//		auto log = logger::start(logger::cout());
		auto system = ikea::system<ikea::tradfri>({tradfri_configuration::ip,
		                                           tradfri_configuration::identity,
		                                           tradfri_configuration::key});
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
