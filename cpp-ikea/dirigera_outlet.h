//
//  dirigera_outlet.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "dirigera_device.h"

namespace ikea {

class dirigera_outlet: public dirigera_device {
public:
	static std::string const device_type;
	
	dirigera_outlet(std::string const& devices_uri, http_connection& connection, json_value const& json);
	
	bool enabled();
	std::uint8_t brightness();
	
	void set(bool enabled);
	void toggle();
	void increase();
	void decrease();
	
private:
	void update(json_value const& json) override;
	
	bool m_enabled;
};

}

