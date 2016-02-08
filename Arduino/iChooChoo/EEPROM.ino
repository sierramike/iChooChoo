#include <EEPROM.h>

#define EEPROM_SIZE 1024
#define CONF_VERSION 0x11
// 0x10 = iChooChoo Conf v1

void EEPROM_Erase()
{
  for(int i = 0; i < EEPROM_SIZE; i++)
    EEPROM.update(i, 0xFF);
}

void EEPROM_ReadSettings()
{
  byte confExists = EEPROM.read(0);
  if (confExists == CONF_VERSION)
  {
    cfI2Caddr = EEPROM.read(1);
    cfModuleType = EEPROM.read(2);
    for(int i = 0; i < DESCSIZE; i++)
    {
      uint8_t c = EEPROM.read(3 + i);
      if (c != 0xFF)
        cfDescription[i] = (char)c;
    }
    if (cfI2Caddr < I2C_ADDR_MIN || cfI2Caddr > I2C_ADDR_MAX || cfModuleType == 0)
    {
      cfI2Caddr = I2C_ADDR_MAX;
      cfModuleType = 0;
      memset(cfDescription, 0, DESCSIZE + 1);
      EEPROM_Erase();
    }
  }
  else
    EEPROM_Erase();
}

uint8_t EEPROMwritestate = 0;

int EEPROM_writeI2Caddr(byte addr)
{
  if (cfI2Caddr == 0x77 && addr >= I2C_ADDR_MIN && addr <= I2C_ADDR_MAX)
  {
    EEPROM.update(1, addr);
    EEPROMwritestate |= 0x01;
    EEPROM_writeVersion();
    return -1;
  }
  else
    return 0;
}

int EEPROM_writeType(byte type)
{
  if (cfI2Caddr == 0x77 && type != 0)
  {
    EEPROM.update(2, type);
    EEPROMwritestate |= 0x02;
    EEPROM_writeVersion();
    return -1;
  }
  else
    return 0;
}

int EEPROM_writeDesc(char* desc)
{
  if (cfI2Caddr != 0x77)
  {
    for(int i = 0; i < DESCSIZE; i++)
      EEPROM.update(3 + i, *(desc + i));
    return -1;
  }
  else
    return 0;
}

void EEPROM_writeVersion()
{
  if (EEPROMwritestate & 0x03 == 0x03)
    EEPROM.update(0, CONF_VERSION);
}

