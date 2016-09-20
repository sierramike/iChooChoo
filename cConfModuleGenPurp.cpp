#include "cConfModuleGenPurp.hpp"

cConfModuleGenPurp::cConfModuleGenPurp()
{
#if VERBOSEDEBUG
	cout << "cConfModuleGenPurp()\n";
#endif
	setType(BICCP_GRP_GENPURP);
}

cConfModuleGenPurp::~cConfModuleGenPurp()
{
}

void cConfModuleGenPurp::setOutput(int position, bool value)
{
	if (position >= 0 && position < 16)
		_bOutputs[position] = value;
}

bool cConfModuleGenPurp::getOutput(int position)
{
	if (position >= 0 && position < 16)
		return _bOutputs[position];
	else
		return false;
}

int cConfModuleGenPurp::writeStatus(char* buffer)
{
	int iReturn = 0;

	for (int i = 0; i < 16; i++)
	{
		iReturn += sprintf(buffer + iReturn, " %d", _bOutputs[i] ? 1 : 0);
	}
	
	return iReturn;
}
