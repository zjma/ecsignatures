// ECSignatures.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include<utility>
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
    auto pk = GroupSecp256k1::generator*sk;
    return std::make_pair(sk, pk);
}

ECSIGNATURES_API ByteString ECDSA_secp256k1::D0Sign(const SKey & sk, const std::vector<uint8_t>& msg)
{
    return ByteString();
}

ECSIGNATURES_API bool ECDSA_secp256k1::D0Verify(const PKey & pk, const ByteString & msg, const ByteString & sig)
{
    return false;
}

ECSIGNATURES_API ECDSA_secp256k1::D1SignInterim ECDSA_secp256k1::D1SignOffline(const SKey & sk)
{
    return D1SignInterim();
}

ECSIGNATURES_API ByteString ECDSA_secp256k1::D1SignOnline(const SKey & sk, const D1SignInterim & st, const ByteString & msg)
{
    return ByteString();
}

ECSIGNATURES_API bool ECDSA_secp256k1::D1Verify(const PKey & pk, const ByteString & msg, const ByteString & sig)
{
    return false;
}

ECSIGNATURES_API std::pair<ECDSA_secp256k1::D2SignInterim, ByteString> ECDSA_secp256k1::D2SignOffline(const SKey & sk)
{
    D2SignInterim st;
    ByteString sig0;
    return std::make_pair(st, sig0);
}

ECSIGNATURES_API ByteString ECDSA_secp256k1::D2SignOnline(const SKey & sk, const D2SignInterim & st, const ByteString & msg)
{
    return ByteString();
}

ECSIGNATURES_API bool ECDSA_secp256k1::D2Verify(const PKey & pk, const ByteString & msg, const ByteString & sig0, const ByteString & sig1)
{
    return false;
}

ECSIGNATURES_API std::pair<ECDSA_secp256k1::D3SignInterim, ByteString> ECDSA_secp256k1::D3SignOffline(const SKey & sk)
{
    D3SignInterim st;
    ByteString sig0;
    return std::make_pair(st, sig0);
}

ECSIGNATURES_API ByteString ECDSA_secp256k1::D3SignOnline(const SKey & sk, const D3SignInterim & st, const ByteString & msg)
{
    return ByteString();
}

ECSIGNATURES_API ECDSA_secp256k1::D3VerifyInterim ECDSA_secp256k1::D3VerifyOffline(const PKey & pk, const ByteString & msg, const ByteString & sig0)
{
    return D3VerifyInterim();
}

ECSIGNATURES_API bool ECDSA_secp256k1::D3VerifyOnline(const PKey & pk, const ByteString & sig1)
{
    return false;
}
