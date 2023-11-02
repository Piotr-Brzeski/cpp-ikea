//
//  device.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <string>
#include <chrono>

namespace ikea {

class device {
public:
	device(device const&) = delete;
	device(device&&) = default;
	
	std::string const& name() const {
		return m_name;
	}
	
protected:
	device(std::string uri, std::string name);
	bool needs_update() const;
	void updated();
	
	std::string const& uri() const {
		return m_uri;
	}
	
private:
	std::string                           m_uri;
	std::string                           m_name;
	std::chrono::steady_clock::time_point m_last_update_time;
};

}
