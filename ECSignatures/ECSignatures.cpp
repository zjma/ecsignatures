// ECSignatures.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include<utility>
#include "../HashLibrary/HashLibrary.h"
#include "ECSignatures.h"

ECDSA_P256::SKey::SKey(const GroupCurveP256::ZqNumber & x)
{
}

ECDSA_P256::PKey::PKey(const GroupCurveP256::GroupElement & Y)
{
}

ECSIGNATURES_API std::pair<ECDSA_P256::SKey, ECDSA_P256::PKey> ECDSA_P256::keyGen()
{
    auto sk = GroupCurveP256::ZqNumber::random();
    auto pk = sk*GroupCurveP256::generator;
    return std::make_pair(sk, pk);
}

ECSIGNATURES_API Bytes ECDSA_P256::D0Sign(const SKey & sk, const std::vector<uint8_t>& msg)
{
    return Bytes();
}

ECSIGNATURES_API bool ECDSA_P256::D0Verify(const PKey & pk, const Bytes & msg, const Bytes & sig)
{
    return false;
}

ECSIGNATURES_API ECDSA_P256::D1SignInterim ECDSA_P256::D1SignOffline(const SKey & sk)
{
    return D1SignInterim();
}

ECSIGNATURES_API Bytes ECDSA_P256::D1SignOnline(const SKey & sk, const D1SignInterim & st, const Bytes & msg)
{
    return Bytes();
}

ECSIGNATURES_API bool ECDSA_P256::D1Verify(const PKey & pk, const Bytes & msg, const Bytes & sig)
{
    return false;
}

ECSIGNATURES_API std::pair<ECDSA_P256::D2SignInterim, Bytes> ECDSA_P256::D2SignOffline(const SKey & sk)
{
    D2SignInterim st;
    Bytes sig0;
    return std::make_pair(st, sig0);
}

ECSIGNATURES_API Bytes ECDSA_P256::D2SignOnline(const SKey & sk, const D2SignInterim & st, const Bytes & msg)
{
    return Bytes();
}

ECSIGNATURES_API bool ECDSA_P256::D2Verify(const PKey & pk, const Bytes & msg, const Bytes & sig0, const Bytes & sig1)
{
    return false;
}

ECSIGNATURES_API std::pair<ECDSA_P256::D3SignInterim, Bytes> ECDSA_P256::D3SignOffline(const SKey & sk)
{
    auto r = GroupCurveP256::ZqNumber::random();
    auto A = r*GroupCurveP256::generator;
    auto d = (GroupCurveP256::ZqNumber)A.getAffineX();
    D3SignInterim st;
    st.rinv = r.inverse();
    st.rinvdx = st.rinv*d*sk.x;
    Bytes sig0 = d.toBytes();
    return std::make_pair(st, sig0);
}

ECSIGNATURES_API Bytes ECDSA_P256::D3SignOnline(const SKey & sk, const D3SignInterim & st, const Bytes & msg)
{
    GroupCurveP256::ZqNumber e = (GroupCurveP256::ZqNumber)DummyHash(msg);
    auto z = e*st.rinv + st.rinvdx;
    return z.toBytes();
}

ECSIGNATURES_API ECDSA_P256::D3VerifyInterim ECDSA_P256::D3VerifyOffline(const PKey & pk, const Bytes & msg, const Bytes & sig0)
{
    D3VerifyInterim st;
    st.d = UBigNum(sig0);
    GroupCurveP256::ZqNumber e = UBigNum(DummyHash(msg));
    st.ePdX = GroupCurveP256::simulProduct(e, GroupCurveP256::generator, st.d, pk.Y);
    return st;
}

ECSIGNATURES_API bool ECDSA_P256::D3VerifyOnline(const PKey & pk, const D3VerifyInterim &st, const Bytes & sig1)
{
    GroupCurveP256::ZqNumber z = UBigNum(sig1);
    auto A = z.inverse()*st.ePdX;
    auto dd = (GroupCurveP256::ZqNumber)A.getAffineX();
    return dd == st.d;
}
