// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MULTIPRECISIONARITHMETICS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MULTIPRECISIONARITHMETICS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MULTIPRECISIONARITHMETICS_EXPORTS
#define MULTIPRECISIONARITHMETICS_API __declspec(dllexport)
#else
#define MULTIPRECISIONARITHMETICS_API __declspec(dllimport)
#endif

#include<exception>
#include<vector>
#include<map>

typedef std::vector<uint8_t> Bytes;



class MULTIPRECISIONARITHMETICS_API InvalidArgument : public std::exception {};
class MULTIPRECISIONARITHMETICS_API NegativeDifference : public std::exception {};
class MULTIPRECISIONARITHMETICS_API DivideByZero : public std::exception {};

class MULTIPRECISIONARITHMETICS_API UBigNum {
protected:
public:
    std::vector<uint32_t> val;
    ///<summary>Create a UBigNum whose value is 0.</summary>
    UBigNum();

    ///<summary>Create a UBigNum from a single type data.</summary>
    UBigNum(uint32_t x);

    ///<summary>Create a UBigNum from a little-endian byte array.</summary>
    UBigNum(const Bytes &bytes);

    ///<summary>Create a UbigNum by copying the value from an old one.</summary>
    UBigNum(const UBigNum &x);

    ///<summary>Create a decimal representation without redundant leading 0.</summary>
    std::string toDecString() const;

    ///<summary>Create a hexadecimal representation without redundant leading 0.</summary>
    std::string toHexString() const;

    ///<summary>Create a UBigNum from a hexadecimal representation.</summary>
    ///<remarks>Argument s is considered valid iff s is in {0,...,9,a,...,f}*.</remarks>
    ///<exception cref="MultiPrecisionArithmetics::InvalidArgument">Thrown when s is not a valid hexadecimal representation.</exception>
    static UBigNum fromHexString(const std::string &s);

    ///<summary>Create a UBigNum from a decimal representation.</summary>
    ///<remarks>Argument s is considered valid iff s is in {0,...,9}*.</remarks>
    ///<exception cref="MultiPrecisionArithmetics::InvalidArgument">Thrown when s is not a valid decimal representation.</exception>
    static UBigNum fromDecString(const std::string&);

    ///<summary>Compare two UBigNums x and y.</summary>
    ///<returns>-1 if x&lt;y, or 0 if x=y, or 1 if x&gt;y</returns>
    static int cmp(const UBigNum &x, const UBigNum &y);
        
    UBigNum &operator=(const UBigNum &y);
    bool operator==(const UBigNum &y) const;
    bool operator!=(const UBigNum &y) const;
    bool operator<=(const UBigNum &y) const;
    bool operator<(const UBigNum &y) const;
    bool operator>(const UBigNum &y) const;
    bool operator>=(const UBigNum &y) const;
    UBigNum& operator+=(const UBigNum &y);
    UBigNum& operator-=(const UBigNum &y);
    UBigNum& operator*=(const UBigNum &y);
    UBigNum& operator/=(const UBigNum &y);
    UBigNum& operator%=(const UBigNum &y);
    UBigNum& operator>>=(uint32_t y);
    UBigNum& operator<<=(uint32_t y);
    const UBigNum operator+(const UBigNum &y) const;
    const UBigNum operator-(const UBigNum &y) const;
    const UBigNum operator*(const UBigNum &y) const;
    const UBigNum operator/(const UBigNum &y) const;
    const UBigNum operator%(const UBigNum &y) const;
    const UBigNum operator>>(uint32_t y) const;
    const UBigNum operator<<(uint32_t y) const;

    ///<summary>Get smallest k such that this value is in [0,2^k-1].</summary>
    int compactBitLen() const;

    uint32_t bitat(uint32_t i) const;

    ///<summary>Create a little-endian byte array from current value.</summary>
    ///<remarks>
    ///The returned byte array is compact: no leading 0s will appear.
    ///The returned array will be empty if the current value is 0.
    ///</remarks>
    Bytes toBytes();
};

