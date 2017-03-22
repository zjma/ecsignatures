// MultiPrecisionArithmetics.cpp : Defines the exported functions for the DLL application.
//

#define NOMINMAX
#include "stdafx.h"
#include<vector>
#include<algorithm>
#include "MultiPrecisionArithmetics.h"

using namespace MultiPrecisionArithmetics;


UBigNum::UBigNum(void)
{
}

MultiPrecisionArithmetics::UBigNum::UBigNum(const UBigNum & x)
{
    val.assign(x.val.begin(), x.val.end());
}

int MultiPrecisionArithmetics::UBigNum::compactBitLen(void) const
{
    int vn = val.size();
    int w = vn;
    for (int i = vn - 1; i >= 0 && val[i] == 0; --i, --w);
    if (w == 0) return 0;
    int b = 0;
    for (auto x = val[w - 1]; x > 0; x /= 2, ++b);
    return (w-1)*32+b;
}

const uint64_t DM = 1000000000;
char *D2H = "0123456789ABCDEF";

std::string uint32_to_dec(uint32_t x, bool compact)
{
    std::string ans;
    for (int i = 0; i < 9 && (!compact || x > 0); ++i) {
        ans.push_back(D2H[x%10]);
        x /= 10;
    }
    if (ans.empty()) ans.push_back('0');
    std::reverse(ans.begin(), ans.end());
    return ans;
}

void mul_dval_by_two(std::vector<uint32_t> &dval)
{
    int dn = dval.size();
    uint32_t carry = 0;
    for (int i = 0; i < dn; ++i) {
        dval[i] = dval[i] * 2 + carry;
        carry = dval[i] / DM;
        dval[i] %= DM;
    }
    if (carry > 0) dval.push_back(carry);
}

void inc_dval_by_one(std::vector<uint32_t> &dval)
{
    int dn = dval.size();
    uint32_t carry = 1;
    for (int i = 0; i < dn && carry > 0; ++i)
    {
        dval[i] += carry;
        carry = dval[i] / DM;
        dval[i] %= DM;
    }
    if (carry > 0) dval.push_back(carry);
}

std::string UBigNum::toDecString() const
{
    int b = compactBitLen();
    if (b == 0) return "0";
    std::vector<uint32_t> dval;
    int wi = (b - 1) / 32;
    for (int i = wi; i >= 0; --i) {
        for (int j = 0; j < 32; ++j) {
            mul_dval_by_two(dval);
            if ((val[i] & (1 << (31 - j))) != 0) {
                inc_dval_by_one(dval);
            }
        }
    }
    int dn = dval.size();
    std::string ans=uint32_to_dec(dval[dn-1], true);

    for (int i = dn-2; i >= 0; --i) {
        ans += uint32_to_dec(dval[i], false);
    }
    return ans;
}

std::string uint32_to_hex(uint32_t x, bool compact)
{
    std::string ans;
    bool nonzerofound = false;
    for (int i = 0; i < 8; ++i) {
        auto c = D2H[(x>>(28-i*4))&15];
        nonzerofound = nonzerofound || (c != '0');
        if (!compact||nonzerofound) ans.push_back(c);
    }
    if (ans.empty()) ans.push_back('0');
    return ans;
}

std::string UBigNum::toHexString() const
{
    int b = compactBitLen();
    if (b == 0) return "0";
    int wi = (b-1)/32;
    std::string ans=uint32_to_hex(val[wi], true);
    for (int i = wi-1; i >= 0; --i) {
        ans += uint32_to_hex(val[i], false);
    }
    return ans;
}

UBigNum UBigNum::fromHexString(const std::string& s)
{
    UBigNum ans;
    for (auto c : s) {
        uint32_t d;
        if (c >= '0'&&c <= '9')
            d = c - 48;
        else if (c >= 'a'&&c <= 'f')
            d = c - 87;
        else if (c >= 'A'&&c <= 'F')
            d = c - 55;
        else
            throw InvalidArgument();
        ans *= 16;
        ans += d;
    }
    return ans;
}

UBigNum UBigNum::fromDecString(const std::string& s)
{
    UBigNum ans;
    for (auto c : s) {
        if (c<'0' || c>'9') throw InvalidArgument();
        uint32_t d = c - 48;
        ans *= 10;
        ans += d;
    }
    return ans;
}

