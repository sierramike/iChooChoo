#pragma once

#include "cConfModule.hpp"
#include "IConfModuleOnOffOutputs.hpp"
#include <string.h>

class cConfModuleGenPurp : public cConfModule, public IConfModuleOnOffOutputs
{
	public:
		cConfModuleGenPurp();
		~cConfModuleGenPurp();
		virtual int writeStatus(char* buffer) override;
		virtual void setOutput(int position, bool value) override;
		virtual bool getOutput(int position) override;

	private:
		bool _bOutputs[16] = { };
};
