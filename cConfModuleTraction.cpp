#include "cConfModuleTraction.hpp"

cConfModuleTraction::cConfModuleTraction()
{
#if VERBOSEDEBUG
	cout << "cConfModuleTraction()\n";
#endif
	setType(BICCP_GRP_TRACTION);
}

cConfModuleTraction::~cConfModuleTraction()
{
}

int cConfModuleTraction::writeStatus(char* buffer)
{
	int iReturn = 0;

	return iReturn;
}
