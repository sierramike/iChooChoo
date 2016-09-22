#pragma once

#include "cConfModule.hpp"
#include <string.h>

class cConfModuleTraction : public cConfModule
{
	public:
		cConfModuleTraction();
		~cConfModuleTraction();

		virtual int writeStatus(char* buffer) override;

	private:
};
