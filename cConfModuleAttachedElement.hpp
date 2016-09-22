#pragma once

#include "cConfElement.hpp"
#include "cConfModule.hpp"
#include <string.h>

class cConfModuleAttachedElement : public cConfElement
{
	public:
		cConfModuleAttachedElement();
		~cConfModuleAttachedElement();
		virtual void setModule(int module);
		virtual int getModule();
		virtual void setIOPort(int iOPort);
		virtual int getIOPort();

	private:
		int _iModule;
		int _cIOPort;
};
