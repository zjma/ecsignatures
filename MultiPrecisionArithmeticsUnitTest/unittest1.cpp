#include<string>
using namespace std::string_literals;

#include "stdafx.h"
#include "CppUnitTest.h"
#include "../MultiPrecisionArithmetics/MultiPrecisionArithmetics.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MultiPrecisionArithmetics;

namespace MultiPrecisionArithmeticsUnitTest
{
    TEST_CLASS(UBigNumUnitTest)
    {
    public:

        TEST_METHOD(DefaultConstructAndStringify)
        {
            UBigNum x;
            Assert::AreEqual("0", x.toDecString().c_str());
            Assert::AreEqual("0", x.toHexString().c_str());
        }
        
        TEST_METHOD(SingleConstructAndStringify)
        {
            UBigNum x(666);
            Assert::AreEqual("666", x.toDecString().c_str());
            Assert::AreEqual("29A", x.toHexString().c_str());
        }

        TEST_METHOD(BytesConstructAndStringify)
        {
            Bytes bytes = { 1,2,3,4,5,6,7,8,9,0,0xab,0xcd,0xef,0xff,0xf,0xff,0xf,0,0,0 };
            UBigNum x(bytes);
            Assert::AreEqual("FFF0FFFEFCDAB00090807060504030201", x.toHexString().c_str());
        }

        TEST_METHOD(BytesConstructAndStringify2)
        {
            Bytes bytes;
            UBigNum x(bytes);
            Assert::AreEqual("0", x.toDecString().c_str());
        }

        TEST_METHOD(CopyConstructAndStringify)
        {
            auto x = UBigNum::fromHexString("2001032120010a321b2001c03d21e20010321f");
            UBigNum y(x);
            Assert::AreEqual("713712023516710914164789719223790760356753951", y.toDecString().c_str());
        }

        TEST_METHOD(DecConstructAndStringify)
        {
            auto x = UBigNum::fromDecString("456789456789456789456789");
            Assert::AreEqual("456789456789456789456789", x.toDecString().c_str());
            Assert::AreEqual("60BA9B2777BEB1484795", x.toHexString().c_str());
        }

        TEST_METHOD(DecConstructAndStringify2)
        {
            auto x = UBigNum::fromDecString("0000000099999999999999999999");
            Assert::AreEqual("99999999999999999999", x.toDecString().c_str());
            Assert::AreEqual("56BC75E2D630FFFFF", x.toHexString().c_str());
        }

        TEST_METHOD(DecConstructAndStringify3)
        {
            auto x = UBigNum::fromDecString("");
            Assert::AreEqual("0", x.toDecString().c_str());
            Assert::AreEqual("0", x.toHexString().c_str());
        }

        TEST_METHOD(HexConstructAndStringify)
        {
            auto x = UBigNum::fromHexString("abcdef1234567890ABCDEF1234567890");
            Assert::AreEqual("228367257073420209420274865598769821840", x.toDecString().c_str());
            Assert::AreEqual("ABCDEF1234567890ABCDEF1234567890", x.toHexString().c_str());
        }

        TEST_METHOD(HexConstructAndStringify2)
        {
            auto x = UBigNum::fromHexString("");
            Assert::AreEqual("0", x.toDecString().c_str());
            Assert::AreEqual("0", x.toHexString().c_str());
        }

        TEST_METHOD(InvalidDecConstruct)
        {
            auto f = []() {return UBigNum::fromDecString("-3"); };
            Assert::ExpectException<InvalidArgument>(f);
        }

        TEST_METHOD(InvalidDecConstruct2)
        {
            auto f = []() {return UBigNum::fromDecString("1234567890a"); };
            Assert::ExpectException<InvalidArgument>(f);
        }

        TEST_METHOD(InvalidHexConstruct)
        {
            auto f = []() {return UBigNum::fromHexString("1234567890abcdefg"); };
            Assert::ExpectException<InvalidArgument>(f);
        }

        TEST_METHOD(InvalidHexConstruct2)
        {
            auto f = []() {return UBigNum::fromHexString(" "); };
            Assert::ExpectException<InvalidArgument>(f);
        }

        TEST_METHOD(OperatorAssign)
        {
            auto x = UBigNum::fromHexString("fffffffffffffffffffffffffff");
            auto y = UBigNum::fromHexString("eeeeeee");
            x = y;
            Assert::AreEqual("EEEEEEE", x.toHexString().c_str());
        }

        TEST_METHOD(CompareEq)
        {
            auto x = UBigNum::fromDecString("1888946593147858085478");
            auto y = UBigNum::fromHexString("0000666666666666666666");
            Assert::AreEqual(UBigNum::cmp(x, y), 0);
        }

        TEST_METHOD(CompareEq2)
        {
            UBigNum x;
            auto y = UBigNum::fromHexString("000000");
            Assert::AreEqual(UBigNum::cmp(x, y), 0);
        }

        TEST_METHOD(CompareLt)
        {
            UBigNum x;
            auto y = UBigNum::fromDecString("1");
            Assert::AreEqual(UBigNum::cmp(x, y), -1);
        }

        TEST_METHOD(CompareLt2)
        {
            auto x = UBigNum::fromDecString("10000000");
            auto y = UBigNum::fromHexString("bbbbbbbbbbbbb");
            Assert::AreEqual(UBigNum::cmp(x, y), -1);
        }

        TEST_METHOD(CompareGt)
        {
            auto x = UBigNum::fromHexString("123456789a");
            auto y = UBigNum::fromHexString("1234567890");
            Assert::AreEqual(UBigNum::cmp(x, y), 1);
        }

        TEST_METHOD(CompareGt2)
        {
            auto x = UBigNum::fromDecString("1");
            UBigNum y;
            Assert::AreEqual(UBigNum::cmp(x, y), 1);
        }

        TEST_METHOD(OperatorEq)
        {
            auto x = UBigNum::fromDecString("987654321234567890");
            auto y = UBigNum::fromDecString("987654321234567890");
            Assert::IsTrue(x == y);
        }

        TEST_METHOD(OperatorNeq)
        {
            auto x = UBigNum::fromDecString("678324567876543");
            auto y = UBigNum::fromDecString("9876548987654789");
            Assert::IsTrue(x != y);
        }

        TEST_METHOD(OperatorLt)
        {
            auto x = UBigNum::fromDecString("2345432345");
            auto y = UBigNum::fromDecString("234542345432");
            Assert::IsTrue(x < y);
        }

        TEST_METHOD(OperatorLeq)
        {
            auto x = UBigNum::fromDecString("23456");
            auto y = UBigNum::fromDecString("78987654345678");
            Assert::IsTrue(x <= y);
        }

        TEST_METHOD(OperatorLeq2)
        {
            auto x = UBigNum::fromDecString("0");
            auto y = UBigNum::fromDecString("0");
            Assert::IsTrue(x <= y);
        }

        TEST_METHOD(OperatorGt)
        {
            auto x = UBigNum::fromDecString("54563454345645654564457654567");
            auto y = UBigNum::fromDecString("65434565434565");
            Assert::IsTrue(x > y);
        }

        TEST_METHOD(OperatorGeq)
        {
            auto x = UBigNum::fromDecString("34567895456789854567");
            auto y = UBigNum::fromDecString("34567895456788854567");
            Assert::IsTrue(x >= y);
        }

        TEST_METHOD(OperatorGeq2)
        {
            auto x = UBigNum::fromDecString("1");
            auto y = UBigNum::fromDecString("1");
            Assert::IsTrue(x >= y);
        }

        TEST_METHOD(OperatorAdd)
        {
            auto x = UBigNum::fromDecString("8888800000000000");
            auto y = UBigNum::fromDecString("0000099999999999");
            Assert::AreEqual("8888899999999999", (x + y).toDecString().c_str());
        }

        TEST_METHOD(OperatorAdd2)
        {
            auto x = UBigNum::fromHexString("d7c80e9c7bec0");
            auto y = UBigNum::fromHexString("0d5eae675d0ae5d05aeb");
            Assert::AreEqual("D5EAE74D98BCF9819AB", (x + y).toHexString().c_str());
        }

        TEST_METHOD(OperatorAdd3)
        {
            auto x = UBigNum::fromHexString("d7c80e9c7bec0");
            auto y = UBigNum::fromHexString("");
            Assert::AreEqual("D7C80E9C7BEC0", (x + y).toHexString().c_str());
        }

        TEST_METHOD(OperatorIncBy)
        {
            auto x = UBigNum::fromDecString("8888800000000000");
            auto y = UBigNum::fromDecString("0000099999999999");
            x += y;
            Assert::AreEqual("8888899999999999", x.toDecString().c_str());
        }

        TEST_METHOD(OperatorSub)
        {
            auto x = UBigNum::fromDecString("1000000000000000000000");
            auto y = UBigNum::fromDecString("0000000000000123456789");
            Assert::AreEqual("999999999999876543211", (x - y).toDecString().c_str());
        }

        TEST_METHOD(OperatorSub2)
        {
            auto x = UBigNum::fromDecString("1000000000000000000000");
            auto y = UBigNum::fromDecString("0000000000000123456789");
            Assert::AreEqual("999999999999876543211", (x - y).toDecString().c_str());
        }

        TEST_METHOD(OperatorSub3)
        {
            auto x = UBigNum::fromHexString("1000000000000000000000");
            auto y = UBigNum::fromHexString("1000000000000000000000");
            auto z = UBigNum::fromHexString("");
            Assert::AreEqual("0", (x - y).toDecString().c_str());
        }

        TEST_METHOD(OperatorDecBy)
        {
            auto x = UBigNum::fromDecString("1000000000000000000000");
            auto y = UBigNum::fromDecString("0000000000000123456789");
            x -= y;
            Assert::AreEqual("999999999999876543211", x.toDecString().c_str());
        }

        TEST_METHOD(InvalidSub)
        {
            auto f = []() {return UBigNum::fromDecString("12") - UBigNum::fromDecString("11111111111111111"); };
            Assert::ExpectException<NegativeDifference>(f);
        }

        TEST_METHOD(InvalidDecBy)
        {
            auto f = []() {
                auto x = UBigNum::fromDecString("123");
                auto y = UBigNum::fromDecString("456789");
                x -= y;
                return x;
            };
            Assert::ExpectException<NegativeDifference>(f);
        }

        TEST_METHOD(OperatorMul)
        {
            auto x = UBigNum::fromHexString("d7c80e9c7bec0");
            auto y = UBigNum::fromHexString("0d5eae675d0ae5d05aeb");
            Assert::AreEqual("B44F734699996AD4355D6FF38F6B9A40", (x*y).toHexString().c_str());
        }

        TEST_METHOD(OperatorMul2)
        {
            auto x = UBigNum::fromDecString("6666666666666666666666666666");
            auto y = UBigNum::fromDecString("55555555555555555555555555555");
            Assert::AreEqual("370370370370370370370370370329629629629629629629629629630", (x*y).toDecString().c_str());
        }

        TEST_METHOD(OperatorMul3)
        {
            auto x = UBigNum::fromDecString("0");
            auto y = UBigNum::fromDecString("3333333333333333333333333333");
            Assert::AreEqual("0", (x*y).toDecString().c_str());
        }

        TEST_METHOD(OperatorMulBy)
        {
            auto x = UBigNum::fromHexString("d7c80e9c7bec0");
            auto y = UBigNum::fromHexString("0d5eae675d0ae5d05aeb");
            x *= y;
            Assert::AreEqual("B44F734699996AD4355D6FF38F6B9A40", x.toHexString().c_str());
        }

        TEST_METHOD(OperatorDiv)
        {
            auto x = UBigNum::fromDecString("0987898798798798798787678745433454344");
            auto y = UBigNum::fromDecString("0987898798798798798787678745433454343");
            Assert::AreEqual("1", (x / y).toDecString().c_str());
        }

        TEST_METHOD(OperatorDiv2)
        {
            auto x = UBigNum::fromDecString("333333333333333333333333333333333333333333333333");
            auto y = UBigNum::fromDecString("444444444444444444444444444444444444");
            Assert::AreEqual("750000000000", (x / y).toDecString().c_str());
        }

        TEST_METHOD(OperatorDiv3)
        {
            auto x = UBigNum::fromDecString("0");
            auto y = UBigNum::fromDecString("1");
            Assert::AreEqual("0", (x / y).toDecString().c_str());
        }

        TEST_METHOD(OperatorDiv4)
        {
            auto x = UBigNum::fromDecString("10");
            auto y = UBigNum::fromDecString("1");
            Assert::AreEqual("10", (x / y).toDecString().c_str());
        }

        TEST_METHOD(OperatorDivBy)
        {
            auto x = UBigNum::fromDecString("333333333333333333333333333333333333333333333333");
            auto y = UBigNum::fromDecString("444444444444444444444444444444444444");
            x /= y;
            Assert::AreEqual("750000000000", x.toDecString().c_str());
        }

        TEST_METHOD(InvalidDiv)
        {
            auto f = []() {
                auto x = UBigNum::fromDecString("123123123123123123123");
                UBigNum y;
                return x / y;
            };
            Assert::ExpectException<DivideByZero>(f);

        }

        TEST_METHOD(InvalidDivBy)
        {
            auto f = []() {
                auto x = UBigNum::fromDecString("123123123123123123123");
                UBigNum y;
                x /= y;
                return x;
            };
            Assert::ExpectException<DivideByZero>(f);

        }

        TEST_METHOD(OperatorMod)
        {
            auto x = UBigNum::fromHexString("0d5eae675d0ae5d05aeb");
            auto y = UBigNum::fromHexString("d7c80e9c7bec0");
            Assert::AreEqual("64641B046BCEB", (x%y).toHexString().c_str());
        }

        TEST_METHOD(OperatorMod2)
        {
            auto x = UBigNum::fromHexString("d7c80e9c7bec0");
            auto y = UBigNum::fromHexString("0d5eae675d0ae5d05aeb");
            Assert::AreEqual("D7C80E9C7BEC0", (x%y).toHexString().c_str());
        }

        TEST_METHOD(OperatorMod3)
        {
            auto x = UBigNum::fromDecString("370370370370370370370370370329629629629629629629629629630");
            auto y = UBigNum::fromDecString("55555555555555555555555555555");
            Assert::AreEqual("0", (x%y).toDecString().c_str());
        }

        TEST_METHOD(OperatorMod4)
        {
            auto x = UBigNum::fromDecString("0000000000000000000000000000000");
            auto y = UBigNum::fromDecString("9999999999999999999999999999999");
            Assert::AreEqual("0", (x%y).toDecString().c_str());
        }

        TEST_METHOD(OperatorModBy)
        {
            auto x = UBigNum::fromHexString("2222222222222222222222222222222222222222");
            auto y = UBigNum::fromHexString("3333333333333333333333333");
            x %= y;
            Assert::AreEqual("2222222222444444444444444", x.toHexString().c_str());
        }

        TEST_METHOD(OperatorModBy2)
        {
            auto x = UBigNum::fromHexString("111111111111111111111111111111111111111111111111");
            auto y = UBigNum::fromHexString("1");
            x %= y;
            Assert::AreEqual("0", x.toHexString().c_str());
        }

        TEST_METHOD(InvalidMod)
        {
            auto f = []() {
                auto x = UBigNum::fromDecString("123123123123123123123");
                UBigNum y;
                return x%y;
            };
            Assert::ExpectException<DivideByZero>(f);
        }

        TEST_METHOD(InvalidModBy)
        {
            auto f = []() {
                auto x = UBigNum::fromDecString("123123123123123123123");
                UBigNum y;
                x%=y;
                return x;
            };
            Assert::ExpectException<DivideByZero>(f);
        }

        TEST_METHOD(OperatorShiftRight)
        {
            auto x = UBigNum::fromDecString("99999999999999999999999999999999999999999999");
            Assert::AreEqual("86736173798840354720596224", (x>>60).toDecString().c_str());
        }

        TEST_METHOD(OperatorShiftRight2)
        {
            auto x = UBigNum::fromDecString("99999999999999999999999999999999999999999999");
            Assert::AreEqual("0", (x >> 200).toDecString().c_str());
        }

        TEST_METHOD(OperatorShiftRight3)
        {
            auto x = UBigNum::fromDecString("86736173798840354720596224");
            Assert::AreEqual("86736173798840354720596224", (x >> 0).toDecString().c_str());
        }

        TEST_METHOD(OperatorShiftRightBy)
        {
            auto x = UBigNum::fromDecString("99999999999999999999999999999999999999999999");
            x >>= 60;
            Assert::AreEqual("86736173798840354720596224", x.toDecString().c_str());
        }

        TEST_METHOD(OperatorShiftLeft)
        {
            auto x = UBigNum::fromDecString("100000000000000000000");
            Assert::AreEqual("126765060022822940149670320537600000000000000000000", (x << 100).toDecString().c_str());
        }

        TEST_METHOD(OperatorShiftLeft2)
        {
            auto x = UBigNum::fromDecString("100000000000000000000");
            Assert::AreEqual("100000000000000000000", (x << 0).toDecString().c_str());

        }

        TEST_METHOD(OperatorShiftLeftBy)
        {
            auto x = UBigNum::fromDecString("100000000000000000000");
            x <<= 10;
            Assert::AreEqual("102400000000000000000000", x.toDecString().c_str());

        }

        TEST_METHOD(CompactBitLen)
        {
            auto x = UBigNum::fromHexString("45634565434563456543");
            Assert::AreEqual(79, x.compactBitLen());
        }

        TEST_METHOD(CompactBitLen2)
        {
            auto x = UBigNum::fromHexString("");
            Assert::AreEqual(0, x.compactBitLen());
        }

        TEST_METHOD(CompactBitLen3)
        {
            auto x = UBigNum::fromHexString("21212121212121214563456543");
            auto y = UBigNum::fromHexString("21212121212121214560000000");
            x -= y;
            Assert::AreEqual(26, x.compactBitLen());
        }

        TEST_METHOD(CompactBitLen4)
        {
            auto x = UBigNum::fromHexString("4563456543");
            auto y = UBigNum::fromHexString("4563456543");
            x *= y;

            Assert::AreEqual(77, x.compactBitLen());
        }

        TEST_METHOD(CompactBitLen5)
        {
            auto x = UBigNum::fromHexString("fffffffffffffffffffffffffffffffffffffffffffff");
            auto y = UBigNum::fromHexString("1");
            x += y;

            Assert::AreEqual(181, x.compactBitLen());
        }

        TEST_METHOD(CompactBitLen6)
        {
            auto x = UBigNum::fromHexString("fffffffffffffffffffffffffffffffffffffffffffff");
            auto y = UBigNum::fromHexString("1");
            x = y;
            Assert::AreEqual(1, x.compactBitLen());
        }

        TEST_METHOD(ToBytes)
        {
            auto x = UBigNum::fromHexString("000000000000b532423b5652b11b11bbbbbba43");
            auto xbytes = x.toBytes();
            Bytes bytes = {0x43,0xba,0xbb,0xbb,0x1b,0xb1,0x11,0x2b,0x65,0xb5,0x23,0x24,0x53,0x0b};
            Assert::AreEqual(bytes.size(), xbytes.size());
            auto n = bytes.size();
            for (int i = 0; i < n; ++i) {
                Assert::AreEqual(bytes[i], xbytes[i]);
            }
        }

        TEST_METHOD(ToBytes2)
        {
            UBigNum x;
            auto xbytes = x.toBytes();
            Assert::AreEqual(0, (int)xbytes.size());
        }
    };

