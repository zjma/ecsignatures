// MultiPrecisionArithmetics.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MultiPrecisionArithmetics.h"

using namespace MultiPrecisionArithmetics;


UBigNum::UBigNum(void)
{
}

MultiPrecisionArithmetics::UBigNum::UBigNum(const UBigNum & x)
{
}

int MultiPrecisionArithmetics::UBigNum::compactBitLen(void)
{
    return 0;
}

std::string UBigNum::toDecString()
{
    return "0";
}

std::string UBigNum::toHexString()
{
    return "0";
}

UBigNum UBigNum::fromHexString(const std::string&)
{
    return UBigNum();
}

UBigNum UBigNum::fromDecString(const std::string&)
{
    return UBigNum();
}

int UBigNum::cmp(const UBigNum &, const UBigNum &)
{
    return 0;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator=(const UBigNum & y)
{
    return *this;
}

bool MultiPrecisionArithmetics::UBigNum::operator==(const UBigNum & y) const
{
    return false;
}

bool MultiPrecisionArithmetics::UBigNum::operator!=(const UBigNum & y) const
{
    return false;
}

bool MultiPrecisionArithmetics::UBigNum::operator<=(const UBigNum & y) const
{
    return false;
}

bool MultiPrecisionArithmetics::UBigNum::operator<(const UBigNum & y) const
{
    return false;
}

bool MultiPrecisionArithmetics::UBigNum::operator>(const UBigNum & y) const
{
    return false;
}

bool MultiPrecisionArithmetics::UBigNum::operator>=(const UBigNum & y) const
{
    return false;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator+=(const UBigNum & y)
{
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator-=(const UBigNum & y)
{
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator*=(const UBigNum & y)
{
    return *this;
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator+(const UBigNum & y) const
{
    return UBigNum();
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator-(const UBigNum & y) const
{
    return UBigNum();
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator*(const UBigNum & y) const
{
    return UBigNum();
}
