#include "cConfModule.hpp"

cConfModule::cConfModule()
{
#if VERBOSEDEBUG
	cout << "cConfModule()\n";
#endif
	setType(0);
}

cConfModule::~cConfModule()
{
}

void cConfModule::setType(int type)
{
	this->_iType = type;
}

int cConfModule::getType()
{
	return this->_iType;
}
void cConfModule::setMajor(int major)
{
	this->_iMajor = major;
}

int cConfModule::getMajor()
{
	return this->_iMajor;
}
void cConfModule::setMinor(int minor)
{
	this->_iMinor = minor;
}

int cConfModule::getMinor()
{
	return this->_iMinor;
}
void cConfModule::setBuild(int build)
{
	this->_iBuild = build;
}

int cConfModule::getBuild()
{
	return this->_iBuild;
}

int cConfModule::writeStatus(char* buffer)
{
	return 0;
}
