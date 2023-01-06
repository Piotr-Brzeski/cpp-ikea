//
//  device.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <string>

namespace tradfri {

class device {
public:
	device(std::string const& id, std::string const& name);
	
protected:
	std::string m_id;
	std::string m_name;
};

}
