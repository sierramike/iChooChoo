#pragma once

#include "cConfElement.hpp"
#include "biccp.h"
#include <string.h>

class cConfModule : public cConfElement
{
	public:
		cConfModule();
		virtual ~cConfModule();
		virtual void setType(int type);
		virtual int getType();
		virtual void setMajor(int major);
		virtual int getMajor();
		virtual void setMinor(int minor);
		virtual int getMinor();
		virtual void setBuild(int build);
		virtual int getBuild();

		virtual int writeStatus(char* buffer);

	private:
		int _iType;
		int _iMajor;
		int _iMinor;
		int _iBuild;
};
