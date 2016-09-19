#pragma once

#define PROG_VERSION_MAJ  0x00 // 0
#define PROG_VERSION_MIN  0x1E //30
#define PROG_VERSION_BLD  0x00 // 0

#define MODTYPE_GENPURP   0x20
#define MODTYPE_LIGHT     0x21

#define PACKETSIZE 20
#define DESCSIZE 14
#define MAXOUTPUTS 16

// I2C Addresses goes from 0x08 to 0x77 = 112 total addresses
#define I2C_ADDR_MIN 0x08
#define I2C_ADDR_MAX 0x77
byte cfI2Caddr = I2C_ADDR_MAX;
byte cfModuleType = 0;
char cfDescription[DESCSIZE + 1] = "";

byte bfrIN[PACKETSIZE];
byte bfrOUT[PACKETSIZE];

int iOutputs[MAXOUTPUTS];

byte bEmergencyStop = LOW;
