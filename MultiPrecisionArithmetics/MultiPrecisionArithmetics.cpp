// MultiPrecisionArithmetics.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MultiPrecisionArithmetics.h"


// This is an example of an exported variable
MULTIPRECISIONARITHMETICS_API int nMultiPrecisionArithmetics=0;

// This is an example of an exported function.
MULTIPRECISIONARITHMETICS_API int fnMultiPrecisionArithmetics(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see MultiPrecisionArithmetics.h for the class definition
CMultiPrecisionArithmetics::CMultiPrecisionArithmetics()
{
    return;
}
