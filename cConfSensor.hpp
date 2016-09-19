#pragma once

#include "cConfModuleAttachedElement.hpp"
#include <string.h>

class cConfSensor : public cConfModuleAttachedElement
{
	public:
		cConfSensor();
		~cConfSensor();
		virtual void setType(int type);
		virtual int getType();

	private:
		int _iType;
};
