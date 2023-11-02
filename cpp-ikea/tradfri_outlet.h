//
//  tradfri_outlet.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "tradfri_device.h"

namespace ikea {

class tradfri_outlet: public tradfri_device {
public:
	static constexpr int system_id = 3;
	static tradfri_outlet load(std::string const& id, coap_connection& coap, json const& json);
	
	bool enabled();
	std::uint8_t brightness();
	
	void set(bool enabled);
	void toggle();
	void increase();
	void decrease();
	
private:
	tradfri_outlet(std::string const& id, coap_connection& coap, json const& json);
	void update(json const& json) override;
	
	bool m_enabled;
};

}
