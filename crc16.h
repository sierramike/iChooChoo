#pragma once

/* CRC calculation macros */
//#define CRC_INIT 0xFFFF
//#define CRC(crcval,newchar) crcval = (crcval >> 8) ^ crc_table[(crcval ^ newchar) & 0x00ff]

unsigned short ComputeCRC(uint8_t* b);

int CheckCRC(uint8_t* b);

void PutCRC(uint8_t* b);
