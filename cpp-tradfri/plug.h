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
	static plug load(std::string const& id, coap_connection& coap, json const& json);
	
	bool enabled();
	
	void set(bool enabled);
	void toggle();
	void increase();
	void decrease();
	
private:
	plug(std::string const& id, coap_connection& coap, json const& json);
	void update(json const& json);
	void update();
	
	bool m_enabled;
};

}
