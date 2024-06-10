#pragma once

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN  

#ifdef _DEBUG
#pragma comment(lib, "Debug\\Engine.lib")
#else
#pragma comment(lib, "Release\\Engine.lib")
#endif 

#include "CorePch.h"
