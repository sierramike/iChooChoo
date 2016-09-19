#include <Wire.h>
#include "iChooChoo.h"
#include "BICCP.h"
#include "SoftReset.h"

#define DEBUG
#define DEBUGDEEP1

#define EMERGENCYSTOP_PIN 2

// 0 = New command to be processed
// 1 = Command processed, answer not send
// 2 = Answer sent, ready to receive new command
byte bCommandProcessed = 2;

uint32_t ResetRequest = 0;

void setup() {
  wdt_disable();
#ifdef DEBUG
  Serial.begin(115200); // start serial for output
#endif

  memset(cfDescription, 0, DESCSIZE + 1);

  EEPROM_ReadSettings();

  memset(bfrIN, 0, PACKETSIZE);
  memset(bfrOUT, 0, PACKETSIZE);
  memset(iOutputs, 0, MAXOUTPUTS * sizeof(int));

  Wire.begin(cfI2Caddr);
  Wire.onReceive(I2CReceive);
  Wire.onRequest(I2CSend);

  // Prepare emergency stop
  pinMode(EMERGENCYSTOP_PIN, INPUT);
  bEmergencyStop = digitalRead(EMERGENCYSTOP_PIN);

  switch (cfModuleType)
  {
    case MODTYPE_GENPURP:
      setupGenPurp();
      break;
    case MODTYPE_LIGHT:
      setupLight();
      break;
  }

#ifdef DEBUG
  Serial.println("Ready!");
#endif
}

void loop() {
  bEmergencyStop = digitalRead(EMERGENCYSTOP_PIN);
  
  if (bCommandProcessed == 0)
  {
#ifdef DEBUG
    Serial.print(F("\r\nloop-Command not processed, bfrIN:"));
    PrintBuffer(bfrIN, PACKETSIZE);
    Serial.println();
#endif
    memset(bfrOUT, 0, PACKETSIZE);
    if (CheckCRC(bfrIN))
    {
#ifdef DEBUG
      Serial.println(F("loop-CRC:OK"));
#endif
      processCommandConf();
      processCommandGenPurp();
      processCommandLight();
      PutCRC(bfrOUT);
      bCommandProcessed = 1;
    }
    else
    {
#ifdef DEBUG
      Serial.println(F("loop-CRC:Error, ignoring"));
#endif
      bCommandProcessed = 2; // We don't want to answer to ignored command
    }
#ifdef DEBUG
    Serial.print(F("loop-Command processed, bfrOUT:"));
    PrintBuffer(bfrOUT, PACKETSIZE);
    Serial.println();
#endif
  }
  
  switch (cfModuleType)
  {
    case MODTYPE_GENPURP:
      loopGenPurp();
      break;
    case MODTYPE_LIGHT:
      loopLight();
      break;
  }

  if (ResetRequest > 0)
    if (micros() - ResetRequest > 4000000L)
        asm volatile ("  jmp 0");
}

void I2CReceive(int byteCount)
{
#ifdef DEBUGDEEP
  Serial.print(F("**I2CRx:Cnt="));
  Serial.print(byteCount);
#endif
  int iIndex = 0;
  while(Wire.available())
  {
    if ((iIndex < PACKETSIZE) && (bCommandProcessed == 2))
    {
      bfrIN[iIndex] = Wire.read();
    }
    else
    {
      byte b = Wire.read();
#ifdef DEBUGDEEP
  Serial.print(F("-/"));
  Serial.print(b, HEX);
#endif
    }
    iIndex++;
  }
  if ((bCommandProcessed == 2) && (bfrIN[0] != 0xCC))
    bCommandProcessed = 0;
#ifdef DEBUGDEEP
  Serial.println();
#endif
}

// callback for sending data
void I2CSend()
{
  if (bCommandProcessed == 1)
  {
#ifdef DEBUGDEEP
  Serial.println(F("**I2CTx:Full"));
#endif
    Wire.write(bfrOUT, 20);
    bCommandProcessed = 2;
  }
  else
  {
#ifdef DEBUGDEEP
  Serial.println(F("**I2CTx:0xCC"));
#endif
    Wire.write(0xCC);
  }
}

#ifdef DEBUG
void PrintBuffer(byte* buf, int count)
{
  int c = (count > PACKETSIZE ? PACKETSIZE : count);
  for (int i = 0; i < c; i++)
  {
    Serial.print("-");
    Serial.print(buf[i], HEX);
  }
}
#endif

