#pragma once

#include "main.h"
#include "IConfModuleOnOffOutputs.hpp"

void* ServerThread(void* ptr);
bool CheckMessage (char* message, int size);
int ProcessMessage(char* message, char* buffer_out);
ModuleIdent* GetModuleIdent(int addr);
