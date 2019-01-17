#include "BitLength.h"

namespace ec_lib
{

	unsigned long GetBitLength(unsigned long long i)
	{
#ifdef _MSC_VER
#ifdef _WIN64
		unsigned long result;
		if (_BitScanReverse64(&result, i))
		{
			return result + 1;
		}
		else
		{
			return 0;
		}
#else
		unsigned long result;
		if (_BitScanReverse(&result, i))
		{
			return result + 1;
		}
		else
		{
			return 0;
		}
#endif
#else
		// TODO! Use __builtin_clz for better performance
		unsigned long result = 0;
		while (i != 0)
		{
			i >>= 1;
			result++;
		}
		return result;
#endif
	}

}