int UBigNum::cmp(const UBigNum &a, const UBigNum &b)
{
    int awn = (a.compactBitLen() + 31) / 32;
    int bwn = (b.compactBitLen() + 31) / 32;
    for (int i = ((awn > bwn) ? awn : bwn) - 1; i >= 0; --i) {
        auto aw = (i < awn) ? a.val[i] : 0;
        auto bw = (i < bwn) ? b.val[i] : 0;
        if (aw > bw) return 1;
        if (aw < bw) return -1;
    }
    return 0;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator=(const UBigNum & y)
{
    val.assign(y.val.begin(), y.val.end());
    return *this;
}

bool MultiPrecisionArithmetics::UBigNum::operator==(const UBigNum & y) const
{
    return cmp(*this, y) == 0;
}

bool MultiPrecisionArithmetics::UBigNum::operator!=(const UBigNum & y) const
{
    return cmp(*this, y) != 0;
}

bool MultiPrecisionArithmetics::UBigNum::operator<=(const UBigNum & y) const
{
    return cmp(*this, y) <= 0;
}

bool MultiPrecisionArithmetics::UBigNum::operator<(const UBigNum & y) const
{
    return cmp(*this, y) < 0;
}

bool MultiPrecisionArithmetics::UBigNum::operator>(const UBigNum & y) const
{
    return cmp(*this, y) > 0;
}

bool MultiPrecisionArithmetics::UBigNum::operator>=(const UBigNum & y) const
{
    return cmp(*this, y) >= 0;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator+=(const UBigNum & y)
{
    int vn = val.size();
    int xwn = (compactBitLen() + 31) / 32;
    int ywn = (y.compactBitLen() + 31) / 32;
    int wn = (xwn > ywn) ? xwn : ywn;
    val.resize(((vn > wn) ? vn : wn) + 1, 0);
    uint64_t carry = 0;
    for (int i = 0; i < wn; ++i) {
        uint64_t yw = (i < ywn) ? y.val[i] : 0;
        auto t = (uint64_t)val[i] + yw + carry;
        val[i] = t & 0xffffffff;
        carry = (t >> 32) & 0xffffffff;
    }
    if (carry > 0) val[wn] = carry;
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator-=(const UBigNum & y)
{
    if (*this < y) throw NegativeDifference();
    int xwn = (compactBitLen() + 31) / 32;
    int ywn = (y.compactBitLen() + 31) / 32;
    int wn = (xwn > ywn) ? xwn : ywn;
    uint64_t borrowed = 0;
    for (int i = 0; i < wn; ++i) {
        uint64_t xw = val[i];
        uint64_t yw = (i < ywn) ? y.val[i] : 0;
        auto t = xw + 0x100000000 - yw - borrowed;
        val[i] = t & 0xffffffff;
        borrowed = 1 - ((t >> 32) & 0xffffffff);
    }
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator*=(const UBigNum & y)
{
    auto z = (*this)*y;
    (*this) = z;
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator+=(const uint32_t & y)
{
    int vn = val.size();
    uint64_t carry = y;
    for (int i = 0; i < vn&&carry>0; ++i) {
        carry += val[i];
        val[i] = carry & 0xffffffff;
        carry = carry >> 32;
    }
    if (carry > 0) val.push_back(carry);
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator-=(const uint32_t & y)
{
    int bn = compactBitLen();
    uint64_t w0 = (bn == 0) ? 0 : val[0];
    if (bn <= 32 && w0 < y) throw NegativeDifference();
    int xwn = (bn + 31) / 32;
    val[0] = (w0 + 0x100000000 - y) & 0xffffffff;
    uint64_t borrowed = (w0 < y) ? 1 : 0;
    for (int i = 1; i < xwn && borrowed>0; ++i) {
        uint64_t xw = val[i];
        auto t = xw + 0x100000000 - borrowed;
        val[i] = t & 0xffffffff;
        borrowed = 1 - ((t >> 32) & 0xffffffff);
    }
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator*=(const uint32_t & y)
{
    int vn = val.size();
    uint64_t carry = 0;
    for (int i = 0; i < vn; ++i) {
        uint64_t t = (uint64_t)val[i] * y + carry;
        val[i] = t & 0xffffffff;
        carry = t >> 32;
    }
    if (carry > 0) val.push_back(carry);
    return *this;
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator+(const UBigNum & y) const
{
    int vn = val.size();
    int xwn = (compactBitLen() + 31) / 32;
    int ywn = (y.compactBitLen() + 31) / 32;
    int wn = (xwn > ywn) ? xwn : ywn;
    UBigNum ans;
    ans.val.resize(wn + 1, 0);
    uint64_t carry = 0;
    for (int i = 0; i < wn; ++i) {
        uint64_t xw = (i < xwn) ? val[i] : 0;
        uint64_t yw = (i < ywn) ? y.val[i] : 0;
        auto t = xw + yw + carry;
        ans.val[i] = t & 0xffffffff;
        carry = (t >> 32) & 0xffffffff;
    }
    if (carry > 0) ans.val[wn] = carry;
    return ans;
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator-(const UBigNum & y) const
{
    if (*this < y) throw NegativeDifference();
    int xwn = (compactBitLen() + 31) / 32;
    int ywn = (y.compactBitLen() + 31) / 32;
    int wn = (xwn > ywn) ? xwn : ywn;
    UBigNum ans;
    ans.val.resize(wn, 0);
    uint64_t borrowed = 0;
    for (int i = 0; i < wn; ++i) {
        uint64_t xw = (i < xwn) ? val[i] : 0;
        uint64_t yw = (i < ywn) ? y.val[i] : 0;
        auto t = xw + 0x100000000 - yw - borrowed;
        ans.val[i] = t & 0xffffffff;
        borrowed = 1 - ((t >> 32) & 0xffffffff);
    }
    return ans;
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator*(const UBigNum & y) const
{
    UBigNum ans;
    int xwn = (compactBitLen() + 31) / 32;
    int ywn = (y.compactBitLen() + 31) / 32;
    ans.val.resize(xwn + ywn, 0);
    uint64_t c0 = 0;
    uint64_t c1 = 0;
    uint64_t c2 = 0;
    for (int k = 0; k < xwn + ywn - 1; ++k) {
        int i0 = (std::max)(0, k - ywn + 1);
        int i1 = (std::min)(xwn - 1, k);
        for (int i = i0; i <= i1; ++i) {
            int j = k - i;
            uint64_t t = (uint64_t)val[i] * (uint64_t)y.val[j];
            c0 += t & 0xffffffff;
            c1 += (t >> 32) + (c0 >> 32);
            c2 += c1 >> 32;
            c1 &= 0xffffffff;
            c0 &= 0xffffffff;
        }
        ans.val[k] = c0;
        c0 = c1;
        c1 = c2;
        c2 = 0;
    }
    ans.val[xwn + ywn - 1] = c0;
    return ans;
}
