//
//  system.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "system.h"
#include "json.h"
#include "bulb.h"
#include <vector>
//#include <algorithm>

//#include <iostream>

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
	for(auto& id : ids) {
		load_device(id);
	}
//	std::sort(m_bulbs.begin(), m_bulbs.end(), [](auto& b1, auto& b2){ return b1.name() < b2.name(); });
}

void system::load_device(std::string id) {
	static auto const type_key = std::string("5750");
	auto device_description = device::load(m_coap, id);
	auto device_json = json(std::move(device_description));
	auto type = device_json[type_key].get_int();
	switch(type) {
		case bulb::system_id:
			m_bulbs.push_back(bulb::load(std::move(id), m_coap, device_json));
			break;
		case plug::system_id:
			m_plugs.push_back(plug::load(std::move(id), m_coap, device_json));
			break;
		default:
//			std::cout << id << ", type=" << type << ", " << device_json["9001"].get_string() << std::endl;
			break;
	}
}