enum ConstantId {
    Zero,
    One,
    Two,
    Three,
    P256FieldModulus,
    P256GroupOrder,
    P256CurveCoeffB,
    P256GenCoorX,
    P256GenCoorY,
    P521FieldModulus,
    P521GroupOrder,
    P521CurveCoeffB,
    P521GenCoorX,
    P521GenCoorY,
};

extern MULTIPRECISIONARITHMETICS_API std::map<ConstantId, UBigNum> BigConstants;


//template <ConstantId mid>
//class MontgomeryAlgorithms {
//public:
//    static const UBigNum q;//st. p*q = -1 mod R
//    static const UBigNum R;
//    static const ModNumber<mid> Rmoded;
//    static const ModNumber<mid> R2moded;
//    static ModNumber<mid> mont(const ModNumber<mid> &a, const ModNumber<mid> &b) {
//        auto masklen = R.compactBitLen() - 1;
//        auto z = UBigNum(a)*UBigNum(b);
//        auto t1 = z*q;
//        auto t2 = t1.low(masklen);
//        auto t3 = t2*ModNumber<mid>::Modulous;
//        auto t4 = z + t3;
//        auto t5 = t4 >> masklen;
//        if (t5 >= ModNumber<mid>::Modulous) t5 -= ModNumber<mid>::Modulous;
//        return ModNumber<mid>(t5);
//    }
//    static ModNumber<mid> mul(const ModNumber<mid> &a, const ModNumber<mid> &b) {
//        auto ia = mont(a, R2moded);
//        auto ib = mont(b, R2moded);
//        auto ic = mont(ia, ib);
//        auto id = mont(ic, 1);
//        return ModNumber<mid>(id);
//    }
//
//    static ModNumber<mid> exp(const ModNumber<mid> &b, const UBigNum &e) {
//        ModNumber<mid> acc(Rmoded);
//        ModNumber<mid> ib = mont(b, R2moded);
//        for (int i = e.compactBitLen() - 1; i >= 0; --i) {
//            acc = mont(acc, acc);
//            if (e.bitat(i) == 1) {
//                acc = mont(acc, ib);
//            }
//        }
//        return mont(acc, 1);
//    }
//
//    static ModNumber<mid> inv(const ModNumber<mid> &b) {
//        return exp(b, ModNumber<mid>::Modulous - 2);
//    }
//};


class MULTIPRECISIONARITHMETICS_API NaiveModMuler {
public:
    static UBigNum calc(const UBigNum &a, const UBigNum &b, const UBigNum &m);
};

class MULTIPRECISIONARITHMETICS_API NaiveModInver {
public:
    static std::pair<bool,UBigNum> calc(const UBigNum &b, const UBigNum &m);
};

template <ConstantId mid, class ModMuler, class ModInver>
class ModNumber : public UBigNum {
public:
    static const UBigNum Modulus;
    static const int BitLen;
    static const int ByteLen;
    ModNumber() {
        val.resize(ByteLen);
    }
    ModNumber(const UBigNum &x) {
        val.resize(ByteLen);
        auto r = x%Modulus;
        val.assign(r.val.begin(), r.val.end());
    }
    static ModNumber fromBytes(const Bytes &bytes) {
        return ModNumber(UBigNum(bytes));
    }
    Bytes toBytes() const {
        Bytes ret(ByteLen);
        for (int i = 0; i < ByteLen; ++i) {
            auto j = ByteLen - 1 - i;
            auto w = val[j / 4];
            ret[i] = w >> (8 * (j % 4));
        }
        return ret;
    }
    const ModNumber operator+(const ModNumber &y) const {
        return (UBigNum(*this) + UBigNum(y)) % Modulus;
    }
    const ModNumber operator-(const ModNumber &y) const {
        return (UBigNum(*this) - UBigNum(y)) % Modulus;
    }
    const ModNumber operator*(const ModNumber &y) const {
        return ModNumber(ModMuler::calc(UBigNum(*this), UBigNum(y), Modulus));
    }
    uint32_t bitat(uint32_t i) const {
        if (i >= BitLen) return 0;
        auto w = val[i / 32];
        auto t = w >> (i % 32);
        return t&1;
    }
    static ModNumber inv(const ModNumber &x) {
        return pow(x, Modulus - 2);
    }
    static ModNumber sqr(const ModNumber &x) {
        return pow(x, 2);
    }
    static ModNumber pow(const ModNumber &b, const UBigNum &e) {
        int n = e.compactBitLen();
        ModNumber ret;
        for (int i = n - 1; i >= 0; --i) {
            ret = ModNumber::sqr(ret);
            if (e.bitat(i)) ret = ret*b;
        }
        return ret;
    }
    static ModNumber random() {
        ModNumber ret;
        for (int i = ByteLen - 1; i >= 0; --i) {
            ret.val[i] = 0;
        }
        return ret;
    }
};
template <ConstantId mid, class ModMuler, class ModInver> const UBigNum ModNumber<mid,ModMuler,ModInver>::Modulus = BigConstants[mid];
template <ConstantId mid, class ModMuler, class ModInver> const int ModNumber<mid,ModMuler,ModInver>::BitLen = Modulus.compactBitLen();
template <ConstantId mid, class ModMuler, class ModInver> const int ModNumber<mid,ModMuler,ModInver>::ByteLen = (ModNumber<mid,ModMuler,ModInver>::BitLen + 7) / 8;

