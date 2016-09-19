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