    TEST_CLASS(P256ZqNumberTest) {
    public:
        TEST_METHOD(DefaultConstructAndStringify)
        {
            P256ZqNumber x;
            Assert::AreEqual("0", x.toDecString().c_str());
        }
        TEST_METHOD(ModConstructAndStringify)
        {
            P256ZqNumber x(UBigNum::fromDecString("8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888"));
            Assert::AreEqual("152EC0BE1D6D374A7E6D9961FD4ADD98532DCE4FDBC4BC62128AAA34F73283D8", x.toDecString().c_str());
        }
        TEST_METHOD(ModConstructAndStringify2)
        {
            P256ZqNumber x(UBigNum::fromDecString("8888888888888888888888888888888888888888888888888888"));
            Assert::AreEqual("8888888888888888888888888888888888888888888888888888", x.toDecString().c_str());
        }
        TEST_METHOD(ModConstructAndStringify3)
        {
            P256ZqNumber x(UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044369"));
            Assert::AreEqual("0", x.toDecString().c_str());
        }
        TEST_METHOD(Add)
        {
            P256ZqNumber x(UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044368"));
            P256ZqNumber y(UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044367"));
            Assert::AreEqual("115792089210356248762697446949407573529996955224135760342422259061068512044366", (x + y).toDecString().c_str());
        }
        TEST_METHOD(Add2)
        {
            P256ZqNumber x(UBigNum::fromDecString("9"));
            P256ZqNumber y(UBigNum::fromDecString("15"));
            Assert::AreEqual("26", (x + y).toDecString().c_str());
        }
        TEST_METHOD(Sub)
        {
            P256ZqNumber x(UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044368"));
            P256ZqNumber y(UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044367"));
            Assert::AreEqual("1", (x - y).toDecString().c_str());
        }
        TEST_METHOD(Sub2)
        {
            P256ZqNumber x(UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044367"));
            P256ZqNumber y(UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044368"));
            Assert::AreEqual("115792089210356248762697446949407573529996955224135760342422259061068512044368", (x - y).toDecString().c_str());
        }
        TEST_METHOD(Mul)
        {
            P256ZqNumber x(UBigNum::fromDecString("333333333333333333333333333333333333333333333333333333333333"));
            P256ZqNumber y(UBigNum::fromDecString("4444444444444444444444444444444444444444444444444444444"));
            Assert::AreEqual("90994184074089617355432738025097597368753842921875971443842063554856425665873", (x * y).toDecString().c_str());
        }
        TEST_METHOD(Mul2)
        {
            P256ZqNumber x(UBigNum::fromDecString("2"));
            P256ZqNumber y(UBigNum::fromDecString("33333333333333333333333"));
            Assert::AreEqual("66666666666666666666666", (x * y).toDecString().c_str());
        }
        TEST_METHOD(Inverse)
        {
            P256ZqNumber x(UBigNum::fromDecString("33333333333333333333333333333333333333333333333"));
            Assert::AreEqual("8907997423900091677643589948669673022704806214912975799323871470741453805152", x.inverse().toDecString().c_str());
        }
        TEST_METHOD(Inverse2)
        {
            P256ZqNumber x(UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044368"));
            Assert::AreEqual("115792089210356248762697446949407573529996955224135760342422259061068512044368", x.inverse().toDecString().c_str());
        }
        TEST_METHOD(InvalidInverse)
        {
            auto f = []() {P256ZqNumber x; return x.inverse(); };
            Assert::ExpectException<DivideByZero>(f);
        }
        TEST_METHOD(Power)
        {
            P256ZqNumber x(UBigNum::fromDecString("33333333333333333333333333333333333333333333333"));
            UBigNum a = 2222;
            Assert::AreEqual("104001243758660214686274006907352015357507299339100700464468384283042628328533", P256ZqNumber::pow(x, a).toDecString().c_str());
        }
        TEST_METHOD(Power2)
        {
            P256ZqNumber x(UBigNum::fromDecString("2222222222222222222222222"));
            auto a = UBigNum::fromDecString("115792089210356248762697446949407573529996955224135760342422259061068512044368");
            Assert::AreEqual("1", P256ZqNumber::pow(x, a).toDecString().c_str());
        }
        TEST_METHOD(Power3)
        {
            P256ZqNumber x(UBigNum::fromDecString("1111111"));
            UBigNum a;
            Assert::AreEqual("1111111", P256ZqNumber::pow(x, a).toDecString().c_str());
        }
    };
}
