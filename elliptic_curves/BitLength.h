#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#ifdef _WIN64
#pragma intrinsic(_BitScanReverse64)
#else
#pragma intrinsic(_BitScanReverse)
#endif
#endif

namespace ec_lib
{

	unsigned long GetBitLength(unsigned long long i);

}