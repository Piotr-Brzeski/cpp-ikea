//
//  bulb.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "bulb.h"

using namespace tradfri;

bulb::bulb(std::string const& id, std::string const& name)
	: device(id, name)
{
}
