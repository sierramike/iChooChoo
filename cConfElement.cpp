#include "cConfElement.hpp"

cConfElement::cConfElement()
{
#if VERBOSEDEBUG
	cout << "cConfElement();\n";
#endif
	setID(0);
	_sDescription = 0;
	setDescription("");
}

cConfElement::~cConfElement()
{
	delete[] this->_sDescription;
}

void cConfElement::setID(int iD)
{
	this->_iID = iD;
}

int cConfElement::getID()
{
	return this->_iID;
}

void cConfElement::setDescription(const char* description)
{
	if (!_sDescription)
	{
		delete[] this->_sDescription;
	}
	int i = strlen(description);
	this->_sDescription = new char[i + 1];
	memcpy(this->_sDescription, description, i + 1);
}

char* cConfElement::getDescription()
{
	return this->_sDescription;
}
