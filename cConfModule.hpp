#pragma once

#include "cConfElement.hpp"
#include "biccp.h"
#include <string.h>

class cConfModule : public cConfElement
{
	public:
		cConfModule();
		~cConfModule();
		virtual int writeStatus(char* buffer);
		virtual void setType(int type);
		virtual int getType();

	private:
		int _iType;
};
