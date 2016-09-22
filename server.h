#pragma once

#include "main.h"
#include "modconf.h"
#include "IConfModuleOnOffOutputs.hpp"
#include "IConfModuleDimmableOutputs.hpp"

void* ServerThread(void* ptr);
bool CheckMessage (char* message, int size);
int ProcessMessage(char* message, char* buffer_out);