typedef ModNumber<P256FieldModulus, NaiveModMuler, NaiveModInver> P256FpNumber;
typedef ModNumber<P256GroupOrder, NaiveModMuler, NaiveModInver> P256ZqNumber;
typedef ModNumber<P521FieldModulus, NaiveModMuler, NaiveModInver> P521FpNumber;
typedef ModNumber<P521GroupOrder, NaiveModMuler, NaiveModInver> P521ZqNumber;


template<class T1, class T2, ConstantId aid, ConstantId bid, ConstantId gxcid, ConstantId gycid>
class FpCurve {
public:
    typedef T1 FpNumber;
    typedef T2 ZqNumber;
    static const FpNumber a;
    static const FpNumber b;
    bool isOnCurve(const FpNumber &x, const FpNumber &y) {
        return false;
    }
    class GroupElement
    {
    private:
        bool is_inf;
        FpNumber x;
        FpNumber y;
    public:
        GroupElement() {
            is_inf = true;
        }
        GroupElement(const FpNumber &x, const FpNumber &y) {
            is_inf = false;
            this->x = x;
            this->y = y;
        }
        GroupElement(const GroupElement &P) {
            is_inf = P.is_inf;
            x = P.x;
            y = P.y;
        }
        bool isInfinite() const {
            return is_inf;
        }
        Bytes toBytes() const {
            Bytes ret;
            uint8_t typ_oct = (is_inf) ? 0 : 1;
            auto xocts = x.toBytes();
            auto yocts = y.toBytes();
            ret.push_back(typ_oct);
            ret.insert(ret.end(), xocts.begin(), xocts.end());
            ret.insert(ret.end(), yocts.begin(), yocts.end());
            return ret;
        }
        FpNumber getAffineX() {
            return x;
        }
        FpNumber getAffineY() {
            return y;
        }
        GroupElement &operator=(const GroupElement &Y) {
            this->is_inf = Y.is_inf;
            this->x = Y.x;
            this->y = Y.y;
            return *this;
        }
        bool operator==(const GroupElement &Y) const {
            bool xinf = this->isInfinite();
            bool yinf = Y.isInfinite();
            if (xinf && yinf) return true;
            if (xinf^yinf) return false;
            return (this->x == Y->x&&this->y == Y.y);
        }
        bool operator!=(const GroupElement &Y) const {
            return !(*this == Y);
        }
        const GroupElement dbl() const {
            if (this->isInfinite()) return GroupElement();
            FpNumber q = (FpNumber(3) * FpNumber::sqr(x) + a)*(FpNumber::inv(FpNumber(2) * y));
            FpNumber x3 = FpNumber::sqr(q) - FpNumber(2) * this->x;
            FpNumber y3 = q*(this->x - x3) - this->y;
            return GroupElement(x3, y3);
        }
        const GroupElement operator+(const GroupElement &Y) const {
            if (this->isInfinite()) return Y;
            if (Y.isInfinite()) return *this;
            FpNumber dx = Y.x - this->x;
            FpNumber dy = Y.y - this->y;
            FpNumber dq = dy*FpNumber::inv(dx);
            FpNumber x3 = FpNumber::sqr(dq) - this->x - Y.x;
            FpNumber y3 = dq*(this->x - x3) - this->y;
            return GroupElement(x3, y3);
        }
        const GroupElement operator+=(const GroupElement &Y) {
            *this = *this + Y;
            return *this;
        }
        const GroupElement operator-() const {
            if (this->isInfinite()) return GroupElement();
            return GroupElement(this->x, -this->y);
        }
        const GroupElement operator-(const GroupElement &Y) const {
            return *this+(-Y);
        }
        const GroupElement operator-=(const GroupElement &Y) {
            *this = *this - Y;
            return *this;
        }
        static GroupElement productSimul(const ZqNumber &a, const GroupElement &X, const ZqNumber &b, const GroupElement &Y) {
            GroupElement ret;
            for (int i = ZqNumber::BitLen - 1; i >= 0; --i) {
                ret = ret.dbl();
                if (a.bitat(i)) ret += X;
                if (b.bitat(i)) ret += Y;
            }
            return ret;
        }
        static bool isValidEncoding(const Bytes &bytes) {
            int n = bytes.size();
            if (n != 1 + 2 * FpNumber::ByteLen) return false;

            return false;
        }
        ///<returns>(0,value) if convert ok, or (-1,*) if decode error, or (-2,*) if point not in curve.</returns>
        static std::tuple<int,GroupElement> fromBytes(const Bytes &bytes) {
            int n = bytes.size();
            GroupElement ret;
            if (n != 1 + 2 * FpNumber::ByteLen) return std::make_tuple(-1, ret);
            if (bytes[0]) {
                auto tx = FpNumber::fromBytes(Bytes(bytes.begin()+1,bytes.begin()+FpNumber::ByteLen+1));
                auto ty = FpNumber::fromBytes(Bytes(bytes.begin()+FpNumber::ByteLen+1, bytes.begin()+2*FpNumber::ByteLen+1));
                if (FpNumber::sqr(ty) != FpNumber::pow(tx,3) + a*tx + b) return std::make_tuple(-2, ret);
            }
            return std::make_tuple(0, ret);
        }
    };
    static const GroupElement generator;
    friend const GroupElement operator*(const ZqNumber &a, const GroupElement &X) {
        GroupElement ret;
        for (int i = ZqNumber::BitLen - 1; i >= 0; -i) {
            ret = ret.dbl();
            if (a.bitat(i)) ret += X;
        }
        return ret;
    }
};

