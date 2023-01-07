//
//  tradfri.cpp
//  tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include <cpp-tradfri/system.h>
#include <iostream>

std::uint8_t load() {
	std::string value;
	std::cin >> value;
	return std::stoul(value);
}


int main(int argc, const char * argv[]) {
	try {
		auto system = tradfri::system("172.0.0.1", "identity", "key");
		system.enumerate_devices();
		auto& bulbs = system.bulbs();
		bool run = true;
		while(run) {
			for(std::size_t i = 0; i < bulbs.size(); ++i) {
				auto& bulb = bulbs[i];
				std::cout << (i < 10 ? " ": "") << i << " : [" << static_cast<int>(bulb.brightness()) << "] " << bulb.name() << std::endl;
			}
			std::cout << "\nType [index] [brightness]:" << std::endl;
			auto index = load();
			auto brightness = load();
			bulbs.at(index).set(brightness);
		}
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
