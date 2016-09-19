void processCommandGenPurp()
{
  if (cfModuleType == MODTYPE_GENPURP)
  {
    if (bfrIN[0] == BICCP_GRP_GENPURP)
    {
      switch(bfrIN[1])
      {
        case BICCP_CMD_GENPURP_IDENT:
          processGenPurpIdent();
          break;
        case BICCP_CMD_GENPURP_PRESET:
          processGenPurpPreset();
          break;

        default:
          if (bfrIN[1] >= BICCP_CMD_GENPURP_OUT_STR && bfrIN[1] <= BICCP_CMD_GENPURP_OUT_END)
            processGenPurpSet();
      }
    }
  }
}

void processGenPurpIdent()
{
#ifdef DEBUG
  Serial.println(F("modGENPURP-processGenPurpIdent"));
#endif
  bfrOUT[0] = BICCP_GRP_GENPURP;
  bfrOUT[1] = BICCP_CMD_GENPURP_IDENT;
  for (int i = 0; i < MAXOUTPUTS; i++)
  {
    bfrOUT[2 + i] = (iOutputs[i] ? 0xFF : 0x00);
  }
}

void processGenPurpPreset()
{
#ifdef DEBUG
  Serial.println(F("modGENPURP-processGenPurpPreset"));
#endif
  int iError = 0;
  for (int i = 0; i < MAXOUTPUTS; i++)
  {
    if (bfrIN[2 + i] != 0xFF && bfrIN[2 + i] != 0x00)
      iError = 1;
  }
  if (iError = 0)
  {
    for (int i = 0; i < MAXOUTPUTS; i++)
      iOutputs[i] = (bfrIN[2 + i] ? HIGH : LOW);
  }
  bfrOUT[0] = BICCP_GRP_GENPURP;
  bfrOUT[1] = BICCP_CMD_GENPURP_PRESET;
  bfrOUT[2] = (iError == 0 ? BICCP_SUCCESS : BICCP_ERROR);
}

void processGenPurpSet()
{
#ifdef DEBUG
  Serial.println(F("modGENPURP-processGenPurpSet"));
#endif
  if (bfrIN[2] == 0xFF || bfrIN[2] == 0x00)
  {
    iOutputs[bfrIN[1] - BICCP_CMD_GENPURP_OUT_STR] = (bfrIN[2] ? HIGH : LOW);
    bfrOUT[2] == BICCP_SUCCESS;
  }
  else
    bfrOUT[2] == BICCP_ERROR;

  bfrOUT[0] = BICCP_GRP_GENPURP;
  bfrOUT[1] = bfrIN[1];
}

void setupGenPurp()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A6, OUTPUT);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A6, LOW);
}

void loopGenPurp()
{
  digitalWrite(3, iOutputs[0] ? HIGH : LOW);
  digitalWrite(4, iOutputs[1] ? HIGH : LOW);
  digitalWrite(5, iOutputs[2] ? HIGH : LOW);
  digitalWrite(6, iOutputs[3] ? HIGH : LOW);
  digitalWrite(7, iOutputs[4] ? HIGH : LOW);
  digitalWrite(8, iOutputs[5] ? HIGH : LOW);
  digitalWrite(9, iOutputs[6] ? HIGH : LOW);
  digitalWrite(10, iOutputs[7] ? HIGH : LOW);
  digitalWrite(11, iOutputs[8] ? HIGH : LOW);
  digitalWrite(12, iOutputs[9] ? HIGH : LOW);
  digitalWrite(13, iOutputs[10] ? HIGH : LOW);
  digitalWrite(A0, iOutputs[11] ? HIGH : LOW);
  digitalWrite(A1, iOutputs[12] ? HIGH : LOW);
  digitalWrite(A2, iOutputs[13] ? HIGH : LOW);
  digitalWrite(A3, iOutputs[14] ? HIGH : LOW);
  digitalWrite(A6, iOutputs[15] ? HIGH : LOW);
}

void emerGenPurp()
{
  setupGenPurp();
}

