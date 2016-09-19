#pragma once

#include "cConfElement.hpp"
#include "cConfModule.hpp"
#include <string.h>

class cConfModuleAttachedElement : public cConfElement
{
	public:
		cConfModuleAttachedElement();
		~cConfModuleAttachedElement();
		virtual void setModule(cConfModule* module);
		virtual cConfModule* getModule();
		virtual void setIOPort(int iOPort);
		virtual int getIOPort();

	private:
		cConfModule* _module;
		int _cIOPort;
};
