void processCommandLight()
{
  if (cfModuleType == MODTYPE_LIGHT)
  {
    if (bfrIN[0] == BICCP_GRP_LIGHT)
    {
      switch(bfrIN[1])
      {
        case BICCP_CMD_LIGHT_IDENT:
          processLightIdent();
          break;
        case BICCP_CMD_LIGHT_PRESET:
          processLightPreset();
          break;

        default:
          if (bfrIN[1] >= BICCP_CMD_LIGHT_OUT_STR && bfrIN[1] <= BICCP_CMD_LIGHT_OUT_END)
            processLightSet();
      }
    }
  }
}

void processLightIdent()
{
#ifdef DEBUG
  Serial.println(F("modLIGHT-processLightIdent"));
#endif
  bfrOUT[0] = BICCP_GRP_GENPURP;
  bfrOUT[1] = BICCP_CMD_GENPURP_IDENT;
  for (int i = 0; i < MAXOUTPUTS; i++)
  {
    if (i < 0xA)
      bfrOUT[2 + i] = (iOutputs[i] ? 0xFF : 0x00);
    else
      bfrOUT[2 + i] = iOutputs[i];
  }
}

void processLightPreset()
{
#ifdef DEBUG
  Serial.println(F("modLIGHT-processLightPreset"));
#endif
  int iError = 0;
  for (int i = 0; i < 10; i++)
  {
    if (bfrIN[2 + i] != 0xFF && bfrIN[2 + i] != 0x00)
      iError = 1;
  }
  if (iError = 0)
  {
    for (int i = 0; i < 10; i++)
      iOutputs[i] = (bfrIN[2 + i] ? HIGH : LOW);
    for (int i = 10; i < MAXOUTPUTS; i++)
      iOutputs[i] = bfrIN[2 + i];
  }
  bfrOUT[0] = BICCP_GRP_GENPURP;
  bfrOUT[1] = BICCP_CMD_GENPURP_PRESET;
  bfrOUT[2] = (iError == 0 ? BICCP_SUCCESS : BICCP_ERROR);
}

void processLightSet()
{
#ifdef DEBUG
  Serial.println(F("modLIGHT-processLightSet"));
#endif
  if (bfrIN[2] == 0xFF || bfrIN[2] == 0x00)
  {
    iOutputs[bfrIN[1] - BICCP_CMD_LIGHT_OUT_STR] = (bfrIN[2] ? HIGH : LOW);
    bfrOUT[2] == BICCP_SUCCESS;
  }
  else
    bfrOUT[2] == BICCP_ERROR;

  bfrOUT[0] = BICCP_GRP_LIGHT;
  bfrOUT[1] = bfrIN[1];
}

void processLightSetDimmable()
{
#ifdef DEBUG
  Serial.println(F("modLIGHT-processLightSetDimmable"));
#endif
  iOutputs[bfrIN[1] - BICCP_CMD_LIGHT_OUT_STR] = bfrIN[2];

  bfrOUT[0] = BICCP_GRP_GENPURP;
  bfrOUT[1] = bfrIN[1];
  bfrOUT[2] == BICCP_SUCCESS;
}

void setupLight()
{
  // Non dimmable
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A6, OUTPUT);
  // Dimmable
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  // Non dimmable
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A6, LOW);
  // Dimmable
  analogWrite(3, 0);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(9, 0);
  analogWrite(10, 0);
  analogWrite(11, 0);
}

void loopLight()
{
  digitalWrite(4, iOutputs[0] ? HIGH : LOW);
  digitalWrite(7, iOutputs[1] ? HIGH : LOW);
  digitalWrite(8, iOutputs[2] ? HIGH : LOW);
  digitalWrite(12, iOutputs[3] ? HIGH : LOW);
  digitalWrite(13, iOutputs[4] ? HIGH : LOW);
  digitalWrite(A0, iOutputs[5] ? HIGH : LOW);
  digitalWrite(A1, iOutputs[6] ? HIGH : LOW);
  digitalWrite(A2, iOutputs[7] ? HIGH : LOW);
  digitalWrite(A3, iOutputs[8] ? HIGH : LOW);
  digitalWrite(A6, iOutputs[9] ? HIGH : LOW);

  digitalWrite(3, iOutputs[0xA]);
  digitalWrite(5, iOutputs[0xB]);
  digitalWrite(6, iOutputs[0xC]);
  digitalWrite(9, iOutputs[0xD]);
  digitalWrite(10, iOutputs[0xE]);
  digitalWrite(11, iOutputs[0xF]);
}

void emerLight()
{
  setupLight();
}

