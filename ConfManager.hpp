#pragma once
#include <map>
#include "xstdstring.hpp"
#include "biccp.h"
#include "log.h"
#include "cConfPosition.hpp"
#include "cConfModule.hpp"
#include "cConfModuleTraction.hpp"
#include "cConfModuleGenPurp.hpp"
#include "cConfModuleLighting.hpp"
#include "cConfSection.hpp"
#include "cConfSwitch.hpp"
#include "cConfRelay.hpp"
#include "cConfSensor.hpp"

using namespace std;

class ConfManager
{
	public:
		ConfManager();
		~ConfManager();
		bool ReadConf(const char* path);
		void Display(std::ostream &os);
		int ScanBus();

		map<int, cConfPosition*> Positions;
		map<int, cConfModule*> Modules;
		map<int, cConfSection*> Sections;
		map<int, cConfSwitch*> Switchs;
		map<int, cConfRelay*> Relays;
		map<int, cConfSensor*> Sensors;

	private:
		void ClearModules();
		cConfModule* ConfModuleFactory(int type);
		cConfModule* GetModuleIdent(int addr);
};
