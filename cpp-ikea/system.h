//
//  system.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device_with_brightness.h"
#include "device_without_brightness.h"
#include "exception.h"
#include <functional>
#include <variant>
#include <string>
#include <vector>

namespace ikea {

struct no_system {
	using configuration = int;
	struct device: device_without_brightness {
		void get_state() override {}
		void send_state() override {}
	};
	no_system(configuration const&) {}
	void enumerate_devices() {}
	auto& bulbs() { return devices; }
	auto& outlets() { return devices; }
	std::vector<device> devices;
};

template<class system1_t, class system2_t = no_system>
class system: public system1_t, public system2_t {
public:
	system(system1_t::configuration const& configuration1, system2_t::configuration const& configuration2 = {})
		: system1_t(configuration1)
		, system2_t(configuration2)
	{
	}
	
	void enumerate_devices() {
		system1_t::enumerate_devices();
		system2_t::enumerate_devices();
	}
	
	std::function<void(bool)> set_operation(std::string const& device_name) {
		auto device = get_device(device_name, system1_t::bulbs(), system1_t::outlets(), system2_t::bulbs(), system2_t::outlets());
		return [device](bool enable){ std::visit([enable](auto&& device){ device->set(enable); }, device); };
	}
	std::function<std::uint8_t()> brightness_operation(std::string const& device_name) {
		auto device = get_device(device_name, system1_t::bulbs(), system1_t::outlets(), system2_t::bulbs(), system2_t::outlets());
		return [device](){ return std::visit([](auto&& device){ return device->brightness(); }, device); };
	}
	std::function<void()> toggle_operation(std::string const& device_name) {
		auto device = get_device(device_name, system1_t::bulbs(), system1_t::outlets(), system2_t::bulbs(), system2_t::outlets());
		return [device](){ std::visit([](auto&& device){ device->toggle(); }, device); };
	}
	std::function<void()> increase_operation(std::string const& device_name) {
		auto device = get_device(device_name, system1_t::bulbs(), system1_t::outlets(), system2_t::bulbs(), system2_t::outlets());
		return [device](){ std::visit([](auto&& device){ device->increase(); }, device); };
	}
	std::function<void()> decrease_operation(std::string const& device_name) {
		auto device = get_device(device_name, system1_t::bulbs(), system1_t::outlets(), system2_t::bulbs(), system2_t::outlets());
		return [device](){ std::visit([](auto&& device){ device->decrease(); }, device); };
	}
	
private:
	template<typename Devices, typename... Args>
	std::variant<device_without_brightness*, device_with_brightness*> get_device(std::string const& device_name, Devices& devices, Args&... more_devices) {
		for(auto& device : devices) {
			if(device.name() == device_name) {
				return &device;
			}
		}
		if constexpr(sizeof...(more_devices) > 0) {
			return get_device(device_name, more_devices...);
		}
		else {
			throw exception("Device \"" + device_name + "\" not found.");
		}
	}
};

}
