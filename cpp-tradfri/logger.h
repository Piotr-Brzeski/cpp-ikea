//
//  logger.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-02-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <iostream>

template<class T>
void log(T const& message) {
	std::cout << message << std::endl;
}
