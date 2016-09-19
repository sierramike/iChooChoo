#pragma once
#include <map>
#include "xstdstring.hpp"
#include "cConfPosition.hpp"
#include "cConfModule.hpp"
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

		map<int, cConfPosition*> Positions;
		map<int, cConfModule*> Modules;
		map<int, cConfSection*> Sections;
		map<int, cConfSwitch*> Switchs;
		map<int, cConfRelay*> Relays;
		map<int, cConfSensor*> Sensors;

	private:
};
