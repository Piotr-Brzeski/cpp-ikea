//
//  system_tester.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <vector>
#include <functional>
#include <iostream>

template<class System>
class tester {
public:
	tester(System& system)
		: system(system)
	{
		system.enumerate_devices();
		for(auto& bulb : system.bulbs()) {
			toggle_operations.emplace_back(system.toggle_operation(bulb->name()));
			increase_operations.emplace_back(system.increase_operation(bulb->name()));
			decrease_operations.emplace_back(system.decrease_operation(bulb->name()));
		}
		for(auto& outlet : system.outlets()) {
			toggle_operations.emplace_back(system.toggle_operation(outlet->name()));
			increase_operations.emplace_back(system.increase_operation(outlet->name()));
			decrease_operations.emplace_back(system.decrease_operation(outlet->name()));
		}
//		for(auto& group : system.groups()) {
//			toggle_operations.emplace_back(system.toggle_operation(group.name()));
//			increase_operations.emplace_back(system.increase_operation(group.name()));
//			decrease_operations.emplace_back(system.decrease_operation(group.name()));
//		}
	}
	
	void start() {
		auto bulbs = system.bulbs();
		auto outlets = system.outlets();
//		auto& gropus = system.groups();
		bool run = true;
		while(run) {
			for(std::size_t i = 0; i < bulbs.size(); ++i) {
				auto& bulb = bulbs[i];
				std::cout << (i < 10 ? " ": "") << i << " : Bulb  [" << static_cast<int>(bulb->brightness()) << "] " << bulb->name() << std::endl;
			}
			for(std::size_t i = 0; i < outlets.size(); ++i) {
				auto& outlet = outlets[i];
				std::cout << (i+bulbs.size() < 10 ? " ": "") << (i+bulbs.size()) << " : Plug  [" << static_cast<int>(outlet->enabled()) << "] " << outlet->name() << std::endl;
			}
//			for(std::size_t i = 0; i < gropus.size(); ++i) {
//				auto& group = gropus[i];
//				std::cout << (i+bulbs.size()+outlets.size() < 10 ? " ": "") << (i+bulbs.size()+outlets.size()) << " : Group [" << static_cast<int>(group.brightness()) << "] " << group.name() << std::endl;
//			}
			std::cout << "\nType [index] to toggle, [index]+ to increase, [index]- to decrease:" << std::endl;
			load()();
		}
	}
	
private:
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
	
	System&                            system;
	std::vector<std::function<void()>> toggle_operations;
	std::vector<std::function<void()>> increase_operations;
	std::vector<std::function<void()>> decrease_operations;
};
