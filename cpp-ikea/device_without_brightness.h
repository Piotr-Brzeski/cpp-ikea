//
//  device_without_brightness.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"
#include <atomic>

namespace ikea {

class device_without_brightness: public virtual device {
public:
	device_without_brightness() = default;
	device_without_brightness(device_without_brightness&&);
	
	bool enabled();
	std::uint8_t brightness();
	
	void set(bool enabled);
	void toggle();
	void increase();
	void decrease();
	
protected:
	void internal_update(bool enabled);
	bool m_enabled_to_send = 0;
	
private:
	std::atomic<bool> m_enabled = 0;
};

}
