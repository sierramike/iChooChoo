#include "ConfManager.hpp"
#include <iostream>
#include <fstream>

ConfManager::ConfManager()
{
}

ConfManager::~ConfManager()
{
}

bool ConfManager::ReadConf(const char* path)
{
	bool bReturn = true;
	string myLine, myMotionLine;
	int iLine = 0;

	ifstream myfile(path);
	if (myfile.is_open())
	{
		bool bHeaderFound = false;
		while (getline(myfile, myLine) && !bHeaderFound)
		{
			iLine++;
			string line = trim(myLine);
//			printf("%s\n", myLine.c_str());
			if (myLine.length() > 0 && myLine[0] != '#')
				if (line.compare(0, 17, "ICHOOCHOO_CONF V1") == 0)
					bHeaderFound = true;
		}
		if (bHeaderFound)
		{
			while (getline(myfile, myLine) && bReturn)
			{
				iLine++;
				string line = trim(myLine);
				if (line.length() > 0 && line[0] != '#')
				{
					if (line.compare(0, 9, "POSITION ") == 0)
					{
						int iID = -1;
						char* sDesc;
						sscanf(line.c_str(), "POSITION %02x %ms", &iID, &sDesc);
						if (iID > -1)
						{
							cConfPosition *ccPos = new cConfPosition();
							ccPos->setID(iID);
							ccPos->setDescription(sDesc);
							Positions[iID] = ccPos;
						}
						else
						{
							bReturn = false;
							printf("Configuration error: syntax error at line %d '%s'\n", iLine, line.c_str());
						}
						free(sDesc);
					}
					else if (line.compare(0, 7, "MODULE ") == 0)
					{
						int iID = -1;
						int iType = -1;
						char* sDesc;
						sscanf(line.c_str(), "MODULE %02x %02x %ms", &iID, &iType, &sDesc);
						if (iID > -1 && iType > -1)
						{
							cConfModule *ccMod = ConfModuleFactory(iType); // new cConfModule();
							ccMod->setID(iID);
							//ccMod->setType(iType);
							ccMod->setDescription(sDesc);
							Modules[iID] = ccMod;
						}
						else
						{
							bReturn = false;
							printf("Configuration error: syntax error at line %d '%s'\n", iLine, line.c_str());
						}
						free(sDesc);
					}
					else if (line.compare(0, 8, "SECTION ") == 0)
					{
						int iID = -1;
						int iModuleAddr = -1;
						int iOutput = 0;
						char* sDesc;
						sscanf(line.c_str(), "SECTION %02x %02x %01x %ms", &iID, &iModuleAddr, &iOutput, &sDesc);
						if (iID > -1 && iModuleAddr > -1)
						{
							cConfSection *ccSec = new cConfSection();
							ccSec->setID(iID);
							ccSec->setModule(Modules[iModuleAddr]);
							ccSec->setIOPort(iOutput);
							ccSec->setDescription(sDesc);
							Sections[iID] = ccSec;
						}
						else
						{
							bReturn = false;
							printf("Configuration error: syntax error at line %d '%s'\n", iLine, line.c_str());
						}
						free(sDesc);
					}
					else if (line.compare(0, 7, "SWITCH ") == 0)
					{
						int iID = -1;
						int iModuleAddr = -1;
						int iOutput = 0;
						int iStraightValue = 0;
						char* sDesc;
						sscanf(line.c_str(), "SWITCH %02x %02x %01x %d %ms", &iID, &iModuleAddr, &iOutput, &iStraightValue, &sDesc);
						if (iID > -1 && iModuleAddr > -1)
						{
							cConfSwitch *ccSwi = new cConfSwitch();
							ccSwi->setID(iID);
							ccSwi->setModule(Modules[iModuleAddr]);
							ccSwi->setIOPort(iOutput);
							ccSwi->setStraightValue(iStraightValue);
							ccSwi->setDescription(sDesc);
							Switchs[iID] = ccSwi;
						}
						else
						{
							bReturn = false;
							printf("Configuration error: syntax error at line %d '%s'\n", iLine, line.c_str());
						}
						free(sDesc);
					}
					else if (line.compare(0, 6, "RELAY ") == 0)
					{
						int iID = -1;
						int iModuleAddr = -1;
						int iOutput = 0;
						char* sDesc;
						sscanf(line.c_str(), "RELAY %02x %02x %01x %ms", &iID, &iModuleAddr, &iOutput, &sDesc);
						if (iID > -1 && iModuleAddr > -1)
						{
							cConfRelay *ccRel = new cConfRelay();
							ccRel->setID(iID);
							ccRel->setModule(Modules[iModuleAddr]);
							ccRel->setIOPort(iOutput);
							ccRel->setDescription(sDesc);
							Relays[iID] = ccRel;
						}
						else
						{
							bReturn = false;
							printf("Configuration error: syntax error at line %d '%s'\n", iLine, line.c_str());
						}
						free(sDesc);
					}
					else if (line.compare(0, 7, "SENSOR ") == 0)
					{
						int iID = -1;
						int iModuleAddr = -1;
						int iOutput = 0;
						int iType = -1;
						char* sDesc;
						sscanf(line.c_str(), "SENSOR %02x %02x %01x %02x %ms", &iID, &iModuleAddr, &iOutput, &iType, &sDesc);
						if (iID > -1 && iModuleAddr > -1 && iType > -1)
						{
							cConfSensor *ccSen = new cConfSensor();
							ccSen->setID(iID);
							ccSen->setModule(Modules[iModuleAddr]);
							ccSen->setIOPort(iOutput);
							ccSen->setType(iType);
							ccSen->setDescription(sDesc);
							Sensors[iID] = ccSen;
						}
						else
						{
							bReturn = false;
							printf("Configuration error: syntax error at line %d '%s'\n", iLine, line.c_str());
						}
						free(sDesc);
					}
					else if (line.compare(0, 7, "MOTION ") == 0)
					{
						bool bInMotion = true;
						while (getline(myfile, myMotionLine) && bInMotion && bReturn)
						{
							iLine++;
							string motionline = trim(myMotionLine);
							if (motionline.compare(0, 9, "ENDMOTION") == 0)
							{
								bInMotion = false;
							}
						}
					}
				}
			}
		}
		else
			bReturn = false;

		myfile.close();
	}
	return bReturn;
}

