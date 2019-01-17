#pragma once
#include <mpir.h>
#include <gmp-impl.h>
#include <string>
#include <fstream>

#include <iostream>
#include <fstream>

#include "Power.h"
#include "Random.h"

using namespace std;

namespace ec_lib
{

	const char example[] = "100500";

	template<const char* kModulus, mp_bitcnt_t kBitCount>
	class PrimeField
	{
	public:

		//static ofstream fout;

		static const unsigned int uiLimbCount = (kBitCount + 8 * sizeof(mp_limb_t) - 1) / (8 * sizeof(mp_limb_t));
		static const unsigned int uiByteCount = (kBitCount + 7) / 8;
		static mp_limb_t modulus[uiLimbCount + 1];
		static mp_limb_t modulus_inv;
		static mp_limb_t normalizer[uiLimbCount];

		static Random* rnd;

		class Elem
		{
			friend class PrimeField<kModulus, kBitCount>;
			friend class Power;

		public:
			mp_limb_t limbs[uiLimbCount + 1];

			// Special form
			bool has_special_form;
			bool is_zero;
			bool is_negative;
			bool is_one;

			void Redcify()
			{
				mp_limb_t buffer1[2 * uiLimbCount];
				mp_limb_t buffer2[uiLimbCount + 1];

				MPN_ZERO(buffer1, uiLimbCount);
				MPN_COPY(buffer1 + uiLimbCount, limbs, uiLimbCount);
				mpn_tdiv_qr(buffer2, limbs, 0L, buffer1, 2 * uiLimbCount, modulus, uiLimbCount);
			}

			void Normalize()
			{
				mp_limb_t buffer1[2 * uiLimbCount];
				mp_limb_t buffer2[uiLimbCount + 1];
				mpn_mul_n(buffer1, limbs, normalizer, uiLimbCount);
				MPN_ZERO(limbs, uiLimbCount);
				mpn_tdiv_qr(buffer2, limbs, 0, buffer1, 2 * uiLimbCount, modulus, uiLimbCount);
			}

		public:
			Elem()
				: has_special_form(false)
			{

			}

			Elem(const Elem& other)
				: has_special_form(false)
			{
				memcpy(limbs, other.limbs, uiLimbCount * sizeof(mp_limb_t));
			}

			/*Elem(Elem&& other)
				: has_special_form(false)
			{
				memcpy(limbs, other.limbs, uiLimbCount * sizeof(mp_limb_t));
			}*/

			Elem(const char* str_value)
				: has_special_form(false)
			{
				MPN_ZERO(limbs, uiLimbCount);
				size_t str_len = strlen(str_value);
				unsigned int neg = 0;
				if ((str_len > 1) && (str_value[0] == '-'))
				{
					neg = 1;
					str_len--;
				}
				unsigned char* str_copy = (unsigned char*)malloc(str_len);
				for (size_t i = 0; i < str_len; i++)
					str_copy[i] = str_value[i + neg] - '0';
				mpn_set_str(limbs, str_copy, str_len, 10);
				Redcify();
				if (neg)
				{
					Neg(*this, *this);
				}
				free(str_copy);
			}

			void ToSpecialForm()
			{
				if (IsZero(*this))
				{
					has_special_form = true;
					is_zero = true;
					is_negative = false;
					is_one = false;
					return;
				}

				if (IsOne(*this))
				{
					has_special_form = true;
					is_zero = false;
					is_negative = false;
					is_one = true;
					return;
				}

				Elem neg_self;
				Neg(neg_self, *this);

				if (IsOne(neg_self))
				{
					has_special_form = true;
					is_zero = false;
					is_negative = true;
					is_one = true;
					return;
				}

				has_special_form = false;
			}

