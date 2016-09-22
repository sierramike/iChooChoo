#include "cConfModuleAttachedElement.hpp"

cConfModuleAttachedElement::cConfModuleAttachedElement()
{
#if VERBOSEDEBUG
	cout << "cConfModuleAttachedElement()\n";
#endif
	setModule(0);
	setIOPort(0);
}

cConfModuleAttachedElement::~cConfModuleAttachedElement()
{
}

void cConfModuleAttachedElement::setModule(int module)
{
	this->_iModule = module;
}

int cConfModuleAttachedElement::getModule()
{
	return this->_iModule;
}

void cConfModuleAttachedElement::setIOPort(int iOPort)
{
	this->_cIOPort = iOPort;
}

int cConfModuleAttachedElement::getIOPort()
{
	return this->_cIOPort;
}
