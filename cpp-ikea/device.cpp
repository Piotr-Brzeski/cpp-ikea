//
//  dirigera_device.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "device.h"

using namespace ikea;

device::device(std::string const& name)
	: m_name(name)
	, m_last_update_time(std::chrono::steady_clock::now())
{
}

void device::send() {
	send_state();
	m_last_update_time = std::chrono::steady_clock::now();
}

void device::update() {
	if(needs_update()) {
		get_state();
		m_last_update_time = std::chrono::steady_clock::now();
	}
}

bool device::needs_update() const {
	constexpr auto threshold = std::chrono::milliseconds(500);
	auto now = std::chrono::steady_clock::now();
	return (now - m_last_update_time) > threshold;
}
