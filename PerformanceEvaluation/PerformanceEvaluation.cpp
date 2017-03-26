// PerformanceEvaluation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../ECSignatures/ECSignatures.h"


int main()
{
    int usercount = 1000;
    Bytes msg;
    for (int i = 0; i < usercount; ++i) {
        auto keypair = ECDSA_P256_D0::genKey();
        auto &sk = keypair.first;
        auto &pk = keypair.second;
        auto sig = ECDSA_P256_D0::sign(sk, msg);
        auto accepted = ECDSA_P256_D0::verify(pk, msg, sig);
    }

    for (int i = 0; i < usercount; ++i) {
        auto keypair = ECDSA_P521_D1::genKey();
        auto &sk = keypair.first;
        auto &pk = keypair.second;
        auto st = ECDSA_P521_D1::signOffline(sk);
        auto sig = ECDSA_P521_D1::signOnline(sk, st, msg);
        auto accepted = ECDSA_P521_D1::verify(pk, msg, sig);
    }

    for (int i = 0; i < usercount; ++i) {
        auto keypair = ECDSA_K283_D2::genKey();
        auto &sk = keypair.first;
        auto &pk = keypair.second;
        auto mid = ECDSA_K283_D2::signOffline(sk);
        auto &st = mid.first;
        auto &sig0 = mid.second;
        auto sig1 = ECDSA_K283_D2::signOnline(sk, st, msg);
        auto accepted = ECDSA_K283_D2::verify(pk, msg, sig0, sig1);
    }

    for (int i = 0; i < usercount; ++i) {
        auto keypair = ECCDSA2_B233_D3::genKey();
        auto &sk = keypair.first;
        auto &pk = keypair.second;
        auto sign_mid = ECCDSA2_B233_D3::signOffline(sk);
        auto &sst = sign_mid.first;
        auto &sig0 = sign_mid.second;
        auto vst = ECCDSA2_B233_D3::verifyOffline(pk, msg, sig0);
        auto sig1 = ECCDSA2_B233_D3::signOnline(sk, sst, msg);
        auto accepted = ECCDSA2_B233_D3::verifyOnline(pk, vst, sig1);
    }

    return 0;
}

