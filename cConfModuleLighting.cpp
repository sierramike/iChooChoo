#include "cConfModuleLighting.hpp"

cConfModuleLighting::cConfModuleLighting()
{
#if VERBOSEDEBUG
	cout << "cConfModuleLighting()\n";
#endif
	setType(BICCP_GRP_LIGHTING);
}

cConfModuleLighting::~cConfModuleLighting()
{
}

int cConfModuleLighting::writeStatus(char* buffer)
{
	int iReturn = 0;

	for (int i = 0; i < 10; i++)
	{
		iReturn += sprintf(buffer + iReturn, " %d", _bOutputs[i] ? 1 : 0);
	}

	for (int i = 0; i < 6; i++)
	{
		iReturn += sprintf(buffer + iReturn, " %d", _bDimmableOutputs[i]);
	}

	return iReturn;
}

bool cConfModuleLighting::setOutput(int position, bool value)
{
	if (position >= 0 && position < 10)
	{
		_bOutputs[position] = value;
		return true;
	}
	else if (position >= 10 && position < 16)
	{
		_bDimmableOutputs[position - 10] = (value ? 255 : 0);
		return true;
	}
	else
		return false;
}

bool cConfModuleLighting::getOutput(int position)
{
	if (position >= 0 && position < 10)
		return _bOutputs[position];
	else if (position >= 10 && position < 16)
		return (_bDimmableOutputs[position - 10] > 0);
	else
		return false;
}

bool cConfModuleLighting::setDimmableOutput(int position, uint8_t value)
{
	if (position >= 0 && position < 10)
	{
		return false;
	}
	else if (position >= 10 && position < 16)
	{
		_bDimmableOutputs[position - 10] = value;
		return true;
	}
	else
		return false;
}

uint8_t cConfModuleLighting::getDimmableOutput(int position)
{
	if (position >= 0 && position < 10)
		return 0;
	else if (position >= 10 && position < 16)
		return _bDimmableOutputs[position - 10];
	else
		return 0;
}
