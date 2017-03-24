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

MultiPrecisionArithmetics::UBigNum::UBigNum(uint32_t x)
{
    val.push_back(x);
}

MultiPrecisionArithmetics::UBigNum::UBigNum(const Bytes & bytes)
{
    int o = bytes.size();
    uint32_t acc = 0;
    for (int i = 0; i < o; ++i) {
        acc += bytes[i] << (8 * (i % 4));
        if (i % 4 == 3 || i == o - 1) {
            val.push_back(acc);
            acc = 0;
        }
    }
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
    *this = *this + y;
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator-=(const UBigNum & y)
{
    *this = *this - y;
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator*=(const UBigNum & y)
{
    (*this) = (*this)*y;
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator/=(const UBigNum & y)
{
    *this = *this / y;
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator%=(const UBigNum & y)
{
    *this = *this%y;
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator>>=(uint32_t y)
{
    *this = *this >> y;
    return *this;
}

UBigNum & MultiPrecisionArithmetics::UBigNum::operator<<=(uint32_t y)
{
    *this = *this << y;
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

const UBigNum MultiPrecisionArithmetics::UBigNum::operator/(const UBigNum & y) const
{
    if (y == 0) throw DivideByZero();
    UBigNum L = 0;
    UBigNum R = *this + 1;
    while (L + 1 < R) {
        auto M = (L + R) >> 1;
        if (M*y <= *this) L = M; else R = M;
    }
    return L;
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator%(const UBigNum & y) const
{
    return *this - (*this / y * y);
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator>>(uint32_t y) const
{
    int bitlen = compactBitLen();
    int old_wordlen = (bitlen + 31) / 32;
    auto target_bitlen = (std::max)(0, bitlen - (int)y);
    auto target_wordlen = (target_bitlen + 31) / 32;
    UBigNum ans;
    ans.val.resize(target_wordlen, 0);
    int delta_w = y / 32;
    int delta_b = y % 32;
    for (int i = 0; i < target_wordlen; ++i) {
        auto word_low = (i + delta_w < old_wordlen) ? val[i + delta_w] : 0;
        auto word_high = (i + delta_w + 1 < old_wordlen) ? val[i + delta_w + 1] : 0;
        uint64_t dd = (uint64_t)word_low | (((uint64_t)word_high) << 32);
        ans.val[i] = dd >> delta_b;
    }
    return ans;
}

const UBigNum MultiPrecisionArithmetics::UBigNum::operator<<(uint32_t y) const
{
    auto old_bitlen = compactBitLen();
    auto target_bitlen = old_bitlen + y;
    auto target_wordlen = (target_bitlen + 31) / 32;
    int delta_w = y / 32;
    int delta_b = y % 32;
    UBigNum ans;
    ans.val.resize(target_wordlen, 0);
    for (int i = 0; i < target_wordlen; ++i) {
        auto word_low = (i - delta_w - 1 >= 0) ? val[i - delta_w - 1] : 0;
        auto word_high = (i - delta_w >= 0) ? val[i - delta_w] : 0;
        uint64_t dd = (uint64_t)word_low | (((uint64_t)word_high) << 32);
        ans.val[i] = (i < delta_w) ? 0 : (dd >> (32 - delta_b));
    }
    return ans;
}

Bytes MultiPrecisionArithmetics::UBigNum::toBytes()
{
    int b = compactBitLen();
    int o = (b + 7) / 8;
    Bytes ans(o);
    for (int i = 0; i < o; ++i) {
        ans[i] = (val[i / 4] >> (8 * (i % 4))) & 0xff;
    }
    return ans;
}


using namespace GroupCurveP256;

const FpNumber GroupCurveP256::FpNumber::operator+(const FpNumber & y) const
{
    return FpNumber();
}

const FpNumber GroupCurveP256::FpNumber::operator-(const FpNumber & y) const
{
    return FpNumber();
}

const FpNumber GroupCurveP256::FpNumber::operator*(const FpNumber & y) const
{
    return FpNumber();
}

FpNumber GroupCurveP256::GroupElement::getAffineX()
{
    return FpNumber();
}

GroupElement & GroupCurveP256::GroupElement::operator=(const GroupElement & Y)
{
    return *this;
}

bool GroupCurveP256::GroupElement::operator==(const GroupElement & Y) const
{
    return false;
}

bool GroupCurveP256::GroupElement::operator!=(const GroupElement & y) const
{
    return false;
}

const GroupElement GroupCurveP256::GroupElement::operator+(const GroupElement & Y) const
{
    return GroupElement();
}

const GroupElement GroupCurveP256::GroupElement::operator-(const GroupElement & Y) const
{
    return GroupElement();
}

const GroupElement GroupCurveP256::operator*(const ZqNumber & a, const GroupElement & X)
{
    return GroupElement();
}

GroupElement GroupCurveP256::simulProduct(const ZqNumber & a, const GroupElement & X, const ZqNumber & b, const GroupElement & Y)
{
    return GroupElement();
}

MultiPrecisionArithmetics::P256FpNumber::P256FpNumber()
{
}

MultiPrecisionArithmetics::P256FpNumber::P256FpNumber(const UBigNum & x)
{
}

P256FpNumber MultiPrecisionArithmetics::P256FpNumber::inverse() const
{
    return P256FpNumber();
}

P256FpNumber MultiPrecisionArithmetics::P256FpNumber::pow(const P256FpNumber & b, const P256FpNumber & e)
{
    return P256FpNumber();
}

const UBigNum P256ZqNumber::q = UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044369");

MultiPrecisionArithmetics::P256ZqNumber::P256ZqNumber()
{
}

MultiPrecisionArithmetics::P256ZqNumber::P256ZqNumber(const UBigNum & x)
{
}

const P256ZqNumber MultiPrecisionArithmetics::P256ZqNumber::operator+(const P256ZqNumber & y) const
{
    return P256ZqNumber();
}

const P256ZqNumber MultiPrecisionArithmetics::P256ZqNumber::operator-(const P256ZqNumber & y) const
{
    return P256ZqNumber();
}

const P256ZqNumber MultiPrecisionArithmetics::P256ZqNumber::operator*(const P256ZqNumber & y) const
{
    return P256ZqNumber();
}

P256ZqNumber MultiPrecisionArithmetics::P256ZqNumber::inverse() const
{
    return P256ZqNumber();
}

P256ZqNumber MultiPrecisionArithmetics::P256ZqNumber::pow(const P256ZqNumber & b, const P256ZqNumber & e)
{
    return P256ZqNumber();
}

P256ZqNumber MultiPrecisionArithmetics::P256ZqNumber::random()
{
    return P256ZqNumber();
}
