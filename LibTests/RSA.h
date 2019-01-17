#pragma once
#include <mpir.h>
#include "Random.h"

class RSA
{
public:
	static void KeyGen(mpz_t& n, mpz_t& d);
	static void Sign(mpz_t n, mpz_t d, mpz_t msg, mpz_t& sig);
	static bool Verify(mpz_t n, mpz_t msg, mpz_t sig);
};

