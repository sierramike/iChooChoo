#pragma once

#include "cConfModule.hpp"
#include "iConfModuleOnOffOutputs.hpp"
#include <string.h>

class cConfModuleGenPurp : public cConfModule
{
	public:
		cConfModuleGenPurp();
		~cConfModuleGenPurp();
		virtual int writeStatus(char* buffer) override;
		virtual void setOutput(int position, bool value);
		virtual bool getOutput(int position);

	private:
		bool _bOutputs[16] = { };
};