			operator Power() const
			{
				mp_limb_t buffer1[2 * uiLimbCount];
				mp_limb_t buffer2[uiLimbCount + 1];
				mp_limb_t buffer3[uiLimbCount + 1];
				mpn_mul_n(buffer1, limbs, normalizer, uiLimbCount);
				mpn_tdiv_qr(buffer2, buffer3, 0, buffer1, 2 * uiLimbCount, modulus, uiLimbCount);
				return Power(buffer3, uiLimbCount);
			}

			friend std::istream& operator>> (std::istream& stream, Elem& e)
			{
				memset(e.limbs, 0, uiLimbCount * sizeof(mp_limb_t));
				const int buffer_size = uiLimbCount * sizeof(mp_limb_t);
				const int number_size = uiByteCount;
				unsigned char buffer[buffer_size + 1];
				stream.read((char*)buffer + (buffer_size - number_size), number_size);
				mpn_set_str(e.limbs, buffer, buffer_size, 256);
				return stream;
			}

			friend std::ostream& operator<< (std::ostream& stream, const Elem& e)
			{
				const int buffer_size = uiLimbCount * sizeof(mp_limb_t);
				const int number_size = uiByteCount;
				unsigned char buffer[buffer_size + 1];
				memset(buffer, 0, buffer_size + 1);
				unsigned long long limb_count = uiLimbCount;
				int written = 0;
				while ((limb_count > 0) && (e.limbs[limb_count - 1] == 0))
				{
					limb_count--;
				}
				if (limb_count != 0)
				{
					written = mpn_get_str(buffer, 256, (mp_ptr)e.limbs, limb_count);
				}
				for (int i = 0; i < number_size - written; i++)
				{
					stream.put(0);
				}
				stream.write((char*)buffer, written);
				return stream;
			}

			friend std::istream& operator>> (std::istream& stream, vector<Elem>& e)
			{
				unsigned int e_size = 0;
				unsigned char tmp;
				for (int i = 0; i < 4; i++)
				{
					stream.read((char*)&tmp, 1);
					e_size |= (unsigned int)tmp << (8 * i);
				}
				e.resize(e_size);
				for (Elem& elem : e)
				{
					stream >> elem;
				}
				return stream;
			}

			friend std::ostream& operator<< (std::ostream& stream, const vector<Elem>& e)
			{
				unsigned char tmp;
				for (int i = 0; i < 4; i++)
				{
					tmp = (e.size() >> (8 * i)) & 0xff;
					stream.write((const char*)&tmp, 1);
				}
				for (const Elem& elem : e)
				{
					stream << elem;
				}
				return stream;
			}
		};

		static bool IsZero(const Elem& e1)
		{
			return CheckEqual(e1, zero);
		}

		static bool IsOne(const Elem& e1)
		{
			return CheckEqual(e1, one);
		}

		static void SetZero(Elem& e1)
		{
			MPN_ZERO(e1.limbs, uiLimbCount);
		}

		static void SetOne(Elem& e1)
		{
			MPN_COPY(e1.limbs, one.limbs, uiLimbCount);
		}

		static void Copy(Elem& result, const Elem& e1)
		{
			memcpy(result.limbs, e1.limbs, uiLimbCount * sizeof(mp_limb_t));
		}

		static void Add(Elem& result, const Elem& e1, const Elem& e2)
		{
			//fout << "print(GFp(" << ToString(e1) << ") + GFp(" << ToString(e2) << ") == ";
			mp_limb_t buffer[uiLimbCount + 1];
			buffer[uiLimbCount] = mpn_add_n(buffer, e1.limbs, e2.limbs, uiLimbCount);
			if (mpn_cmp(buffer, modulus, uiLimbCount + 1) >= 0)
			{
				mpn_sub(result.limbs, buffer, uiLimbCount + 1, modulus, uiLimbCount);
			}
			else
			{
				memcpy(result.limbs, buffer, uiLimbCount * sizeof(mp_limb_t));
				result.limbs[uiLimbCount] = 0;
			}
			//fout << "GFp(" << ToString(result) << "))\n";
		}

