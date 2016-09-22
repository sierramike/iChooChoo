#pragma once

#include "header.h"

//int ScanBus(struct ModuleIdent** list);

//struct ModuleIdent ModConfGetIdent(int addr);

//int ModConfGetIdent2(int addr, struct ModuleIdent* mi);

int ModConfSetAddress(int addr, uint8_t newaddr);

int ModConfSetType(int addr, uint8_t type);

int ModConfSetDescription(int addr, char* desc);

int ModConfSoftReset(int addr);

int ModConfHardReset(int addr);
