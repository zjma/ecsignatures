// PerformanceEvaluation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../ECSignatures/ECSignatures.h"

int main()
{
    int usercount = 1000;
    Bytes msg;
    for (int i = 0; i < usercount; ++i) {
        auto keypair = ECDSA_P256::keyGen();
        auto &sk = keypair.first;
        auto &pk = keypair.second;
        auto after_d3sign0 = ECDSA_P256::D3SignOffline(sk);
        auto &sign_st = after_d3sign0.first;
        auto &sig0 = after_d3sign0.second;
        auto vrfy_st = ECDSA_P256::D3VerifyOffline(pk, msg, sig0);
        auto sig1 = ECDSA_P256::D3SignOnline(sk, sign_st, msg);
        auto accepted = ECDSA_P256::D3VerifyOnline(pk, vrfy_st, sig1);
    }
    return 0;
}

