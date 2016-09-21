#pragma once

#include "cConfModule.hpp"
#include "IConfModuleOnOffOutputs.hpp"
#include <string.h>

class cConfModuleLighting : public cConfModule, IConfModuleOnOffOutputs
{
	public:
		cConfModuleLighting();
		~cConfModuleLighting();
		virtual void setOutput(int position, bool value) override;
		virtual bool getOutput(int position) override;

	private:
		bool _bOutputs[10] = { };
};
