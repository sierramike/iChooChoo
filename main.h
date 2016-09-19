#pragma once

#include "ConfManager.hpp"

ConfManager* conf;

int ProcessArguments(int argc, char* argv[]);

int CheckArgument(int argc, char* argv[], int argi, char* argname, int nparam);

int CheckArgumentGetByte(int argc, char* argv[], int argi, char* argname, int nparam, uint8_t* data);

void PrintModuleList(struct ModuleIdent* list, int iNbModules);

int PrintVersion();

int PrintHelp(char* executableName);
