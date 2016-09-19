#include "cConfSensor.hpp"

cConfSensor::cConfSensor()
{
#if VERBOSEDEBUG
	cout << "cConfSensor()\n";
#endif
}

cConfSensor::~cConfSensor()
{
}

void cConfSensor::setType(int type)
{
	this->_iType = type;
}

int cConfSensor::getType()
{
	return this->_iType;
}
