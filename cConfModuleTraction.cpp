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
