//
//  device_with_brightness.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "device_with_brightness.h"
#include <cpp-log/log.h>

using namespace ikea;

std::uint8_t device_with_brightness::brightness() {
	update();
	return m_brightness;
}

void device_with_brightness::set(bool enabled) {
	set(enabled ? max_brightness : zero_brightness);
}

void device_with_brightness::set(std::uint8_t brightness) {
	m_brightness_to_send = brightness;
	send();
	logger::log("[" + name() + "] set brightness: " + std::to_string(m_brightness) + " -> " + std::to_string(brightness));
	m_brightness = brightness;
}

void device_with_brightness::toggle() {
	set(m_brightness == 0 ? max_brightness : zero_brightness);
}

void device_with_brightness::increase() {
	if(m_brightness < max_brightness) {
		set(static_cast<std::uint8_t>(m_brightness + 1));
	}
}

void device_with_brightness::decrease() {
	if(m_brightness > 0) {
		set(static_cast<std::uint8_t>(m_brightness - 1));
	}
}

void device_with_brightness::internal_update(std::uint8_t brightness) {
	auto previous_brightness = m_brightness;
	m_brightness = brightness;
	logger::log("[" + name() + "] update brightness: " + std::to_string(previous_brightness) + " -> " + std::to_string(m_brightness));

}
