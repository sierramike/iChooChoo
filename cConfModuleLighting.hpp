#pragma once

#include "cConfModule.hpp"
#include "IConfModuleOnOffOutputs.hpp"
#include "IConfModuleDimmableOutputs.hpp"
#include <string.h>

class cConfModuleLighting : public cConfModule, public IConfModuleOnOffOutputs, public IConfModuleDimmableOutputs
{
	public:
		cConfModuleLighting();
		~cConfModuleLighting();

		virtual int writeStatus(char* buffer) override;

		virtual bool setOutput(int position, bool value) override;
		virtual bool getOutput(int position) override;

		virtual bool setDimmableOutput(int position, uint8_t value) override;
		virtual uint8_t getDimmableOutput(int position) override;

	private:
		bool _bOutputs[10] = { };
		uint8_t _bDimmableOutputs[6] = { };
};
