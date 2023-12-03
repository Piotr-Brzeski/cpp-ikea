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
	device(device&&) = default;
	
	std::string const& name() const {
		return m_name;
	}
	
protected:
	device(std::string const& name);
	void update();
	void send();
	
	std::string const m_name;
	
private:
	virtual bool needs_update() const;
	virtual void get_state() = 0;
	virtual void send_state() = 0;
	
	std::chrono::steady_clock::time_point m_last_update_time;
};

}
