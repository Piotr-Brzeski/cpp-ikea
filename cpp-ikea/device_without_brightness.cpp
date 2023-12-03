//
//  device_without_brightness.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "device_without_brightness.h"
#include "device_with_brightness.h"
#include <cpp-log/log.h>

using namespace ikea;

namespace {

std::string const& state_name(bool state) {
	static std::string const on = "on";
	static std::string const off = "off";
	return state ? on : off;
}

}

device_without_brightness::device_without_brightness(device_without_brightness&& dev)
	: m_enabled_to_send(dev.m_enabled_to_send)
	, m_enabled(dev.m_enabled.load())
{
}

bool device_without_brightness::enabled() {
	update();
	return m_enabled;
}

std::uint8_t device_without_brightness::brightness() {
	return enabled() ? device_with_brightness::max_brightness : device_with_brightness::zero_brightness;
}

void device_without_brightness::set(bool enabled) {
	m_enabled_to_send = enabled;
	send();
	logger::log("[" + name() + "] set enabled state: " + state_name(m_enabled) + " -> " + state_name(enabled));
	m_enabled = enabled;
}

void device_without_brightness::toggle() {
	set(!enabled());
}

void device_without_brightness::increase() {
	if(!enabled()) {
		set(true);
	}
}

void device_without_brightness::decrease() {
	if(enabled()) {
		set(false);
	}
}

void device_without_brightness::internal_update(bool enabled) {
	bool previous_state = m_enabled;
	m_enabled = enabled;
	logger::log("[" + name() + "] update enabled state: " + state_name(previous_state) + " -> " + state_name(m_enabled));
}