void ConfManager::Display(std::ostream &os)
{
	os << "ICHOOCHOO_CONF V1\n";
	os << "\n";

	os << "#POSITION ID DESCRIPTION\n";
	typedef std::map<int, cConfPosition*>::iterator it_pos;
	for(it_pos iterator = Positions.begin(); iterator != Positions.end(); ++iterator)
	{
		cConfPosition* ccPos = iterator->second;
		os << "POSITION " << std::hex << std::uppercase << ccPos->getID() << " " << ccPos->getDescription() << "\n";
	}

	os << "\n";
	os << "#MODULE ID TYPE DESCRIPTION\n";
	typedef std::map<int, cConfModule*>::iterator it_mod;
	for(it_mod iterator = Modules.begin(); iterator != Modules.end(); ++iterator)
	{
		cConfModule* ccMod = iterator->second;
		os << "MODULE " << std::hex << std::uppercase << ccMod->getID() << " " << std::hex << std::uppercase << ccMod->getType() << " " << ccMod->getDescription() << "\n";
	}

	os << "\n";
	os << "#SECTION ID MODULE_ID OUTPUT DESCRIPTION\n";
	typedef std::map<int, cConfSection*>::iterator it_sec;
	for(it_sec iterator = Sections.begin(); iterator != Sections.end(); ++iterator)
	{
		cConfSection* ccSec = iterator->second;
		os << "SECTION " << std::hex << std::uppercase << ccSec->getID() << " " << std::hex << std::uppercase << ccSec->getModule()->getID()
			 << " " << std::hex << ccSec->getIOPort() << " " << ccSec->getDescription() << "\n";
	}

	os << "\n";
	os << "#SWITCH ID MODULE_ID OUTPUT STRAIGHTVALUE DESCRIPTION\n";
	typedef std::map<int, cConfSwitch*>::iterator it_swi;
	for(it_swi iterator = Switchs.begin(); iterator != Switchs.end(); ++iterator)
	{
		cConfSwitch* ccSwi = iterator->second;
		os << "SWITCH " << std::hex << std::uppercase << ccSwi->getID() << " " << std::hex << std::uppercase << ccSwi->getModule()->getID()
			 << " " << std::hex << ccSwi->getIOPort() << " " << ccSwi->getStraightValue() << " " << ccSwi->getDescription() << "\n";
	}

	os << "\n";
	os << "#RELAY ID MODULE_ID OUTPUT DESCRIPTION\n";
	typedef std::map<int, cConfRelay*>::iterator it_rel;
	for(it_rel iterator = Relays.begin(); iterator != Relays.end(); ++iterator)
	{
		cConfRelay* ccRel = iterator->second;
		os << "RELAY " << std::hex << std::uppercase << ccRel->getID() << " " << std::hex << std::uppercase << ccRel->getModule()->getID()
			 << " " << std::hex << ccRel->getIOPort() << " " << ccRel->getDescription() << "\n";
	}

	os << "\n";
	os << "#SENSOR ID MODULE_ID INPUT TYPE DESCRIPTION\n";
	typedef std::map<int, cConfSensor*>::iterator it_sen;
	for(it_sen iterator = Sensors.begin(); iterator != Sensors.end(); ++iterator)
	{
		cConfSensor* ccSen = iterator->second;
		os << "SENSOR " << std::hex << std::uppercase << ccSen->getID() << " " << std::hex << std::uppercase << ccSen->getModule()->getID()
			 << " " << std::hex << ccSen->getIOPort() << " " << std::hex << std::uppercase << ccSen->getType() << " " << ccSen->getDescription() << "\n";
	}
}

cConfModule* ConfManager::ConfModuleFactory(int type)
{
	cConfModule* ret = 0;

	if (type == BICCP_GRP_TRACTION)
		ret = new cConfModuleTraction();
	else if (type == BICCP_GRP_GENPURP)
		ret = new cConfModuleGenPurp();
	else if (type == BICCP_GRP_LIGHTING)
		ret = new cConfModuleLighting();

	return ret;
}