		static void Add(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			//fout << "print(GFp(" << ToString(e1) << ") + GFp(" << e2 << ") == ";
			mp_limb_t buffer[uiLimbCount + 1];
			buffer[uiLimbCount] = mpn_add_1(buffer, e1.limbs, uiLimbCount, e2);
			if (mpn_cmp(buffer, modulus, uiLimbCount + 1) >= 0)
			{
				mpn_sub(result.limbs, buffer, uiLimbCount + 1, modulus, uiLimbCount);
			}
			else
			{
				memcpy(result.limbs, buffer, uiLimbCount * sizeof(mp_limb_t));
				result.limbs[uiLimbCount] = 0;
			}
			//fout << "GFp(" << ToString(result) << "))\n";
		}

		static void Dbl(Elem& result, const Elem& e1) // TODO! Check!
		{
			//fout << "print(GFp(" << ToString(e1) << ") * 2 == ";
			mp_limb_t buffer[uiLimbCount + 1];
			buffer[uiLimbCount] = mpn_lshift(buffer, e1.limbs, uiLimbCount, 1);
			if (mpn_cmp(buffer, modulus, uiLimbCount + 1) >= 0)
			{
				mpn_sub(result.limbs, buffer, uiLimbCount + 1, modulus, uiLimbCount);
			}
			else
			{
				memcpy(result.limbs, buffer, uiLimbCount * sizeof(mp_limb_t));
				result.limbs[uiLimbCount] = 0;
			}
			//fout << "GFp(" << ToString(result) << "))\n";
		}

		static void Neg(Elem& result, const Elem& e1)
		{
			if (IsZero(e1))
			{
				Copy(result, e1);
				return;
			}
			mpn_sub_n(result.limbs, modulus, e1.limbs, uiLimbCount);
		}

		static void Sub(Elem& result, const Elem& e1, const Elem& e2)
		{
			//fout << "print(GFp(" << ToString(e1) << ") - GFp(" << ToString(e2) << ") == ";
			if (mpn_cmp(e1.limbs, e2.limbs, uiLimbCount) < 0)
			{
				result.limbs[uiLimbCount] = mpn_add_n(result.limbs, e1.limbs, modulus, uiLimbCount);
			}
			else
			{
				memcpy(result.limbs, e1.limbs, uiLimbCount * sizeof(mp_limb_t));
				result.limbs[uiLimbCount] = 0;
			}
			mpn_sub(result.limbs, result.limbs, uiLimbCount + 1, e2.limbs, uiLimbCount);
			//fout << "GFp(" << ToString(result) << "))\n";
		}

		static void Sub(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			//fout << "print(GFp(" << ToString(e1) << ") - GFp(" << e2 << ") = ";
			static const mp_limb_t buffer[uiLimbCount - 1] = { 0 };
			if ((mpn_cmp(e1.limbs, &e2, 1) < 0) && ((uiLimbCount == 1) || (mpn_cmp(e1.limbs + 1, buffer, uiLimbCount - 1) == 0)))
			{
				result.limbs[uiLimbCount] = mpn_add_n(result.limbs, e1.limbs, modulus, uiLimbCount);
			}
			else
			{
				memcpy(result.limbs, e1.limbs, uiLimbCount * sizeof(mp_limb_t));
				result.limbs[uiLimbCount] = 0;
			}
			mpn_sub_1(result.limbs, result.limbs, uiLimbCount + 1, e2);
			//fout << "GFp(" << ToString(result) << "))\n";
		}

		static void Sqr(Elem& result, const Elem& e1)
		{
			//fout << "print(GFp(" << ToString(e1) << ") ** 2 ==  ";
			mp_limb_t buffer1[2 * uiLimbCount];
			mp_limb_t buffer2[uiLimbCount + 1];
			mpn_sqr(buffer1, e1.limbs, uiLimbCount);
			mpn_redc_1(result.limbs, buffer1, modulus, uiLimbCount, modulus_inv);
			if (mpn_cmp(result.limbs, modulus, uiLimbCount) >= 0)
			{
				mpn_sub_n(result.limbs, result.limbs, modulus, uiLimbCount);
			}
			//fout << "GFp(" << ToString(result) << "))\n";
		}

