//
//  bulb.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"

namespace tradfri {

class bulb: public device {
public:
	bulb(std::string const& id, std::string const& name);
};

}
