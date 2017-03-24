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
#include "../MultiPrecisionArithmetics/MultiPrecisionArithmetics.h"
using namespace MultiPrecisionArithmetics;



namespace ECDSA_P256
{
    class ECSIGNATURES_API SKey {
    public:
        GroupCurveP256::ZqNumber x;
        SKey(const GroupCurveP256::ZqNumber &x);
    };
    class ECSIGNATURES_API PKey {
    public:
        GroupCurveP256::GroupElement Y;
        PKey(const GroupCurveP256::GroupElement &Y);
    };
    class ECSIGNATURES_API D1SignInterim {};
    class ECSIGNATURES_API D2SignInterim {};
    class ECSIGNATURES_API D3SignInterim {
    public:
        GroupCurveP256::ZqNumber rinv;
        GroupCurveP256::ZqNumber rinvdx;
    };
    class ECSIGNATURES_API D3VerifyInterim {
    public:
        GroupCurveP256::ZqNumber d;
        GroupCurveP256::GroupElement ePdX;
    };

    ECSIGNATURES_API std::pair<SKey, PKey> keyGen();

    ECSIGNATURES_API Bytes D0Sign(const SKey &sk, const std::vector<uint8_t> &msg);
    ECSIGNATURES_API bool D0Verify(const PKey &pk, const Bytes &msg, const Bytes &sig);

    ECSIGNATURES_API D1SignInterim D1SignOffline(const SKey &sk);
    ECSIGNATURES_API Bytes D1SignOnline(const SKey &sk, const D1SignInterim &st, const Bytes &msg);
    ECSIGNATURES_API bool D1Verify(const PKey &pk, const Bytes &msg, const Bytes &sig);

    ECSIGNATURES_API std::pair<D2SignInterim, Bytes> D2SignOffline(const SKey &sk);
    ECSIGNATURES_API Bytes D2SignOnline(const SKey &sk, const D2SignInterim &st, const Bytes &msg);
    ECSIGNATURES_API bool D2Verify(const PKey &pk, const Bytes &msg, const Bytes &sig0, const Bytes &sig1);

    ECSIGNATURES_API std::pair<D3SignInterim, Bytes> D3SignOffline(const SKey &sk);
    ECSIGNATURES_API Bytes D3SignOnline(const SKey &sk, const D3SignInterim &st, const Bytes &msg);
    ECSIGNATURES_API D3VerifyInterim D3VerifyOffline(const PKey &pk, const Bytes &msg, const Bytes &sig0);
    ECSIGNATURES_API bool D3VerifyOnline(const PKey &pk, const D3VerifyInterim &st, const Bytes &sig1);

};
