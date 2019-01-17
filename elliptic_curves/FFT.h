#pragma once
#include <mpir.h>
#include <string>

#include "Power.h"
#include "BitLength.h"

namespace ec_lib
{

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	class FFTEvaluator
	{
		static enum TypeOfFFT
		{
			BASIC_RADIX2_FFT,
			EXTENDED_RADIX2_FFT,
			STEP_RADIX2_FFT
		} type_of_fft;
		static typename BaseField::Elem root_of_unity_1;
		static typename BaseField::Elem root_of_unity_2;
		static typename BaseField::Elem root_of_unity_1_inv;
		static typename BaseField::Elem root_of_unity_2_inv;
		static unsigned long long root_of_unity_power_1;
		static unsigned long long root_of_unity_power_2;
		static typename BaseField::Elem root_of_unity_power_1_inv;
		static typename BaseField::Elem root_of_unity_power_2_inv;
		static mpz_t base_field_order_minus_one;

		static void GetRootOfUnity(typename BaseField::Elem& result, unsigned long long root_power)
		{
			if (root_power == 0)
			{
				BaseField::SetOne(result);
				return;
			}
			/*else if (root_power == 1)
			{
				BaseField::SetOne(result);
				BaseField::Neg(result, result);
				return;
			}*/

			mpz_t pow1;
			mpz_init(pow1);
			mpz_divexact_ui(pow1, base_field_order_minus_one, 1ul << root_power);
			Power power1(mpz_limbs_read(pow1), mpz_size(pow1));

			mp_limb_t pow2 = (1ul << (root_power - 1));
			Power power2(&pow2, 1);

			typename BaseField::Elem base, one, tmp;
			BaseField::SetOne(one);
			BaseField::SetOne(base);

			while (true)
			{
				//BaseField::GetRandomElement(result);
				BaseField::Add(base, base, one);
				BaseField::Exp(result, base, power1);
				BaseField::Exp(tmp, result, power2);
				if (!BaseField::IsOne(tmp))
				{
					mpz_clear(pow1);
					return;
				}
			}
		}

		static unsigned long long ReverseBits(unsigned long long x)
		{
			if (sizeof(unsigned long long) == 4)
			{
				x = ((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1);
				x = ((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2);
				x = ((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4);
				x = ((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8);
				x = ((x & 0xffff0000) >> 16) | ((x & 0x0000ffff) << 16);
				return x;
			}
			else if (sizeof(unsigned long long) == 8)
			{
				x = ((x & 0xaaaaaaaaaaaaaaaa) >> 1) | ((x & 0x5555555555555555) << 1);
				x = ((x & 0xcccccccccccccccc) >> 2) | ((x & 0x3333333333333333) << 2);
				x = ((x & 0xf0f0f0f0f0f0f0f0) >> 4) | ((x & 0x0f0f0f0f0f0f0f0f) << 4);
				x = ((x & 0xff00ff00ff00ff00) >> 8) | ((x & 0x00ff00ff00ff00ff) << 8);
				x = ((x & 0xffff0000ffff0000) >> 16) | ((x & 0x0000ffff0000ffff) << 16);
				x = ((x & 0xffffffff00000000) >> 32) | ((x & 0x00000000ffffffff) << 32);
				return x;
			}
			else
			{
				throw "Not implemented yet";
				return x;
			}
		}

		static void BasicFFT(typename BaseField::Elem* result, unsigned long long result_size, const typename BaseField::Elem* poly, unsigned long long poly_size, const typename BaseField::Elem& root_of_unity)
		{
			if (result_size == 0)
				return;

			// Reverse coefficients order
			for (unsigned long long k = 0; k < (1ul << result_size); k++)
			{
				//if (k < poly_size)
				BaseField::Copy(result[ReverseBits(k) >> (sizeof(unsigned long long) * 8 - result_size)], poly[k]);
				//else
				//	BaseField::SetZero(result[ReverseBits(k) >> (sizeof(unsigned long long) * 8 - result_size)]);
			}

			// Compute root of unity powers
			typename BaseField::Elem* roots_of_unity = new typename BaseField::Elem[result_size];
			BaseField::Copy(roots_of_unity[result_size - 1], root_of_unity);
			for (int i = result_size - 2; i >= 0; i--)
			{
				BaseField::Sqr(roots_of_unity[i], roots_of_unity[i + 1]);
			}

			// Init temp variables
			typename BaseField::Elem omega, tmp;

			// Evaluate FFT
			for (unsigned long long s = 0; s < result_size; s++)
			{
				unsigned long long m = (1ul << s);
				for (unsigned long long k = 0; k < (1ul << result_size); k += 2 * m)
				{
					BaseField::SetOne(omega);
					for (unsigned long long j = 0; j < m; j++)
					{
						BaseField::Mul(tmp, omega, result[k + j + m]);
						BaseField::Sub(result[k + j + m], result[k + j], tmp);
						BaseField::Add(result[k + j], result[k + j], tmp);
						BaseField::Mul(omega, omega, roots_of_unity[s]);
					}
				}
			}

			delete[] roots_of_unity;
		}

		static void BasicEvaluateLagrangePolynomials(typename BaseField::Elem* result, const typename BaseField::Elem& e, const typename BaseField::Elem& omega, unsigned long long size)
		{
			typename BaseField::Elem t;
			mp_limb_t m = (1ul << size);
			Power power(&m, 1);
			BaseField::Exp(t, e, power);
			if (BaseField::IsOne(t))
			{
				typename BaseField::Elem omega_i;
				BaseField::SetOne(omega_i);
				for (unsigned long long i = 0; i < m; i++)
				{
					if (BaseField::CheckEqual(omega_i, e))
					{
						BaseField::SetOne(result[i]);
					}
					else
					{
						BaseField::SetZero(result[i]);
					}
					BaseField::Mul(omega_i, omega_i, omega);
				}
				return;
			}

			typename BaseField::Elem Z, l, r, mm(to_string(m).c_str()), mm_inv, tmp;
			BaseField::SetOne(r);
			BaseField::Sub(Z, t, r);
			BaseField::Inv(mm_inv, mm);
			BaseField::Mul(l, Z, mm_inv);
			for (unsigned long long i = 0; i < m; i++)
			{
				BaseField::Sub(tmp, e, r);
				BaseField::Inv(tmp, tmp);
				BaseField::Mul(result[i], l, tmp);
				BaseField::Mul(l, l, omega);
				BaseField::Mul(r, r, omega);
			}
		}

	public:
		static unsigned long long polynomial_size;

		static void GetNthPoint(typename BaseField::Elem& result, unsigned long long n)
		{
			switch (type_of_fft)
			{
			case BASIC_RADIX2_FFT:
			{
				BaseField::Exp(result, root_of_unity_1, to_string(n).c_str());
				break;
			}

			case EXTENDED_RADIX2_FFT:
			{
				throw "Not implemented yet";
				// TODO! Implement
				break;
			}

			case STEP_RADIX2_FFT:
			{
				if (n < (1ul << root_of_unity_power_1))
				{
					BaseField::Exp(result, root_of_unity_1, to_string(2 * n).c_str());
				}
				else
				{
					BaseField::Exp(result, root_of_unity_2, to_string(n - (1ul << root_of_unity_power_1)).c_str());
					BaseField::Mul(result, result, root_of_unity_1);
				}
				break;
			}

			default:
			{
				throw "Unknown FFT type";
			}
			}
		}

		static void FFT(typename BaseField::Elem* result, const typename BaseField::Elem* poly)
		{
			if (result == poly)
			{
				throw "In-place FFT is not supported yet";
			}

			switch (type_of_fft)
			{
			case BASIC_RADIX2_FFT:
			{
				BasicFFT(result, root_of_unity_power_1, poly, root_of_unity_power_1, root_of_unity_1);
				break;
			}

			case EXTENDED_RADIX2_FFT:
			{
				throw "Not implemented yet";
				// TODO! Implement
				break;
			}

			case STEP_RADIX2_FFT:
			{
				typename BaseField::Elem* c = new typename BaseField::Elem[1ul << root_of_unity_power_1];
				typename BaseField::Elem* d = new typename BaseField::Elem[1ul << root_of_unity_power_1];
				typename BaseField::Elem* e = new typename BaseField::Elem[1ul << root_of_unity_power_2];
				typename BaseField::Elem omega;

				BaseField::SetOne(omega);
				for (unsigned long long i = 0; i < (1ul << root_of_unity_power_1); i++)
				{
					if (i < (1ul << root_of_unity_power_2))
					{
						BaseField::Add(c[i], poly[i], poly[i + (1ul << root_of_unity_power_1)]);
						BaseField::Sub(d[i], poly[i], poly[i + (1ul << root_of_unity_power_1)]);
						BaseField::Mul(d[i], d[i], omega);
					}
					else
					{
						BaseField::Copy(c[i], poly[i]);
						BaseField::Copy(d[i], poly[i]);
						BaseField::Mul(d[i], d[i], omega);
					}
					BaseField::Mul(omega, omega, root_of_unity_1);
				}

				unsigned long long compr = (1ul << (root_of_unity_power_1 - root_of_unity_power_2));
				for (unsigned long long i = 0; i < (1ul << root_of_unity_power_2); i++)
				{
					BaseField::SetZero(e[i]);
					for (unsigned long long j = 0; j < compr; j++)
					{
						BaseField::Add(e[i], e[i], d[i + j * (1ul << root_of_unity_power_2)]);
					}
				}

				// TODO! Square root_of_unity_1?
				typename BaseField::Elem root_of_unity_1_squared;
				BaseField::Sqr(root_of_unity_1_squared, root_of_unity_1);
				BasicFFT(result, root_of_unity_power_1, c, root_of_unity_power_1, root_of_unity_1_squared);
				BasicFFT(result + (1ul << root_of_unity_power_1), root_of_unity_power_2, e, root_of_unity_power_2, root_of_unity_2);

				delete[] c;
				delete[] d;
				delete[] e;
				break;
			}

			default:
			{
				throw "Unknown FFT type";
			}
			}
		}

		static void iFFT(typename BaseField::Elem* result, const typename BaseField::Elem* poly)
		{
			if (result == poly)
			{
				throw "In-place iFFT is not supported yet";
			}

			switch (type_of_fft)
			{
			case BASIC_RADIX2_FFT:
			{
				BasicFFT(result, root_of_unity_power_1, poly, root_of_unity_power_1, root_of_unity_1_inv);
				for (unsigned long long i = 0; i < polynomial_size; i++)
				{
					BaseField::Mul(result[i], result[i], root_of_unity_power_1_inv);
				}
				break;
			}

			case EXTENDED_RADIX2_FFT:
			{
				throw "Not implemented yet";
				// TODO! Implement
				break;
			}

			case STEP_RADIX2_FFT:
			{
				typename BaseField::Elem* c = new typename BaseField::Elem[1ul << root_of_unity_power_1];
				typename BaseField::Elem* d = new typename BaseField::Elem[1ul << root_of_unity_power_1];
				typename BaseField::Elem* e = new typename BaseField::Elem[1ul << root_of_unity_power_2];
				typename BaseField::Elem omega;

				// TODO! Square root_of_unity_1?
				typename BaseField::Elem root_of_unity_1_squared_inv;
				BaseField::Sqr(root_of_unity_1_squared_inv, root_of_unity_1_inv);
				BasicFFT(c, root_of_unity_power_1, poly, root_of_unity_power_1, root_of_unity_1_squared_inv);
				for (unsigned long long i = 0; i < (1ul << root_of_unity_power_1); i++)
				{
					BaseField::Mul(c[i], c[i], root_of_unity_power_1_inv);
				}

				BasicFFT(e, root_of_unity_power_2, poly + (1ul << root_of_unity_power_1), root_of_unity_power_2, root_of_unity_2_inv);
				for (unsigned long long i = 0; i < (1ul << root_of_unity_power_2); i++)
				{
					BaseField::Mul(e[i], e[i], root_of_unity_power_2_inv);
				}

				BaseField::SetOne(omega);
				for (unsigned long long i = 0; i < (1ul << root_of_unity_power_1); i++)
				{
					BaseField::Mul(d[i], c[i], omega);
					BaseField::Mul(omega, omega, root_of_unity_1);
				}

				// TODO! Risky operation, may be better to copy with BaseField::Copy
				//memcpy(result, c, polynomial_size * sizeof(typename BaseField::Elem));
				memcpy(result, c, (1ul << root_of_unity_power_1) * sizeof(typename BaseField::Elem));

				unsigned long long compr = (1ul << (root_of_unity_power_1 - root_of_unity_power_2));
				for (unsigned long long i = 0; i < (1ul << root_of_unity_power_2); i++)
				{
					for (unsigned long long j = 1; j < compr; j++)
					{
						BaseField::Sub(e[i], e[i], d[i + j * (1ul << root_of_unity_power_2)]);
					}
				}

				BaseField::SetOne(omega);
				for (size_t i = 0; i < (1ul << root_of_unity_power_2); i++)
				{
					BaseField::Mul(e[i], e[i], omega);
					BaseField::Mul(omega, omega, root_of_unity_1_inv);
				}

				omega = typename BaseField::Elem("2");
				BaseField::Inv(omega, omega);
				for (size_t i = 0; i < (1ul << root_of_unity_power_2); i++)
				{
					BaseField::Add(result[i], c[i], e[i]);
					BaseField::Mul(result[i], result[i], omega);
				}
				for (size_t i = 0; i < (1ul << root_of_unity_power_2); i++)
				{
					BaseField::Sub(result[(1ul << root_of_unity_power_1) + i], c[i], e[i]);
					BaseField::Mul(result[(1ul << root_of_unity_power_1) + i], result[(1ul << root_of_unity_power_1) + i], omega);
				}

				delete[] c;
				delete[] d;
				delete[] e;
				break;
			}

			default:
			{
				throw "Unknown FFT type";
			}
			}
		}

		static void MoveToCoset(typename BaseField::Elem* result, const typename BaseField::Elem* poly, const typename BaseField::Elem& e)
		{
			typename BaseField::Elem u;
			typename BaseField::Copy(u, e);
			for (unsigned long long i = 1; i < polynomial_size; i++)
			{
				BaseField::Mul(result[i], poly[i], u);
				BaseField::Mul(u, u, e);
			}
		}

		static void EvaluateLagrangePolynomials(typename BaseField::Elem* result, const typename BaseField::Elem& e)
		{
			switch (type_of_fft)
			{
			case BASIC_RADIX2_FFT:
			{
				BasicEvaluateLagrangePolynomials(result, e, root_of_unity_1, root_of_unity_power_1);
				break;
			}

			case EXTENDED_RADIX2_FFT:
			{
				throw "Not implemented yet";
				// TODO! Implement
				break;
			}

			case STEP_RADIX2_FFT:
			{
				typename BaseField::Elem tmp;
				mp_limb_t big_m = (1ul << root_of_unity_power_1), small_m = (1ul << root_of_unity_power_2);
				BaseField::Mul(tmp, e, root_of_unity_1_inv);
				typename BaseField::Elem root_of_unity_1_squared;
				BaseField::Sqr(root_of_unity_1_squared, root_of_unity_1);
				BasicEvaluateLagrangePolynomials(result, e, root_of_unity_1_squared, root_of_unity_power_1);
				BasicEvaluateLagrangePolynomials(result + big_m, tmp, root_of_unity_2, root_of_unity_power_2);

				typename BaseField::Elem L0, L1, omega_to_small_m, big_omega_to_small_m, elt;
				Power small_m_power(&small_m, 1), big_m_power(&big_m, 1);
				BaseField::Exp(omega_to_small_m, root_of_unity_1, small_m_power);
				BaseField::Exp(L0, e, small_m_power);
				BaseField::Sub(L0, L0, omega_to_small_m);
				BaseField::Sqr(big_omega_to_small_m, root_of_unity_1);
				BaseField::Exp(big_omega_to_small_m, big_omega_to_small_m, small_m_power);
				BaseField::SetOne(elt);
				BaseField::Exp(L1, e, big_m_power);
				BaseField::Sub(L1, L1, elt);
				BaseField::Exp(tmp, root_of_unity_1, big_m_power);
				BaseField::Sub(tmp, tmp, elt);
				BaseField::Inv(tmp, tmp);
				BaseField::Mul(L1, L1, tmp);
				for (unsigned long long i = 0; i < big_m; i++)
				{
					BaseField::Sub(tmp, elt, omega_to_small_m);
					BaseField::Inv(tmp, tmp);
					BaseField::Mul(result[i], result[i], L0);
					BaseField::Mul(result[i], result[i], tmp);
					BaseField::Mul(elt, elt, big_omega_to_small_m);
				}
				for (unsigned long long i = big_m; i < big_m + small_m; i++)
				{
					BaseField::Mul(result[i], result[i], L1);
				}
				break;
			}

			default:
			{
				throw "Unknown FFT type";
			}
			}
		}

		static void EvaluateVanishingPolynomial(typename BaseField::Elem& result, const typename BaseField::Elem& e)
		{
			switch (type_of_fft)
			{
			case BASIC_RADIX2_FFT:
			{
				typename BaseField::Elem tmp;
				mp_limb_t m = (1ul << root_of_unity_power_1);
				Power power(&m, 1);
				BaseField::Exp(result, e, power);
				BaseField::SetOne(tmp);
				BaseField::Sub(result, result, tmp);
				break;
			}

			case EXTENDED_RADIX2_FFT:
			{
				throw "Not implemented yet";
				// TODO! Implement
				break;
			}

			case STEP_RADIX2_FFT:
			{
				typename BaseField::Elem tmp1, tmp2;
				mp_limb_t big_m = (1ul << root_of_unity_power_1), small_m = (1ul << root_of_unity_power_2);
				Power small_m_power(&small_m, 1), big_m_power(&big_m, 1);
				BaseField::Exp(result, e, big_m_power);
				BaseField::SetOne(tmp1);
				BaseField::Sub(result, result, tmp1);
				BaseField::Exp(tmp1, e, small_m_power);
				BaseField::Exp(tmp2, root_of_unity_1, small_m_power);
				BaseField::Sub(tmp1, tmp1, tmp2);
				BaseField::Mul(result, result, tmp1);
				break;
			}

			default:
			{
				throw "Unknown FFT type";
			}
			}
		}

		static void AddVanishingPolynomial(typename BaseField::Elem* result, const typename BaseField::Elem& e)
		{
			switch (type_of_fft)
			{
			case BASIC_RADIX2_FFT:
			{
				mp_limb_t m = (1ul << root_of_unity_power_1);
				BaseField::Add(result[m], result[m], e);
				BaseField::Sub(result[0], result[0], e);
				break;
			}

			case EXTENDED_RADIX2_FFT:
			{
				throw "Not implemented yet";
				// TODO! Implement
				break;
			}

			case STEP_RADIX2_FFT:
			{
				typename BaseField::Elem tmp;
				mp_limb_t big_m = (1ul << root_of_unity_power_1), small_m = (1ul << root_of_unity_power_2);
				Power small_m_power(&small_m, 1);
				BaseField::Exp(tmp, root_of_unity_1, small_m_power);
				BaseField::Mul(tmp, tmp, e);
				BaseField::Add(result[big_m + small_m], result[big_m + small_m], e);
				BaseField::Sub(result[small_m], result[small_m], e);
				BaseField::Sub(result[big_m], result[big_m], tmp);
				BaseField::Add(result[0], result[0], tmp);
				break;
			}

			default:
			{
				throw "Unknown FFT type";
			}
			}
		}

		static void Init()
		{
			polynomial_size = kPolynomialSize;
			unsigned long long tmp1 = polynomial_size & (polynomial_size - 1);
			unsigned long long tmp2 = tmp1 & (tmp1 - 1);
			bool decreased = false;
			while ((tmp1 != 0) && (tmp2 != 0))
			{
				decreased = true;
				polynomial_size = tmp1;
				tmp1 = tmp2;
				tmp2 = tmp1 & (tmp1 - 1);
			}
			if (decreased)
			{
				polynomial_size = (polynomial_size << 1) - tmp1;
			}

			// Quick and dirty fix of the situation when second root of unity is 1
			// TODO! Research and fix if possible
			if (polynomial_size & 1 == 1)
			{
				polynomial_size++;
			}

			root_of_unity_power_1 = GetBitLength(polynomial_size) - 1;
			tmp1 = polynomial_size - (1ul << root_of_unity_power_1);
			root_of_unity_power_2 = (unsigned long long)GetBitLength(tmp1) - 1;

			mpz_init_set_str(base_field_order_minus_one, kBaseFieldOrder, 10);
			mpz_sub_ui(base_field_order_minus_one, base_field_order_minus_one, 1);
			mp_bitcnt_t max_root_of_unity_power = mpz_scan1(base_field_order_minus_one, 0);
			if (root_of_unity_power_2 == -1)
			{
				if (root_of_unity_power_1 <= max_root_of_unity_power)
				{
					type_of_fft = BASIC_RADIX2_FFT;
					GetRootOfUnity(root_of_unity_1, root_of_unity_power_1);
					BaseField::Inv(root_of_unity_1_inv, root_of_unity_1);
					root_of_unity_power_1_inv = typename BaseField::Elem(std::to_string(1ul << root_of_unity_power_1).c_str());
					BaseField::Inv(root_of_unity_power_1_inv, root_of_unity_power_1_inv);
				}
				else if (root_of_unity_power_1 == max_root_of_unity_power + 1)
				{
					type_of_fft = EXTENDED_RADIX2_FFT;
					throw "Not implemented yet";
					// TODO! Implement
				}
				else
				{
					throw "Required root of unity does not exist";
				}
			}
			else
			{
				type_of_fft = STEP_RADIX2_FFT;
				if (root_of_unity_power_1 + 1 > max_root_of_unity_power)
				{
					throw "Required root of unity does not exist";
				}
				GetRootOfUnity(root_of_unity_1, root_of_unity_power_1 + 1);
				GetRootOfUnity(root_of_unity_2, root_of_unity_power_2);
				BaseField::Inv(root_of_unity_1_inv, root_of_unity_1);
				BaseField::Inv(root_of_unity_2_inv, root_of_unity_2);
				root_of_unity_power_1_inv = typename BaseField::Elem(std::to_string(1ul << root_of_unity_power_1).c_str());
				BaseField::Inv(root_of_unity_power_1_inv, root_of_unity_power_1_inv);
				root_of_unity_power_2_inv = typename BaseField::Elem(std::to_string(1ul << root_of_unity_power_2).c_str());
				BaseField::Inv(root_of_unity_power_2_inv, root_of_unity_power_2_inv);
			}


			mpz_clear(base_field_order_minus_one);
		}

	};


	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	typename FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::TypeOfFFT FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::type_of_fft;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	typename BaseField::Elem FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::root_of_unity_1;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	typename BaseField::Elem FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::root_of_unity_2;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	typename BaseField::Elem FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::root_of_unity_1_inv;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	typename BaseField::Elem FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::root_of_unity_2_inv;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	unsigned long long FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::root_of_unity_power_1;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	unsigned long long FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::root_of_unity_power_2;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	typename BaseField::Elem FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::root_of_unity_power_1_inv;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	typename BaseField::Elem FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::root_of_unity_power_2_inv;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	unsigned long long FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::polynomial_size;

	template<class BaseField, const char* kBaseFieldOrder, unsigned long long kPolynomialSize>
	mpz_t FFTEvaluator<BaseField, kBaseFieldOrder, kPolynomialSize>::base_field_order_minus_one;

}