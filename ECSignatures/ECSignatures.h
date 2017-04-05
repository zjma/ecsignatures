// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ECSIGNATURES_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ECSIGNATURES_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ECSIGNATURES_EXPORTS
#define ECSIGNATURES_API __declspec(dllexport)
#else
#define ECSIGNATURES_API __declspec(dllimport)
#endif

#include<vector>
#include<tuple>
#include "../HashLibrary/HashLibrary.h"
#include "../MultiPrecisionArithmetics/MultiPrecisionArithmetics.h"

extern ECSIGNATURES_API int dummy;

template <typename G>
class ECDSA_D0 {
public:
    static std::pair<Bytes, Bytes> genKey()
    {
        auto x = G::ZqNumber::random();
        auto Y = x*G::generator;
        return std::make_pair(x.toBytes(), Y.toBytes());
    }

    static Bytes sign(const Bytes &sk, const Bytes &msg)
    {
        auto x = G::ZqNumber::fromBytes(sk);
        auto r = G::ZqNumber::random();
        auto rinv = G::ZqNumber::inv(r);
        auto A = r*G::generator;
        auto d = A.getAffineX();
        auto e = G::ZqNumber::fromBytes(vhash(msg, G::ZqNumber::ByteLen));
        auto z = rinv*(e + d*x);
        auto dbytes = d.toBytes();
        auto zbytes = z.toBytes();
        Bytes ans;
        ans.insert(ans.end(), dbytes.begin(), dbytes.end());
        ans.insert(ans.end(), zbytes.begin(), zbytes.end());
        return ans;
    }

    static bool verify(const Bytes &pk, const Bytes &msg, const Bytes &sig)
    {
        if (!G::GroupElement::isValidEncoding(pk)) return false;
        if (sig.size() != G::ZqNumber::ByteLen * 2) return false;
        bool flag;
        G::GroupElement Y;
        std::tie(flag, Y) = G::GroupElement::fromBytes(pk);
        if (flag != 0) return false;
        auto e = G::ZqNumber::fromBytes(vhash(msg, G::ZqNumber::ByteLen));
        Bytes dbytes(sig.begin(), sig.begin() + G::ZqNumber::ByteLen);
        Bytes zbytes(sig.begin() + G::ZqNumber::ByteLen, sig.end());
        auto d = G::ZqNumber::fromBytes(dbytes);
        auto z = G::ZqNumber::fromBytes(zbytes);
        auto zinv = G::ZqNumber::inv(z);
        auto A = zinv*G::GroupElement::productSimul(e, G::generator, d, Y);
        auto dd = (G::ZqNumber)A.getAffineX();
        return dd == d;
    }
};

template <typename G>
class ECDSA_D1 {
public:
    static std::pair<Bytes, Bytes> genKey()
    {
        auto x = G::ZqNumber::random();
        auto Y = x*G::generator;
        return std::make_pair(x.toBytes(), Y.toBytes());
    }

    static Bytes signOffline(const Bytes &sk)
    {
        return Bytes();
    }

    static Bytes signOnline(const Bytes &sk, const Bytes &st, const Bytes &msg)
    {
        return Bytes();
    }

    static bool verify(const Bytes &pk, const Bytes &msg, const Bytes &sig)
    {
        return false;
    }
};

template <typename G>
class ECDSA_D2 {
public:
    static std::pair<Bytes, Bytes> genKey()
    {
        auto x = G::ZqNumber::random();
        auto Y = x*G::generator;
        return std::make_pair(x.toBytes(), Y.toBytes());
    }

    static std::pair<Bytes, Bytes> signOffline(const Bytes &sk)
    {
        return std::make_pair(Bytes(), Bytes());
    }
    static Bytes signOnline(const Bytes &sk, const Bytes &st, const Bytes &msg)
    {
        return Bytes();
    }
    static bool verify(const Bytes &pk, const Bytes &msg, const Bytes &sig0, const Bytes &sig1)
    {
        return false;
    }
};

template <class G>
class ECCDSA2_D3 {
public:
    static std::pair<Bytes, Bytes> genKey()
    {
        auto x = G::ZqNumber::random();
        auto Y = x*G::generator;
        return std::make_pair(x.toBytes(), Y.toBytes());
    }

    static std::pair<Bytes, Bytes> signOffline(const Bytes &sk)
    {
        return std::make_pair(Bytes(), Bytes());
    }

    static Bytes signOnline(const Bytes &sk, const Bytes &st, const Bytes &msg)
    {
        return Bytes();
    }

    static Bytes verifyOffline(const Bytes &pk, const Bytes &msg, const Bytes &sig0)
    {
        return Bytes();
    }

    static bool verifyOnline(const Bytes &pk, const Bytes &st, const Bytes sig1)
    {
        return false;
    }
};


typedef ECDSA_D0<CurveP256Group> ECDSA_P256_D0;
typedef ECDSA_D1<CurveP521Group> ECDSA_P521_D1;
typedef ECDSA_D2<CurveP256Group> ECDSA_P256_D2;
typedef ECCDSA2_D3<CurveP521Group> ECCDSA2_P521_D3;
