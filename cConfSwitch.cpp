#include "cConfSwitch.hpp"

cConfSwitch::cConfSwitch()
{
#if VERBOSEDEBUG
	cout << "cConfSwitch()\n";
#endif
	setStraightValue(0);
}

cConfSwitch::~cConfSwitch()
{
}

void cConfSwitch::setStraightValue(int straightValue)
{
	this->_iStraightValue = straightValue;
}

int cConfSwitch::getStraightValue()
{
	return this->_iStraightValue;
}
