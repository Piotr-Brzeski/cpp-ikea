//
//  tradfri.cpp
//  tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include <cpp-ikea/tradfri.h>
#include <cpp-log/log.h>
#include <iostream>
#include <fstream>

#include "configuration.h"

std::vector<std::function<void()>> toggle_operations;
std::vector<std::function<void()>> increase_operations;
std::vector<std::function<void()>> decrease_operations;

auto& load() {
	std::string value;
	std::cin >> value;
	if(value.back() == '+') {
		value.pop_back();
		return increase_operations.at(std::stoul(value));
	}
	if(value.back() == '-') {
		value.pop_back();
		return decrease_operations.at(std::stoul(value));
	}
	return toggle_operations.at(std::stoul(value));
}

int main(int argc, const char * argv[]) {
	try {
		auto log = logger::start(logger::cout());
		auto system = ikea::tradfri(configuration::ip, configuration::identity, configuration::key);
		system.enumerate_devices();
		auto& bulbs = system.bulbs();
		auto& plugs = system.plugs();
		auto& gropus = system.groups();
		bool run = true;
		while(run) {
			for(std::size_t i = 0; i < bulbs.size(); ++i) {
				auto& bulb = bulbs[i];
				std::cout << (i < 10 ? " ": "") << i << " : Bulb  [" << static_cast<int>(bulb.brightness()) << "] " << bulb.name() << std::endl;
				toggle_operations.emplace_back(system.toggle_operation(bulb.name()));
				increase_operations.emplace_back(system.increase_operation(bulb.name()));
				decrease_operations.emplace_back(system.decrease_operation(bulb.name()));
			}
			for(std::size_t i = 0; i < plugs.size(); ++i) {
				auto& plug = plugs[i];
				std::cout << (i+bulbs.size() < 10 ? " ": "") << (i+bulbs.size()) << " : Plug  [" << static_cast<int>(plug.enabled()) << "] " << plug.name() << std::endl;
				toggle_operations.emplace_back(system.toggle_operation(plug.name()));
				increase_operations.emplace_back(system.increase_operation(plug.name()));
				decrease_operations.emplace_back(system.decrease_operation(plug.name()));
			}
			for(std::size_t i = 0; i < gropus.size(); ++i) {
				auto& group = gropus[i];
				std::cout << (i+bulbs.size()+plugs.size() < 10 ? " ": "") << (i+bulbs.size()+plugs.size()) << " : Group [" << static_cast<int>(group.brightness()) << "] " << group.name() << std::endl;
				toggle_operations.emplace_back(system.toggle_operation(group.name()));
				increase_operations.emplace_back(system.increase_operation(group.name()));
				decrease_operations.emplace_back(system.decrease_operation(group.name()));
			}
			std::cout << "\nType [index] to toggle, [index]+ to increase, [index]- to decrease:" << std::endl;
			load()();
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
