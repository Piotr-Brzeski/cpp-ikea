//
//  dirigera.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "http_connection.h"
#include "dirigera_outlet.h"
#include "tradfri_bulb.h"
#include "tradfri_group.h"
#include <vector>
#include <functional>

namespace ikea {

class dirigera {
public:
	struct configuration {
		std::string ip;
		std::string access_token;
	};
	
	dirigera(configuration const& configuration);
	dirigera(std::string const& ip, std::string const& access_token);
	
	void enumerate_devices();
	
	std::vector<tradfri_bulb>& bulbs() {
		static auto bulbs = std::vector<tradfri_bulb>();
		return bulbs;
	}
	std::vector<dirigera_outlet>& outlets() {
		return m_outlets;
	}
	std::vector<tradfri_group>& groups() {
		static auto groups = std::vector<tradfri_group>();
		return groups;
	}

private:
	http_connection              m_connection;
	std::string const            m_uri;
	std::vector<dirigera_outlet> m_outlets;
};

}
