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


namespace MultiPrecisionArithmetics
{
    class MULTIPRECISIONARITHMETICS_API InvalidArgument : public std::exception {};
    class MULTIPRECISIONARITHMETICS_API NegativeDifference : public std::exception {};
    class MULTIPRECISIONARITHMETICS_API DivideByZero : public std::exception {};

    class MULTIPRECISIONARITHMETICS_API UBigNum {
    private:
        std::vector<uint32_t> val;
    public:
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
    
    class MULTIPRECISIONARITHMETICS_API P256FpNumber :public UBigNum {
    public:
        P256FpNumber();
        P256FpNumber(const UBigNum &x);
        const P256FpNumber operator+(const P256FpNumber &y) const;
        const P256FpNumber operator-(const P256FpNumber &y) const;
        const P256FpNumber operator*(const P256FpNumber &y) const;
        P256FpNumber inverse() const;
        static P256FpNumber pow(const P256FpNumber &b, const P256FpNumber &e);

    };
    

    class MULTIPRECISIONARITHMETICS_API P256ZqNumber : public UBigNum {
    public:
        const static UBigNum q;
        P256ZqNumber();
        P256ZqNumber(const UBigNum &x);
        const P256ZqNumber operator+(const P256ZqNumber &y) const;
        const P256ZqNumber operator-(const P256ZqNumber &y) const;
        const P256ZqNumber operator*(const P256ZqNumber &y) const;
        P256ZqNumber inverse() const;
        static P256ZqNumber pow(const P256ZqNumber &b, const P256ZqNumber &e);
        static P256ZqNumber random();
    };
}

namespace GroupCurveP256 {
    typedef MultiPrecisionArithmetics::P256FpNumber FpNumber;
    typedef MultiPrecisionArithmetics::P256ZqNumber ZqNumber;
    MULTIPRECISIONARITHMETICS_API FpNumber a;
    MULTIPRECISIONARITHMETICS_API FpNumber b;
    class MULTIPRECISIONARITHMETICS_API GroupElement
    {
    public:
        FpNumber getAffineX();
        GroupElement &operator=(const GroupElement &Y);
        bool operator==(const GroupElement &Y) const;
        bool operator!=(const GroupElement &y) const;
        const GroupElement operator+(const GroupElement &Y) const;
        const GroupElement operator-(const GroupElement &Y) const;
    };
    MULTIPRECISIONARITHMETICS_API const GroupElement operator*(const ZqNumber &a, const GroupElement &X);
    MULTIPRECISIONARITHMETICS_API GroupElement simulProduct(const ZqNumber &a, const GroupElement &X, const ZqNumber &b, const GroupElement &Y);
    MULTIPRECISIONARITHMETICS_API GroupElement generator;
};