		static void Mul(Elem& result, const Elem& e1, const Elem& e2)
		{
			//fout << "print(GFp(" << ToString(e1) << ") * GFp(" << ToString(e2) << ") == ";
			if (!e2.has_special_form)
			{
				mp_limb_t buffer1[2 * uiLimbCount];
				mp_limb_t buffer2[uiLimbCount + 1];
				mpn_mul_n(buffer1, e1.limbs, e2.limbs, uiLimbCount);
				mpn_redc_1(result.limbs, buffer1, modulus, uiLimbCount, modulus_inv);
				if (mpn_cmp(result.limbs, modulus, uiLimbCount) >= 0)
				{
					mpn_sub_n(result.limbs, result.limbs, modulus, uiLimbCount);
				}
			}
			else // TODO! Looks like ordinary multiplication is faster than bitshift. WTF?
			{
				if (e2.is_zero)
				{
					SetZero(result);
					//fout << "GFp(" << ToString(result) << "))\n";
					return;
				}
				if (e2.is_one)
				{
					if (e2.is_negative)
					{
						Neg(result, e1);
						//fout << "GFp(" << ToString(result) << "))\n";
						return;
					}
					else
					{
						Copy(result, e1);
						//fout << "GFp(" << ToString(result) << "))\n";
						return;
					}
				}
			}
			//fout << "GFp(" << ToString(result) << "))\n";
		}

		static void Mul(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			//fout << "print(GFp(" << ToString(e1) << ") * GFp(" << e2 << ") == ";
			mp_limb_t buffer1[/*2 * uiLimbCount*/uiLimbCount + 1];
			mp_limb_t buffer2[uiLimbCount + 1];
			//MPN_ZERO(buffer1 + uiLimbCount + 1, uiLimbCount - 1);
			buffer1[uiLimbCount] = mpn_mul_1(buffer1, e1.limbs, uiLimbCount, e2);
			//mpn_redc_1(result.limbs, buffer1, modulus, uiLimbCount, modulus_inv);
			mpn_tdiv_qr(buffer2, result.limbs, 0L, buffer1, uiLimbCount + 1, modulus, uiLimbCount);
			//fout << "GFp(" << ToString(result) << "))\n";
		}

		static void Inv(Elem& result, const Elem& e1)
		{
			mp_limb_t buffer1[uiLimbCount + 1];
			mp_limb_t buffer2[uiLimbCount + 1];
			mp_limb_t buffer3[uiLimbCount + 1];
			mp_limb_t buffer4[3 * uiLimbCount];
			mp_limb_t buffer5[2 * uiLimbCount + 1];
			mp_size_t sn;
			memcpy(buffer1, e1.limbs, uiLimbCount * sizeof(mp_limb_t));
			buffer1[uiLimbCount] = 0;
			memcpy(buffer2, modulus, uiLimbCount * sizeof(mp_limb_t));
			buffer2[uiLimbCount] = 0;
			MPN_ZERO(buffer4, 2 * uiLimbCount);
			mpn_gcdext(buffer3, buffer4 + 2 * uiLimbCount, &sn, buffer1, uiLimbCount, buffer2, uiLimbCount);
			if (sn < 0)
			{
				mpn_sub(buffer4 + 2 * uiLimbCount, modulus, uiLimbCount, buffer4 + 2 * uiLimbCount, -sn);
			}
			else
			{
				while (sn < uiLimbCount)
				{
					(buffer4 + 2 * uiLimbCount)[sn++] = 0;
				}
			}
			mpn_tdiv_qr(buffer5, result.limbs, 0, buffer4, 3 * uiLimbCount, modulus, uiLimbCount);
		}

