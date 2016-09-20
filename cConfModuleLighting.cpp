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

void cConfModuleLighting::setOutput(int position, bool value)
{
	if (position >= 0 && position < 10)
		_bOutputs[position] = value;
}

bool cConfModuleLighting::getOutput(int position)
{
	if (position >= 0 && position < 10)
		return _bOutputs[position];
	else
		return false;
}
