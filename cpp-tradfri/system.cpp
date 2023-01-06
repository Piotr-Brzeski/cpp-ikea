//
//  system.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "system.h"
#include <vector>

#include <iostream>

using namespace tradfri;

namespace {

std::vector<std::string_view> parse_ids(std::string const& array) {
	constexpr auto delimeters = std::string_view(",]", 3);
	std::vector<std::string_view> ids;
	std::size_t begin = array.find('[');
	while(begin != std::string::npos) {
		++begin;
		auto end = array.find_first_of(delimeters, begin);
		if(end != std::string::npos) {
			ids.emplace_back(array.data() + begin, end - begin);
		}
		begin = end;
	}
	return ids;
}

}


system::system(std::string const& ip, int port) {
	
}

void system::enumerate_devices() {
	std::string all_devices = "[131093,131080,131081,131091,131083,131077,131085,131089,131090,131079,131095,131088,131097,131078,131082,131084,131087,131094,131098]";
	auto ids = parse_ids(all_devices);
	std::cout << "Size: " << ids.size() << std::endl;
	for(auto& id : ids) {
		std::cout << id << std::endl;
	}
}
