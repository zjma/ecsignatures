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
            Assert::IsTrue(x.toDecString() == "0");
            Assert::IsTrue(x.toHexString() == "0");
        }
        
        TEST_METHOD(CopyConstructAndStringify)
        {
            auto x = UBigNum::fromHexString("2001032120010a321b2001c03d21e20010321f");
            UBigNum y(x);
            Assert::IsTrue(y.toDecString()=="713712023516710914164789719223790760356753951");
        }

        TEST_METHOD(DecConstructAndStringify)
        {
            auto x = UBigNum::fromDecString("456789456789456789456789");
            Assert::IsTrue(x.toDecString() == "456789456789456789456789");
            Assert::IsTrue(x.toHexString() == "60BA9B2777BEB1484795");
        }

        TEST_METHOD(DecConstructAndStringify2)
        {
            auto x = UBigNum::fromDecString("0000000099999999999999999999");
            Assert::IsTrue(x.toDecString() == "99999999999999999999");
            Assert::IsTrue(x.toHexString() == "56BC75E2D630FFFFF");
        }

        TEST_METHOD(DecConstructAndStringify3)
        {
            auto x = UBigNum::fromDecString("");
            Assert::IsTrue(x.toDecString() == "0");
            Assert::IsTrue(x.toHexString() == "0");
        }

        TEST_METHOD(HexConstructAndStringify)
        {
            auto x = UBigNum::fromHexString("abcdef1234567890ABCDEF1234567890");
            Assert::IsTrue(x.toDecString() == "228367257073420209420274865598769821840");
            Assert::IsTrue(x.toHexString() == "ABCDEF1234567890ABCDEF1234567890");
        }

        TEST_METHOD(HexConstructAndStringify2)
        {
            auto x = UBigNum::fromHexString("");
            Assert::IsTrue(x.toDecString() == "0");
            Assert::IsTrue(x.toHexString() == "0");
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
            Assert::IsTrue(x.toDecString()=="eeeeeee");
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
            auto z = UBigNum::fromDecString("8888899999999999");
            Assert::IsTrue(x + y == z);
        }

        TEST_METHOD(OperatorAdd2)
        {
            auto x = UBigNum::fromHexString("d7c80e9c7bec0");
            auto y = UBigNum::fromHexString("0d5eae675d0ae5d05aeb");
            auto z = UBigNum::fromHexString("D5EAE74D98BCF9819AB");
            Assert::IsTrue(x + y == z);
        }

        TEST_METHOD(OperatorAdd3)
        {
            auto x = UBigNum::fromHexString("d7c80e9c7bec0");
            auto y = UBigNum::fromHexString("");
            auto z = UBigNum::fromHexString("d7c80e9c7bec0");
            Assert::IsTrue(x + y == z);
        }

        TEST_METHOD(OperatorIncBy)
        {
            auto x = UBigNum::fromDecString("8888800000000000");
            auto y = UBigNum::fromDecString("0000099999999999");
            x += y;
            auto z = UBigNum::fromDecString("8888899999999999");
            Assert::IsTrue(x == z);
        }

        TEST_METHOD(OperatorSub)
        {
            auto x = UBigNum::fromDecString("1000000000000000000000");
            auto y = UBigNum::fromDecString("0000000000000123456789");
            auto z = UBigNum::fromDecString("9999999999999876543211");
            Assert::IsTrue(x - y == z);
        }

        TEST_METHOD(OperatorSub2)
        {
            auto x = UBigNum::fromHexString("1000000000000000000000");
            auto y = UBigNum::fromHexString("0000000000000123456789");
            auto z = UBigNum::fromHexString("9999999999999876543211");
            Assert::IsTrue(x - y == z);
        }

        TEST_METHOD(OperatorSub3)
        {
            auto x = UBigNum::fromHexString("1000000000000000000000");
            auto y = UBigNum::fromHexString("1000000000000000000000");
            auto z = UBigNum::fromHexString("");
            Assert::IsTrue(x - y == z);
        }

        TEST_METHOD(OperatorDecBy)
        {
            auto x = UBigNum::fromDecString("1000000000000000000000");
            auto y = UBigNum::fromDecString("0000000000000123456789");
            x -= y;
            auto z = UBigNum::fromDecString("9999999999999876543211");
            Assert::IsTrue(x == z);
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
            auto z = UBigNum::fromHexString("B44F734699996AD4355D6FF38F6B9A40");
            Assert::IsTrue(x * y == z);
        }

        TEST_METHOD(OperatorMul2)
        {
            auto x = UBigNum::fromDecString("6666666666666666666666666666");
            auto y = UBigNum::fromDecString("55555555555555555555555555555");
            auto z = UBigNum::fromDecString("370370370370370370370370370329629629629629629629629629630");
            Assert::IsTrue(x * y == z);
        }

        TEST_METHOD(OperatorMul3)
        {
            auto x = UBigNum::fromDecString("0");
            auto y = UBigNum::fromDecString("3333333333333333333333333333");
            auto z = UBigNum::fromDecString("0");
            Assert::IsTrue(x * y == z);
        }

        TEST_METHOD(OperatorMulBy)
        {
            auto x = UBigNum::fromHexString("d7c80e9c7bec0");
            auto y = UBigNum::fromHexString("0d5eae675d0ae5d05aeb");
            x *= y;
            auto z = UBigNum::fromHexString("B44F734699996AD4355D6FF38F6B9A40");
            Assert::IsTrue(x == z);
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

            Assert::AreEqual(180, x.compactBitLen());
        }

        TEST_METHOD(CompactBitLen6)
        {
            auto x = UBigNum::fromHexString("fffffffffffffffffffffffffffffffffffffffffffff");
            auto y = UBigNum::fromHexString("1");
            x = y;
            Assert::AreEqual(180, x.compactBitLen());
        }
    };
}
