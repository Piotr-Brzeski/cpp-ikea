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

std::vector<std::string> parse_ids(std::string const& list) {
	std::vector<std::string> ids;
	auto begin = list.find('[');
	while(begin != std::string::npos) {
		++begin;
		auto end = list.find(',', begin);
		if(end != std::string::npos) {
			if(ids.empty()) {
				auto expected_number_of_ids = list.size()/end;
				ids.reserve(expected_number_of_ids);
			}
			ids.emplace_back(list.data() + begin, end - begin);
		}
		else {
			auto end = list.find(']', begin);
			if(end != std::string::npos) {
				ids.emplace_back(list.data() + begin, end - begin);
			}
		}
		begin = end;
	}
	return ids;
}

}

system::system(std::string const& ip, std::string const& identity, std::string const& key)
	: m_coap(ip, identity, key)
{
}

void system::enumerate_devices() {
	static auto const uri = std::string("15001");
	auto devices_list = m_coap.get(uri);
	auto ids = parse_ids(devices_list);
	std::cout << "Size: " << ids.size() << std::endl;
	for(auto& id : ids) {
		load_device(id);
	}
}

void system::load_device(std::string const& id) {
	static auto const uri = std::string("15001/");
	auto device_description = m_coap.get(uri + id);
	std::cout << (uri + id) << " : " << device_description << std::endl;
}