template<class T1, class T2, ConstantId aid, ConstantId bid, ConstantId gxcid, ConstantId gycid>
const typename FpCurve<T1, T2, aid, bid, gxcid, gycid>::GroupElement FpCurve<T1, T2, aid, bid, gxcid, gycid>::generator = FpCurve<T1, T2, aid, bid, gxcid, gycid>::GroupElement(FpCurve<T1, T2, aid, bid, gxcid, gycid>::a, FpCurve<T1, T2, aid, bid, gxcid, gycid>::b);

template<class T1, class T2, ConstantId  aid, ConstantId bid, ConstantId gxcid, ConstantId gycid>
const typename FpCurve<T1, T2, aid, bid, gxcid, gycid>::FpNumber FpCurve<T1, T2, aid, bid, gxcid, gycid>::a = BigConstants[aid];

template<class T1, class T2, ConstantId aid, ConstantId bid, ConstantId gxcid, ConstantId gycid>
const typename FpCurve<T1, T2, aid, bid, gxcid, gycid>::FpNumber FpCurve<T1, T2, aid, bid, gxcid, gycid>::b = BigConstants[bid];


typedef FpCurve<P256FpNumber, P256ZqNumber, Three, P256CurveCoeffB, P256GenCoorX, P256GenCoorY> CurveP256Group;
typedef FpCurve<P521FpNumber, P521ZqNumber, Three, P521CurveCoeffB, P521GenCoorX, P521GenCoorY> CurveP521Group;
