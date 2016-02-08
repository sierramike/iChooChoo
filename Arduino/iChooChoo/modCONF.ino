void processCommandConf()
{
  if (bfrIN[0] == BICCP_GRP_CONF)
  {
    switch(bfrIN[1])
    {
      case BICCP_CMD_CONF_IDENT:
        processConfIdent();
        break;
      case BICCP_CMD_CONF_VERSION:
        processConfVersion();
        break;
      case BICCP_CMD_CONF_ADDR:
        processConfAddress();
        break;
      case BICCP_CMD_CONF_TYPE:
        processConfType();
        break;
      case BICCP_CMD_CONF_DESC:
        processConfDescription();
        break;
      case BICCP_CMD_CONF_SOFTRST:
        processConfSoftReset();
        break;
      case BICCP_CMD_CONF_HARDRST:
        processConfHardReset();
        break;
    }
  }
}

void processConfIdent()
{
#ifdef DEBUG
  Serial.println(F("modCONF-processConfIdent"));
#endif
  bfrOUT[0] = BICCP_GRP_CONF;
  bfrOUT[1] = BICCP_CMD_CONF_IDENT;
  bfrOUT[2] = cfI2Caddr;
  bfrOUT[3] = cfModuleType;
  memcpy(bfrOUT + 4, cfDescription, 14);
}

void processConfVersion()
{
#ifdef DEBUG
  Serial.println(F("modCONF-processConfVersion"));
#endif
  bfrOUT[0] = BICCP_GRP_CONF;
  bfrOUT[1] = BICCP_CMD_CONF_VERSION;
  bfrOUT[2] = PROG_VERSION_MAJ;
  bfrOUT[3] = PROG_VERSION_MIN;
  bfrOUT[4] = PROG_VERSION_BLD;
}

void processConfAddress()
{
#ifdef DEBUG
  Serial.println(F("modCONF-processConfAddress"));
#endif
  int i = EEPROM_writeI2Caddr(bfrIN[2]);
  bfrOUT[0] = BICCP_GRP_CONF;
  bfrOUT[1] = BICCP_CMD_CONF_ADDR;
  bfrOUT[2] = (i ? BICCP_SUCCESS : BICCP_ERROR);
}

void processConfType()
{
#ifdef DEBUG
  Serial.println(F("modCONF-processConfType"));
#endif
  int i = EEPROM_writeType(bfrIN[2]);
  bfrOUT[0] = BICCP_GRP_CONF;
  bfrOUT[1] = BICCP_CMD_CONF_TYPE;
  bfrOUT[2] = (i ? BICCP_SUCCESS : BICCP_ERROR);
}

void processConfDescription()
{
#ifdef DEBUG
  Serial.println(F("modCONF-processConfDesc"));
#endif
  for(int i = 2; i < DESCSIZE + 2; i++)
  {
    if (bfrIN[i] == 0)
    {
      memset(bfrIN + i + 1, 0, DESCSIZE + 2 - i);
      continue;
    }
  }
  memcpy(cfDescription, bfrIN + 2, DESCSIZE + 1);
  int i = EEPROM_writeDesc(cfDescription);
  bfrOUT[0] = BICCP_GRP_CONF;
  bfrOUT[1] = BICCP_CMD_CONF_DESC;
  bfrOUT[2] = (i ? BICCP_SUCCESS : BICCP_ERROR);
}

void processConfSoftReset()
{
#ifdef DEBUG
  Serial.println(F("modCONF-processConfSoftReset"));
#endif
  bfrOUT[0] = BICCP_GRP_CONF;
  bfrOUT[1] = BICCP_CMD_CONF_SOFTRST;
  bfrOUT[2] = BICCP_SUCCESS;

  ResetRequest = micros();
}

void processConfHardReset()
{
#ifdef DEBUG
  Serial.println(F("modCONF-processConfHardReset"));
#endif
  EEPROM_Erase();
  bfrOUT[0] = BICCP_GRP_CONF;
  bfrOUT[1] = BICCP_CMD_CONF_HARDRST;
  bfrOUT[2] = BICCP_SUCCESS;

  ResetRequest = micros();
}

