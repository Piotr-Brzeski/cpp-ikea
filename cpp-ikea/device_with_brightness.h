//
//  device_with_brightness.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"
#include <cstdint>
#include <atomic>

namespace ikea {

class device_with_brightness: public virtual device {
public:
	static constexpr std::uint8_t zero_brightness = 0;
	static constexpr std::uint8_t min_brightness = 1;
	static constexpr std::uint8_t max_brightness = 7;
	
	device_with_brightness() = default;
	device_with_brightness(device_with_brightness&&);
	
	std::uint8_t brightness();
	bool enabled();
	void set(bool enabled);
	void set(std::uint8_t brightness);
	void toggle();
	void increase();
	void decrease();
	
protected:
	void internal_update(std::uint8_t brightness);
	std::uint8_t m_brightness_to_send = 0;
	
private:
	std::atomic<std::uint8_t> m_brightness = 0;
};

}
