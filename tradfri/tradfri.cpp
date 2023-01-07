//
//  tradfri.cpp
//  tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include <cpp-tradfri/system.h>
#include <iostream>

std::uint8_t load() {
	std::string value;
	std::cin >> value;
	return std::stoul(value);
}


int main(int argc, const char * argv[]) {
	try {
		auto system = tradfri::system("172.0.0.1", "identity", "key");
		system.enumerate_devices();
		auto& bulbs = system.bulbs();
		auto& plugs = system.plugs();
		bool run = true;
		while(run) {
			for(std::size_t i = 0; i < bulbs.size(); ++i) {
				auto& bulb = bulbs[i];
				std::cout << (i < 10 ? " ": "") << i << " : Bulb [" << static_cast<int>(bulb.brightness()) << "] " << bulb.name() << std::endl;
			}
			for(std::size_t i = 0; i < plugs.size(); ++i) {
				auto& plug = plugs[i];
				std::cout << (i+bulbs.size() < 10 ? " ": "") << (i+bulbs.size()) << " : Plug [" << static_cast<int>(plug.enabled()) << "] " << plug.name() << std::endl;
			}
			std::cout << "\nType [index] [brightness] fot bulb or [index] [state] for plug:" << std::endl;
			auto index = load();
			auto state = load();
			if(index < bulbs.size()) {
				bulbs.at(index).set(state);
			}
			else {
				auto& plug = plugs.at(index - bulbs.size());
				switch(state) {
					case 0:
						plug.set(false);
						break;
					case 1:
						plug.set(true);
						break;
					default:
						plug.toggle();
						break;
				}
			}
		}
		return 0;
	}
	catch(std::exception &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	catch(...) {
		std::cerr << "Exception." << std::endl;
	}
	return 1;
}
