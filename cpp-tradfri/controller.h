//
//  controller.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-20.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "protocol.h"
#include "system.h"
#include <map>

namespace tradfri {

class controller {
public:
	controller(std::string_view configuration_path);
	
	using operation = std::function<void()>;
private:
	system                                         m_tradfri_system;
	std::map<protocol::device_state_id, operation> m_commands;
};

}
