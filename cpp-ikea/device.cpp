//
//  dirigera_device.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "device.h"

using namespace ikea;

device::device(std::string uri, std::string name)
	: m_uri(std::move(uri))
	, m_name(std::move(name))
	, m_last_update_time(std::chrono::steady_clock::now())
{
}

bool device::needs_update() const {
	constexpr auto threshold = std::chrono::milliseconds(500);
	auto now = std::chrono::steady_clock::now();
	return (now - m_last_update_time) > threshold;
}

void device::updated() {
	m_last_update_time = std::chrono::steady_clock::now();
}
