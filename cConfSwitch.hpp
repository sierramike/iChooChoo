#pragma once

#include "cConfModuleAttachedElement.hpp"
#include <string.h>

class cConfSwitch : public cConfModuleAttachedElement
{
	public:
		cConfSwitch();
		~cConfSwitch();
		virtual void setStraightValue(int straightValue);
		virtual int getStraightValue();

	private:
		int _iStraightValue;
};
