//
//  tradfri.cpp
//  tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include <cpp-tradfri/system.h>
#include <iostream>
#include <fstream>

#include "configuration.h"

std::uint8_t load() {
	std::string value;
	std::cin >> value;
	return std::stoul(value);
}

int main(int argc, const char * argv[]) {
	try {
		auto system = tradfri::system(configuration::ip, configuration::identity, configuration::key);
		system.enumerate_devices();
		std::vector<std::function<void()>> operations;
		auto& bulbs = system.bulbs();
		auto& plugs = system.plugs();
		auto& gropus = system.groups();
		bool run = true;
		while(run) {
			for(std::size_t i = 0; i < bulbs.size(); ++i) {
				auto& bulb = bulbs[i];
				std::cout << (i < 10 ? " ": "") << i << " : Bulb  [" << static_cast<int>(bulb.brightness()) << "] " << bulb.name() << std::endl;
				operations.emplace_back(system.toggle_operation(bulb.name()));
			}
			for(std::size_t i = 0; i < plugs.size(); ++i) {
				auto& plug = plugs[i];
				std::cout << (i+bulbs.size() < 10 ? " ": "") << (i+bulbs.size()) << " : Plug  [" << static_cast<int>(plug.enabled()) << "] " << plug.name() << std::endl;
				operations.emplace_back(system.toggle_operation(plug.name()));
			}
			for(std::size_t i = 0; i < gropus.size(); ++i) {
				auto& group = gropus[i];
				std::cout << (i+bulbs.size()+plugs.size() < 10 ? " ": "") << (i+bulbs.size()+plugs.size()) << " : Group [" << static_cast<int>(group.brightness()) << "] " << group.name() << std::endl;
				operations.emplace_back(system.toggle_operation(group.name()));
			}
			std::cout << "\nType [index] to toggle:" << std::endl;
			auto index = load();
			operations.at(index)();
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
