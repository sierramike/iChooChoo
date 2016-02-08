#pragma once

#define I2C_ADDR_MIN 0x08
#define I2C_ADDR_MAX 0x77

int I2CSetup(int addr, uint8_t busId);

int I2CScan(uint8_t busId, uint8_t* data);

int I2CRead(int fd);

int I2CReadBlock(int fd, uint8_t* myData);

int I2CWriteBlock(int fd, uint8_t* bfr);

void PrintBuffer(uint8_t* bfr, int count);

