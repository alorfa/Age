#pragma once

#ifdef _DEBUG

#include <iostream>

#define PRINT(value) std::cout << #value " = " << (value) << std::endl
#define PRINTR(value) std::cout << "\r" #value " = " << (value) << "                "
#define DEBUG(value) std::cout << (value) << std::endl
#define DEBUGSTREAM std::cout

#else

#define PRINT(value)
#define PRINTR(value) 
#define DEBUG(value)
#define DEBUGSTREAM

#endif //RENGINE_DEBUG