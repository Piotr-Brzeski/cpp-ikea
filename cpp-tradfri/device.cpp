//
//  device.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "device.h"

using namespace tradfri;

device::device(std::string const& id, std::string const& name)
	: m_id(id)
	, m_name(name)
{
}