		static void Div(Elem& result, const Elem& e1, const Elem& e2)
		{
			// TODO! Too many buffers?
			mp_limb_t buffer1[uiLimbCount + 1];
			mp_limb_t buffer2[uiLimbCount + 1];
			mp_limb_t buffer3[uiLimbCount + 1];
			mp_limb_t buffer4[3 * uiLimbCount];
			mp_limb_t buffer5[2 * uiLimbCount + 1];
			mp_limb_t buffer6[uiLimbCount + 1];
			mp_limb_t buffer7[2 * uiLimbCount];
			mp_size_t sn;
			memcpy(buffer1, e2.limbs, uiLimbCount * sizeof(mp_limb_t));
			buffer1[uiLimbCount] = 0;
			memcpy(buffer2, modulus, uiLimbCount * sizeof(mp_limb_t));
			buffer2[uiLimbCount] = 0;
			MPN_ZERO(buffer4, 2 * uiLimbCount);
			mpn_gcdext(buffer3, buffer4 + 2 * uiLimbCount, &sn, buffer1, uiLimbCount, buffer2, uiLimbCount);
			if (sn < 0)
			{
				mpn_sub(buffer4 + 2 * uiLimbCount, modulus, uiLimbCount, buffer4 + 2 * uiLimbCount, -sn);
			}
			else
			{
				while (sn < uiLimbCount)
				{
					(buffer4 + 2 * uiLimbCount)[sn++] = 0;
				}
			}
			mpn_tdiv_qr(buffer5, buffer6, 0, buffer4, 3 * uiLimbCount, modulus, uiLimbCount);
			mpn_mul_n(buffer7, e1.limbs, buffer6, uiLimbCount);
			mpn_redc_1(result.limbs, buffer7, modulus, uiLimbCount, modulus_inv);
			if (mpn_cmp(result.limbs, modulus, uiLimbCount) >= 0)
			{
				mpn_sub_n(result.limbs, result.limbs, modulus, uiLimbCount);
			}
		}

		static void Exp(Elem& result, const Elem& e1, const Power& power)
		{
			power.Exponentiate<PrimeField<kModulus, kBitCount>, SetOne, Sqr, Copy, Mul>(result, e1);
		}

		static bool CheckEqual(const Elem& e1, const Elem& e2)
		{
			return (mpn_cmp(e1.limbs, e2.limbs, uiLimbCount) == 0);
		}

		static string ToString(const Elem& e1)
		{
			mp_limb_t buffer1[2 * uiLimbCount];
			mp_limb_t buffer2[uiLimbCount + 1];
			mp_limb_t buffer3[uiLimbCount + 1];
			mpn_mul_n(buffer1, e1.limbs, normalizer, uiLimbCount);
			mpn_tdiv_qr(buffer2, buffer3, 0, buffer1, 2 * uiLimbCount, modulus, uiLimbCount);

			unsigned char buffer[uiLimbCount * (sizeof(mp_limb_t) / 4 * 10) + 1];
			mp_size_t limb_count = uiLimbCount;
			while (limb_count > 0 && buffer3[limb_count - 1] == 0)
			{
				limb_count--;
			}
			if (limb_count == 0)
			{
				return "0";
			}

			mp_size_t char_number = mpn_get_str(buffer, 10, (mp_ptr)buffer3, uiLimbCount);
			mp_size_t offset = 0;
			while (buffer[offset] == 0)
			{
				offset++;
			}
			for (mp_size_t i = offset; i < char_number; i++)
			{
				buffer[i] += '0';
			}
			return string((char*)buffer + offset, char_number - offset);
		}

		static string ToString(const Elem* e, unsigned long long size)
		{
			string result = "[";
			for (unsigned long long i = 0; i < size; i++)
			{
				result += ToString(e[i]) + ", ";
			}
			result += "]";
			return result;
		}

