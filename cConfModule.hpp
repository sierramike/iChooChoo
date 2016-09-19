#pragma once

#include "cConfElement.hpp"
#include <string.h>

class cConfModule : public cConfElement
{
	public:
		cConfModule();
		~cConfModule();
		virtual void setType(int type);
		virtual int getType();

	private:
		int _iType;
};
