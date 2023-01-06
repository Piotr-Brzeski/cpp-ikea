//
//  system.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <string>

namespace tradfri {

class system {
public:
	system(std::string const& ip, int port);
	
	void enumerate_devices();
};

}
