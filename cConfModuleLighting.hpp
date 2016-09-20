#pragma once

#include "cConfModule.hpp"
#include "iConfModuleOnOffOutputs.hpp"
#include <string.h>

class cConfModuleLighting : public cConfModule, iConfModuleOnOffOutputs
{
	public:
		cConfModuleLighting();
		~cConfModuleLighting();
		virtual void setOutput(int position, bool value);
		virtual bool getOutput(int position);

	private:
		bool _bOutputs[10] = { };
};
