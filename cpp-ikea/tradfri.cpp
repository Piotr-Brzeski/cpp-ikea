//
//  tradfri.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "tradfri.h"
#include "json.h"

using namespace ikea;

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

tradfri::tradfri(configuration const& configuration)
	: m_coap(configuration.ip, configuration.identity, configuration.key)
{
}

tradfri::tradfri(std::string const& ip, std::string const& identity, std::string const& key)
	: m_coap(ip, identity, key)
{
}

void tradfri::enumerate_devices() {
	static auto const devices_uri = std::string("15001");
	auto list = m_coap.get(devices_uri);
	auto ids = parse_ids(list);
	for(auto& id : ids) {
		load_device(id);
	}
//	static auto const groups_uri = std::string("15004");
//	list = m_coap.get(groups_uri);
//	ids = parse_ids(list);
//	for(auto& id : ids) {
//		load_group(id);
//	}
}

void tradfri::load_device(std::string const& id) {
	static auto const type_key = std::string("5750");
	auto device_description = tradfri_device::load(m_coap, id);
	auto device_json = json(std::move(device_description));
	auto type = device_json[type_key].get_int();
	switch(type) {
		case tradfri_bulb::system_id:
			m_bulbs.push_back(tradfri_bulb(id, m_coap, device_json));
			break;
		case tradfri_outlet::system_id:
			m_outlets.push_back(tradfri_outlet(id, m_coap, device_json));
			break;
		default:
//			std::cout << id << ", type=" << type << ", " << device_json["9001"].get_string() << std::endl;
			break;
	}
}

//void tradfri::load_group(std::string const& id) {
//	auto group_description = tradfri_group::load(m_coap, id);
//	auto group_json = json(std::move(group_description));
//	m_groups.push_back(tradfri_group(id, m_coap, group_json));
//}
