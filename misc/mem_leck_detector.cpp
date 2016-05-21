#include "mem_leck_detector.h"
#ifdef _WIN32
#include <crtdbg.h>
#endif

mem_leck_detector::mem_leck_detector(unsigned int n)
{
	if(n>0)
	{
		#ifdef _WIN32
		_CrtSetBreakAlloc(n);
		#endif
	}
}

mem_leck_detector::~mem_leck_detector()
{
	#ifdef _WIN32
	_CrtDumpMemoryLeaks();
	#endif
}
