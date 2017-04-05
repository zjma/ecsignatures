// MultiPrecisionArithmetics.cpp : Defines the exported functions for the DLL application.
//

#define NOMINMAX
#include "stdafx.h"
#include<vector>
#include<algorithm>
#include "MultiPrecisionArithmetics.h"



UBigNum::UBigNum(void)
{
}

UBigNum::UBigNum(uint32_t x)
{
    val.push_back(x);
}

UBigNum::UBigNum(const Bytes & bytes)
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

UBigNum::UBigNum(const UBigNum & x)
{
    val.assign(x.val.begin(), x.val.end());
}

int UBigNum::compactBitLen(void) const
{
    int vn = val.size();
    int w = vn;
    for (int i = vn - 1; i >= 0 && val[i] == 0; --i, --w);
    if (w == 0) return 0;
    int b = 0;
    for (auto x = val[w - 1]; x > 0; x /= 2, ++b);
    return (w-1)*32+b;
}

uint32_t UBigNum::bitat(uint32_t i) const
{
    return 0;
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

UBigNum & UBigNum::operator=(const UBigNum & y)
{
    val.assign(y.val.begin(), y.val.end());
    return *this;
}

bool UBigNum::operator==(const UBigNum & y) const
{
    return cmp(*this, y) == 0;
}

bool UBigNum::operator!=(const UBigNum & y) const
{
    return cmp(*this, y) != 0;
}

bool UBigNum::operator<=(const UBigNum & y) const
{
    return cmp(*this, y) <= 0;
}

bool UBigNum::operator<(const UBigNum & y) const
{
    return cmp(*this, y) < 0;
}

bool UBigNum::operator>(const UBigNum & y) const
{
    return cmp(*this, y) > 0;
}

bool UBigNum::operator>=(const UBigNum & y) const
{
    return cmp(*this, y) >= 0;
}

UBigNum & UBigNum::operator+=(const UBigNum & y)
{
    *this = *this + y;
    return *this;
}

UBigNum & UBigNum::operator-=(const UBigNum & y)
{
    *this = *this - y;
    return *this;
}

UBigNum & UBigNum::operator*=(const UBigNum & y)
{
    (*this) = (*this)*y;
    return *this;
}

UBigNum & UBigNum::operator/=(const UBigNum & y)
{
    *this = *this / y;
    return *this;
}

UBigNum & UBigNum::operator%=(const UBigNum & y)
{
    *this = *this%y;
    return *this;
}

UBigNum & UBigNum::operator>>=(uint32_t y)
{
    *this = *this >> y;
    return *this;
}

UBigNum & UBigNum::operator<<=(uint32_t y)
{
    *this = *this << y;
    return *this;
}

const UBigNum UBigNum::operator+(const UBigNum & y) const
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

const UBigNum UBigNum::operator-(const UBigNum & y) const
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

const UBigNum UBigNum::operator*(const UBigNum & y) const
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

const UBigNum UBigNum::operator/(const UBigNum & y) const
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

const UBigNum UBigNum::operator%(const UBigNum & y) const
{
    return *this - (*this / y * y);
}

const UBigNum UBigNum::operator>>(uint32_t y) const
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

const UBigNum UBigNum::operator<<(uint32_t y) const
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

Bytes UBigNum::toBytes()
{
    int b = compactBitLen();
    int o = (b + 7) / 8;
    Bytes ans(o);
    for (int i = 0; i < o; ++i) {
        ans[i] = (val[i / 4] >> (8 * (i % 4))) & 0xff;
    }
    return ans;
}


MULTIPRECISIONARITHMETICS_API std::map<ConstantId, UBigNum> BigConstants = {
    {Zero, UBigNum()},
    {One,UBigNum(1)},
    {Two,UBigNum(2)},
    {Three,UBigNum(3)},
    { P256FieldModulus, UBigNum::fromDecString("115792089210356248762697446949407573530086143415290314195533631308867097853951") },
    { P256GroupOrder, UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044369") },
    { P256CurveCoeffB, UBigNum::fromHexString("5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b") },
    { P256GenCoorX, UBigNum::fromHexString("6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296") },
    { P256GenCoorY, UBigNum::fromHexString("4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5") },
    { P521FieldModulus, UBigNum::fromDecString("6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151") },
    { P521GroupOrder, UBigNum::fromDecString("6864797660130609714981900799081393217269435300143305409394463459185543183397655394245057746333217197532963996371363321113864768612440380340372808892707005449") },
    { P521CurveCoeffB, UBigNum::fromHexString("051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00") },
    { P521GenCoorX, UBigNum::fromHexString("c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66") },
    { P521GenCoorY, UBigNum::fromHexString("11839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650") },
};

UBigNum NaiveModMuler::calc(const UBigNum & a, const UBigNum & b, const UBigNum & m)
{
    return a*b%m;
}


std::pair<bool,UBigNum> NaiveModInver::calc(const UBigNum & b, const UBigNum & m)
{
    return std::make_pair(false,UBigNum());
}