		static void GetRandomElement(Elem& result)
		{
			if (rnd == nullptr)
				rnd = Random::GetInstance();
			mp_limb_t buffer1[2 * uiLimbCount];
			mp_limb_t buffer2[uiLimbCount + 1];
			rnd->GenBytes((unsigned char*)buffer1, 2 * uiLimbCount * sizeof(mp_limb_t));
			mpn_tdiv_qr(buffer2, result.limbs, 0, buffer1, 2 * uiLimbCount, modulus, uiLimbCount); // TODO! Will mpn_redc_1 be faster?
		}

		static void Init()
		{
			MPN_ZERO(modulus, uiLimbCount + 1);
			size_t str_len = strlen(kModulus);
			unsigned char* str_copy = (unsigned char*)malloc(str_len);
			for (size_t i = 0; i < str_len; i++)
				str_copy[i] = kModulus[i] - '0';
			mpn_set_str(modulus, str_copy, str_len, 10);
			free(str_copy);

			modlimb_invert(modulus_inv, modulus[0]);
			modulus_inv = 0 - modulus_inv;

			mp_limb_t buffer1[uiLimbCount + 1];
			mp_limb_t buffer2[2];
			mp_limb_t buffer3[uiLimbCount + 1];
			MPN_ZERO(buffer1, uiLimbCount);
			buffer1[uiLimbCount] = 1;
			mpn_tdiv_qr(buffer2, buffer3, 0, buffer1, uiLimbCount + 1, modulus, uiLimbCount);
			mp_limb_t buffer4[uiLimbCount + 1];
			mp_limb_t buffer5[uiLimbCount + 1];
			mp_limb_t buffer6[uiLimbCount + 1];
			mp_size_t sn;
			memcpy(buffer4, buffer3, uiLimbCount * sizeof(mp_limb_t));
			buffer4[uiLimbCount] = 0;
			memcpy(buffer5, modulus, uiLimbCount * sizeof(mp_limb_t));
			buffer5[uiLimbCount] = 0;
			mpn_gcdext(buffer6, normalizer, &sn, buffer4, uiLimbCount, buffer5, uiLimbCount);
			if (sn < 0)
			{
				mpn_sub(normalizer, modulus, uiLimbCount, normalizer, -sn);
			}
			else
			{
				while (sn < uiLimbCount)
				{
					normalizer[sn++] = 0;
				}
			}

			MPN_ZERO(zero.limbs, uiLimbCount);
			MPN_ZERO(one.limbs, uiLimbCount);
			one.limbs[0] = 1;
			one.Redcify();
		}

		static Elem zero, one;

	};

	template<const char* kModulus, mp_bitcnt_t kBitCount>
	mp_limb_t PrimeField<kModulus, kBitCount>::modulus[uiLimbCount + 1];

	template<const char* kModulus, mp_bitcnt_t kBitCount>
	mp_limb_t PrimeField<kModulus, kBitCount>::modulus_inv;

	template<const char* kModulus, mp_bitcnt_t kBitCount>
	mp_limb_t PrimeField<kModulus, kBitCount>::normalizer[uiLimbCount];

	template<const char* kModulus, mp_bitcnt_t kBitCount>
	typename PrimeField<kModulus, kBitCount>::Elem PrimeField<kModulus, kBitCount>::zero;

	template<const char* kModulus, mp_bitcnt_t kBitCount>
	typename PrimeField<kModulus, kBitCount>::Elem PrimeField<kModulus, kBitCount>::one;

	//template<const char* kModulus, mp_bitcnt_t kBitCount>
	//ofstream PrimeField<kModulus, kBitCount>::fout("test.txt");

	template<const char* kModulus, mp_bitcnt_t kBitCount>
	Random* PrimeField<kModulus, kBitCount>::rnd = nullptr;

}