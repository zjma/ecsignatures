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

    ///<summary>Create a little-endian byte array from current value.</summary>
    ///<remarks>
    ///The returned byte array is compact: no leading 0s will appear.
    ///The returned array will be empty if the current value is 0.
    ///</remarks>
    Bytes toBytes();
};

extern MULTIPRECISIONARITHMETICS_API UBigNum ConstModulus[2];


template <int mid>
class ModNumber : public UBigNum {
public:
    static const UBigNum Modulus;
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
        return ModNumber();
    }
    Bytes toBytes() const {
        return Bytes();
    }
    const ModNumber operator+(const ModNumber &y) const {
        return ModNumber();
    }
    const ModNumber operator-(const ModNumber &y) const {
        return ModNumber();
    }
    const ModNumber operator*(const ModNumber &y) const {
        return ModNumber();
    }
    ModNumber inverse() const {
        return ModNumber();
    }
    static ModNumber inv(const ModNumber &x) {
        return ModNumber();
    }
    static ModNumber pow(const ModNumber &b, const ModNumber &e) {
        return ModNumber();
    }
    static ModNumber random() {
        return ModNumber();
    }
};
template <int mid> const UBigNum ModNumber<mid>::Modulus = ConstModulus[mid];
template <int mid> const int ModNumber<mid>::ByteLen = (Modulus.compactBitLen() + 7) / 8;

typedef ModNumber<0> P256FpNumber;
typedef ModNumber<1> P256ZqNumber;


template<class T1, class T2, int gxcid, int gycid>
class FpCurve {
public:
    typedef T1 FpNumber;
    typedef T2 ZqNumber;
    static const FpNumber b;
    bool isOnCurve(const FpNumber &x, const FpNumber &y) {
        return false;
    }
    class GroupElement
    {
    public:
        GroupElement() {

        }
        GroupElement(const FpNumber &x, const FpNumber &y) {

        }
        GroupElement(const GroupElement &P) {

        }
        bool isInfinite() const {
            return false;
        }
        Bytes toBytes() const {
            return Bytes();
        }
        FpNumber getAffineX() {
            return FpNumber();
        }
        FpNumber getAffineY() {
            return FpNumber();
        }
        GroupElement &operator=(const GroupElement &Y) {
            return *this;
        }
        bool operator==(const GroupElement &Y) const {
            return false;
        }
        bool operator!=(const GroupElement &y) const {
            return false;
        }
        const GroupElement operator+(const GroupElement &Y) const {
            return GroupElement();
        }
        const GroupElement operator-(const GroupElement &Y) const {
            return GroupElement();
        }
        const GroupElement operator-() const {
            return GroupElement();
        }
        static GroupElement productSimul(const ZqNumber &a, const GroupElement &X, const ZqNumber &b, const GroupElement &Y) {
            return GroupElement();
        }
        static bool isValidEncoding(const Bytes &bytes) {
            return false;
        }
        static GroupElement fromBytes(const Bytes &bytes) {
            return GroupElement();
        }
    };
    static const GroupElement generator;
    friend const GroupElement operator*(const ZqNumber &a, const GroupElement &X) {
        return GroupElement();
    }
};

template<class T1, class T2, int gxcid, int gycid>
const typename FpCurve<T1,T2,gxcid,gycid>::GroupElement FpCurve<T1, T2, gxcid, gycid>::generator = FpCurve<T1, T2, gxcid, gycid>::GroupElement();


typedef FpCurve<P256FpNumber, P256ZqNumber, 0, 1> CurveP256Group;
typedef FpCurve<P256FpNumber, P256ZqNumber, 0, 1> CurveP521Group;
typedef FpCurve<P256FpNumber, P256ZqNumber, 0, 1> CurveK283Group;
typedef FpCurve<P256FpNumber, P256ZqNumber, 0, 1> CurveB233Group;
