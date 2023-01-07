//
//  plug.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"

namespace tradfri {

class plug: public device {
public:
	static constexpr int system_id = 3;
	static plug load(std::string&& id, coap_connection& coap, json const& json);
	
	void update();
	bool enabled() const {
		return m_enabled;
	}
	void set(bool enabled);
	void toggle();
	
private:
	plug(std::string&& id, coap_connection& coap, json const& json);
	void update(json const& json);
	
	bool m_enabled;
};

}
