//
//  dirigera_device.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"
#include "http_connection.h"
#include "json.h"

namespace ikea {

class dirigera_device: public virtual device {
public:
	dirigera_device(dirigera_device const&) = delete;
	dirigera_device(dirigera_device&&) = default;
	
	std::string const& id() const {
		return m_id;
	}
	
	virtual void update_state(json_value const& json) = 0;
	
protected:
	dirigera_device(std::string const& devices_uri, http_get& get_connection, json_value const& json);
	static std::string get_name(json_value const& json);
	
	std::string const m_uri;
	http_get&         m_get_connection;
	http_patch        m_patch_connection;
	
private:
	bool needs_update() const override;
	
	std::string const m_id;
	
};

}
