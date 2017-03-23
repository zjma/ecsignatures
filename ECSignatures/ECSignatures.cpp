// ECSignatures.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include<utility>
#include "../HashLibrary/HashLibrary.h"
#include "ECSignatures.h"

ECDSA_secp256k1::SKey::SKey(const GroupSecp256k1::ZqNumber & x)
{
}

ECDSA_secp256k1::PKey::PKey(const GroupSecp256k1::GroupElement & Y)
{
}

ECSIGNATURES_API std::pair<ECDSA_secp256k1::SKey, ECDSA_secp256k1::PKey> ECDSA_secp256k1::keyGen()
{
    auto sk = GroupSecp256k1::ZqNumber::random();
    auto pk = sk*GroupSecp256k1::generator;
    return std::make_pair(sk, pk);
}

ECSIGNATURES_API Bytes ECDSA_secp256k1::D0Sign(const SKey & sk, const std::vector<uint8_t>& msg)
{
    return Bytes();
}

ECSIGNATURES_API bool ECDSA_secp256k1::D0Verify(const PKey & pk, const Bytes & msg, const Bytes & sig)
{
    return false;
}

ECSIGNATURES_API ECDSA_secp256k1::D1SignInterim ECDSA_secp256k1::D1SignOffline(const SKey & sk)
{
    return D1SignInterim();
}

ECSIGNATURES_API Bytes ECDSA_secp256k1::D1SignOnline(const SKey & sk, const D1SignInterim & st, const Bytes & msg)
{
    return Bytes();
}

ECSIGNATURES_API bool ECDSA_secp256k1::D1Verify(const PKey & pk, const Bytes & msg, const Bytes & sig)
{
    return false;
}

ECSIGNATURES_API std::pair<ECDSA_secp256k1::D2SignInterim, Bytes> ECDSA_secp256k1::D2SignOffline(const SKey & sk)
{
    D2SignInterim st;
    Bytes sig0;
    return std::make_pair(st, sig0);
}

ECSIGNATURES_API Bytes ECDSA_secp256k1::D2SignOnline(const SKey & sk, const D2SignInterim & st, const Bytes & msg)
{
    return Bytes();
}

ECSIGNATURES_API bool ECDSA_secp256k1::D2Verify(const PKey & pk, const Bytes & msg, const Bytes & sig0, const Bytes & sig1)
{
    return false;
}

ECSIGNATURES_API std::pair<ECDSA_secp256k1::D3SignInterim, Bytes> ECDSA_secp256k1::D3SignOffline(const SKey & sk)
{
    auto r = GroupSecp256k1::ZqNumber::random();
    auto A = r*GroupSecp256k1::generator;
    auto d = (GroupSecp256k1::ZqNumber)A.getAffineX();
    D3SignInterim st;
    st.rinv = r.getInverse();
    st.rinvdx = st.rinv*d*sk.x;
    Bytes sig0 = d.toBytes();
    return std::make_pair(st, sig0);
}

ECSIGNATURES_API Bytes ECDSA_secp256k1::D3SignOnline(const SKey & sk, const D3SignInterim & st, const Bytes & msg)
{
    GroupSecp256k1::ZqNumber e = (GroupSecp256k1::ZqNumber)DummyHash(msg);
    auto z = e*st.rinv + st.rinvdx;
    return z.toBytes();
}

ECSIGNATURES_API ECDSA_secp256k1::D3VerifyInterim ECDSA_secp256k1::D3VerifyOffline(const PKey & pk, const Bytes & msg, const Bytes & sig0)
{
    D3VerifyInterim st;
    st.d = UBigNum(sig0);
    GroupSecp256k1::ZqNumber e = UBigNum(DummyHash(msg));
    st.ePdX = GroupSecp256k1::simulProduct(e, GroupSecp256k1::generator, st.d, pk.Y);
    return st;
}

ECSIGNATURES_API bool ECDSA_secp256k1::D3VerifyOnline(const PKey & pk, const D3VerifyInterim &st, const Bytes & sig1)
{
    GroupSecp256k1::ZqNumber z = UBigNum(sig1);
    auto A = z.getInverse()*st.ePdX;
    auto dd = (GroupSecp256k1::ZqNumber)A.getAffineX();
    return dd == st.d;
}
