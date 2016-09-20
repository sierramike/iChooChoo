#pragma once

#include "biccp.h"
#include "cConfModule.hpp"
#include <string.h>

class cConfModuleGenPurp : public cConfModule
{
	public:
		cConfModuleGenPurp();
		~cConfModuleGenPurp();

	private:
		bool[16] _bOutputs;
};
