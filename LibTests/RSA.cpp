#include "RSA.h"

void RSA::KeyGen(mpz_t& n, mpz_t& d)
{
	unsigned char random_buffer[64];
	gmp_randstate_t rand_state;
	gmp_randinit_default(rand_state);
	mpz_t p, q, phi;
	mpz_init(p);
	mpz_init(q);
	do
	{
		Random::GetInstance()->GenBytes(random_buffer, 64);
		random_buffer[0] |= 0xc0;
		random_buffer[63] |= 0x01;
		mpz_import(p, 64, 1, 1, 1, 0, random_buffer);

	} while (mpz_likely_prime_p(p, rand_state, 0) == 0);
	do
	{
		Random::GetInstance()->GenBytes(random_buffer, 64);
		random_buffer[0] |= 0xc0;
		random_buffer[63] |= 0x01;
		mpz_import(q, 64, 1, 1, 1, 0, random_buffer);
	} while (mpz_likely_prime_p(q, rand_state, 0) == 0);
	mpz_init(n);
	mpz_mul(n, p, q);
	mpz_init_set_ui(d, 17);
	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_init(phi);
	mpz_mul(phi, p, q);
	mpz_invert(d, d, phi);
}

void RSA::Sign(mpz_t n, mpz_t d, mpz_t msg, mpz_t& sig)
{
	mpz_init(sig);
	mpz_powm(sig, msg, d, n);
}

bool RSA::Verify(mpz_t n, mpz_t msg, mpz_t sig)
{
	mpz_t msg1;
	mpz_init(msg1);
	mpz_powm_ui(msg1, sig, 17, n);
	return (mpz_cmp(msg, msg1) == 0);
}
