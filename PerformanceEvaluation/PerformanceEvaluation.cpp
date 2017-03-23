// PerformanceEvaluation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../ECSignatures/ECSignatures.h"

int main()
{
    int usercount = 1000;
    ByteString msg;
    for (int i = 0; i < usercount; ++i) {
        auto keypair = ECDSA_secp256k1::keyGen();
        auto &sk = keypair.first;
        auto &pk = keypair.second;
        auto after_d3sign0 = ECDSA_secp256k1::D3SignOffline(sk);
        auto &sign_st = after_d3sign0.first;
        auto &sig0 = after_d3sign0.second;
        auto vrfy_st = ECDSA_secp256k1::D3VerifyOffline(pk, msg, sig0);
        auto sig1 = ECDSA_secp256k1::D3SignOnline(sk, sign_st, msg);
        auto accepted = ECDSA_secp256k1::D3VerifyOnline(pk, sig1);
    }
    return 0;
}

