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


namespace MultiPrecisionArithmetics
{
    class MULTIPRECISIONARITHMETICS_API InvalidArgument : public std::exception {};
    class MULTIPRECISIONARITHMETICS_API NegativeDifference : public std::exception {};


    class MULTIPRECISIONARITHMETICS_API UBigNum {
    private:
        std::vector<uint32_t> val;
    public:
        ///<summary>Create a UBigNum whose value is 0.</summary>
        UBigNum();

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
        UBigNum& operator+=(const uint32_t &y);
        UBigNum& operator-=(const uint32_t &y);
        UBigNum& operator*=(const uint32_t &y);
        const UBigNum operator+(const UBigNum &y) const;
        const UBigNum operator-(const UBigNum &y) const;
        const UBigNum operator*(const UBigNum &y) const;

        ///<summary>Get smallest k such that this value is in [0,2^k-1].</summary>
        int compactBitLen() const;


    };
}

namespace GroupSecp256k1 {
    class MULTIPRECISIONARITHMETICS_API FpNumber
    {
    public:
        FpNumber &operator=(const FpNumber &y);
        bool operator==(const FpNumber &y) const;
        bool operator!=(const FpNumber &y) const;
        FpNumber& operator+=(const FpNumber &y);
        FpNumber& operator-=(const FpNumber &y);
        FpNumber& operator*=(const FpNumber &y);
        FpNumber& operator/=(const FpNumber &y);
        const FpNumber operator+(const FpNumber &y) const;
        const FpNumber operator-(const FpNumber &y) const;
        const FpNumber operator*(const FpNumber &y) const;
        const FpNumber operator/(const FpNumber &y) const;
    };
    class MULTIPRECISIONARITHMETICS_API ZqNumber
    {
    public:
        ZqNumber &operator=(const ZqNumber &y);
        bool operator==(const ZqNumber &y) const;
        bool operator!=(const ZqNumber &y) const;
        ZqNumber& operator+=(const ZqNumber &y);
        ZqNumber& operator-=(const ZqNumber &y);
        ZqNumber& operator*=(const ZqNumber &y);
        ZqNumber& operator/=(const ZqNumber &y);
        const ZqNumber operator+(const ZqNumber &y) const;
        const ZqNumber operator-(const ZqNumber &y) const;
        const ZqNumber operator*(const ZqNumber &y) const;
        const ZqNumber operator/(const ZqNumber &y) const;
        static ZqNumber random();
    };
    class MULTIPRECISIONARITHMETICS_API GroupElement
    {
    public:
        GroupElement &operator=(const GroupElement &Y);
        bool operator==(const GroupElement &Y) const;
        bool operator!=(const GroupElement &y) const;
        GroupElement& operator+=(const GroupElement &Y);
        GroupElement& operator-=(const GroupElement &Y);
        const GroupElement operator+(const GroupElement &Y) const;
        const GroupElement operator-(const GroupElement &Y) const;
        const GroupElement operator*(const ZqNumber &x) const;
    };
    FpNumber a;
    FpNumber b;
    GroupElement generator;
};
