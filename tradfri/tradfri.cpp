//
//  tradfri.cpp
//  tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include <cpp-tradfri/system.h>
#include <iostream>

int main(int argc, const char * argv[]) {
	try {
		auto system = tradfri::system("127.0.0.1", 0);
		system.enumerate_devices();
		return 0;
	}
	catch(...) {
	}
	return 1;
}
