#pragma once
#include "FiniteFieldPrime.h"

namespace ec_lib
{

	const PrimeField<example, 255>::Elem example3;

	template<class BaseField, const typename BaseField::Elem& kBaseElem>
	class CubicExtensionField
	{
	public:
		class Elem
		{
			friend class CubicExtensionField<BaseField, kBaseElem>;

		public:
			typename BaseField::Elem val0;
			typename BaseField::Elem val1;
			typename BaseField::Elem val2;

			// Special form
			bool has_special_form;
			bool val0_is_zero;
			bool val1_is_zero;
			bool val2_is_zero;

			template<class SomeFieldElem>
			void CreateThis(const SomeFieldElem& other)
			{
				typename BaseField::Elem tmp(other);
				BaseField::Copy(val0, tmp);
				BaseField::SetZero(val1);
				BaseField::SetZero(val2);
			}

			void CreateThis(const Elem& other)
			{
				BaseField::Copy(val0, other.val0);
				BaseField::Copy(val1, other.val1);
				BaseField::Copy(val2, other.val2);
			}

			template<class SomeFieldElem1, class SomeFieldElem2, class SomeFieldElem3>
			void CreateThis(const SomeFieldElem1 & v2, const SomeFieldElem2 & v1, const SomeFieldElem3 & v0)
			{
				typename BaseField::Elem tmp0(v0);
				typename BaseField::Elem tmp1(v1);
				typename BaseField::Elem tmp2(v2);
				BaseField::Copy(val0, tmp0);
				BaseField::Copy(val1, tmp1);
				BaseField::Copy(val2, tmp2);
			}

			void CreateThis(const typename BaseField::Elem & v2, const typename BaseField::Elem & v1, const typename BaseField::Elem & v0)
			{
				BaseField::Copy(val0, v0);
				BaseField::Copy(val1, v1);
				BaseField::Copy(val2, v2);
			}

		public:
			Elem()
				: has_special_form(false)
			{

			}

			template<class SomeFieldElem>
			Elem(const SomeFieldElem& other)
				: has_special_form(false)
			{
				CreateThis(other);
			}

			template<class SomeFieldElem1, class SomeFieldElem2, class SomeFieldElem3>
			Elem(const SomeFieldElem1 & v2, const SomeFieldElem2 & v1, const SomeFieldElem3 & v0)
				: has_special_form(false)
			{
				CreateThis(v2, v1, v0);
			}

			void ToSpecialForm()
			{
				val0.ToSpecialForm();
				val1.ToSpecialForm();
				val2.ToSpecialForm();
				val0_is_zero = BaseField::IsZero(val0);
				val1_is_zero = BaseField::IsZero(val1);
				val2_is_zero = BaseField::IsZero(val2);
				has_special_form = (val0_is_zero || val1_is_zero || val2_is_zero);
			}

			friend std::istream& operator>> (std::istream& stream, Elem& e)
			{
				stream >> e.val0 >> e.val1 >> e.val2;
				return stream;
			}

			friend std::ostream& operator<< (std::ostream& stream, const Elem& e)
			{
				stream << e.val0 << e.val1 << e.val2;
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
			return BaseField::IsZero(e1.val0) && BaseField::IsZero(e1.val1) && BaseField::IsZero(e1.val2);
		}

		static bool IsOne(const Elem& e1)
		{
			return BaseField::IsOne(e1.val0) && BaseField::IsZero(e1.val1) && BaseField::IsZero(e1.val2);
		}

		static void SetZero(Elem& e1)
		{
			BaseField::SetZero(e1.val0);
			BaseField::SetZero(e1.val1);
			BaseField::SetZero(e1.val2);
		}

		static void SetOne(Elem& e1)
		{
			BaseField::SetOne(e1.val0);
			BaseField::SetZero(e1.val1);
			BaseField::SetZero(e1.val2);
		}

		static void Copy(Elem& result, const Elem& e1)
		{
			BaseField::Copy(result.val0, e1.val0);
			BaseField::Copy(result.val1, e1.val1);
			BaseField::Copy(result.val2, e1.val2);
		}

		template<class SomeFieldElem>
		static void Add(Elem& result, const Elem& e1, const SomeFieldElem& e2)
		{
			BaseField::Add(result.val0, e1.val0, e2);
		}

		static void Add(Elem& result, const Elem& e1, const Elem& e2)
		{
			BaseField::Add(result.val0, e1.val0, e2.val0);
			BaseField::Add(result.val1, e1.val1, e2.val1);
			BaseField::Add(result.val2, e1.val2, e2.val2);
		}

		static void Add(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			BaseField::Add(result.val0, e1.val0, e2);
		}

		static void Dbl(Elem& result, const Elem& e1)
		{
			BaseField::Dbl(result.val0, e1.val0);
			BaseField::Dbl(result.val1, e1.val1);
			BaseField::Dbl(result.val2, e1.val2);
		}

		static void Neg(Elem& result, const Elem& e1)
		{
			BaseField::Neg(result.val0, e1.val0);
			BaseField::Neg(result.val1, e1.val1);
			BaseField::Neg(result.val2, e1.val2);
		}

		template<class SomeFieldElem>
		static void Sub(Elem& result, const Elem& e1, const SomeFieldElem& e2)
		{
			BaseField::Sub(result.val0, e1.val0, e2);
		}

		static void Sub(Elem& result, const Elem& e1, const Elem& e2)
		{
			BaseField::Sub(result.val0, e1.val0, e2.val0);
			BaseField::Sub(result.val1, e1.val1, e2.val1);
			BaseField::Sub(result.val2, e1.val2, e2.val2);
		}

		static void Sub(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			BaseField::Sub(result.val0, e1.val0, e2);
		}

		static void Sqr(Elem& result, const Elem& e1)
		{
			typename BaseField::Elem s0, s1, s2, s3, s4;
			BaseField::Sqr(s0, e1.val0);
			BaseField::Mul(s1, e1.val0, e1.val1);
			BaseField::Dbl(s1, s1);
			BaseField::Sub(s2, e1.val0, e1.val1);
			BaseField::Add(s2, s2, e1.val2);
			BaseField::Sqr(s2, s2);
			BaseField::Mul(s3, e1.val1, e1.val2);
			BaseField::Dbl(s3, s3);
			BaseField::Sqr(s4, e1.val2);
			BaseField::Add(result.val2, s1, s2);
			BaseField::Add(result.val2, result.val2, s3);
			BaseField::Sub(result.val2, result.val2, s0);
			BaseField::Sub(result.val2, result.val2, s4);
			BaseField::Mul(s4, s4, baseElem); // Special
			BaseField::Add(result.val1, s1, s4);
			BaseField::Mul(s3, s3, baseElem); // Special
			BaseField::Add(result.val0, s0, s3);
		}

		template<class SomeFieldElem>
		static void Mul(Elem& result, const Elem& e1, const SomeFieldElem& e2)
		{
			BaseField::Mul(result.val0, e1.val0, e2);
			BaseField::Mul(result.val1, e1.val1, e2);
			BaseField::Mul(result.val2, e1.val2, e2);
		}

		static void Mul(Elem& result, const Elem& e1, const Elem& e2)
		{
			if (!e2.has_special_form)
			{
				typename BaseField::Elem v0, v1, v2, tmp0, tmp1, tmp2, tmp;
				BaseField::Mul(v0, e1.val0, e2.val0);
				BaseField::Mul(v1, e1.val1, e2.val1);
				BaseField::Mul(v2, e1.val2, e2.val2);

				BaseField::Add(tmp0, e1.val1, e1.val2);
				BaseField::Add(tmp, e2.val1, e2.val2);
				BaseField::Mul(tmp0, tmp0, tmp);
				BaseField::Sub(tmp0, tmp0, v1);
				BaseField::Sub(tmp0, tmp0, v2);
				BaseField::Mul(tmp0, tmp0, baseElem); //Special

				BaseField::Add(tmp1, e1.val0, e1.val1);
				BaseField::Add(tmp, e2.val0, e2.val1);
				BaseField::Mul(tmp1, tmp1, tmp);
				BaseField::Sub(tmp1, tmp1, v0);
				BaseField::Sub(tmp1, tmp1, v1);

				BaseField::Add(tmp2, e1.val0, e1.val2);
				BaseField::Add(tmp, e2.val0, e2.val2);
				BaseField::Mul(tmp2, tmp2, tmp);
				BaseField::Sub(tmp2, tmp2, v0);
				BaseField::Add(tmp2, tmp2, v1);

				BaseField::Sub(result.val2, tmp2, v2);
				BaseField::Mul(v2, v2, baseElem); //Special
				BaseField::Add(result.val1, tmp1, v2);
				BaseField::Add(result.val0, v0, tmp0);
				return;
			}
			else
			{
				if (e2.val0_is_zero)
				{
					if (e2.val1_is_zero)
					{
						if (e2.val2_is_zero)
						{
							SetZero(result);
							return;
						}
						typename BaseField::Elem tmp;
						BaseField::Mul(tmp, e1.val2, e2.val2); // Special
						BaseField::Mul(result.val2, e1.val0, e2.val2); // Special
						BaseField::Mul(result.val0, e1.val1, e2.val2); // Special
						BaseField::Mul(result.val0, result.val0, baseElem); // Special
						BaseField::Mul(result.val1, tmp, baseElem); // Special
						return;
					}
					if (e2.val2_is_zero)
					{
						typename BaseField::Elem tmp;
						BaseField::Mul(tmp, e1.val2, e2.val1); // Special
						BaseField::Mul(result.val2, e1.val1, e2.val1); // Special
						BaseField::Mul(result.val1, e1.val0, e2.val1); // Special
						BaseField::Mul(result.val0, tmp, baseElem); // Special
						return;

					}
					typename BaseField::Elem v1, v2, tmp0, tmp1, tmp2, tmp;
					BaseField::Mul(v1, e1.val1, e2.val1);
					BaseField::Mul(v2, e1.val2, e2.val2);

					BaseField::Add(tmp0, e1.val1, e1.val2);
					BaseField::Add(tmp, e2.val1, e2.val2);
					BaseField::Mul(tmp0, tmp0, tmp);
					BaseField::Sub(tmp0, tmp0, v1);
					BaseField::Sub(tmp0, tmp0, v2);

					BaseField::Add(tmp1, e1.val0, e1.val1);
					BaseField::Mul(tmp1, tmp1, e2.val1);
					BaseField::Sub(tmp1, tmp1, v1);

					BaseField::Add(tmp2, e1.val0, e1.val2);
					BaseField::Mul(tmp2, tmp2, e2.val2);
					BaseField::Add(tmp2, tmp2, v1);

					BaseField::Sub(result.val2, tmp2, v2);
					BaseField::Mul(v2, v2, baseElem); //Special
					BaseField::Add(result.val1, tmp1, v2);
					BaseField::Mul(result.val0, tmp0, baseElem); //Special
					return;
				}
				if (e2.val1_is_zero)
				{
					if (e2.val2_is_zero)
					{
						BaseField::Mul(result.val2, e1.val2, e2.val0); // Special
						BaseField::Mul(result.val1, e1.val1, e2.val0); // Special
						BaseField::Mul(result.val0, e1.val0, e2.val0); // Special
						return;
					}
					typename BaseField::Elem v0, v2, tmp0, tmp1, tmp2, tmp;
					BaseField::Mul(v0, e1.val0, e2.val0);
					BaseField::Mul(v2, e1.val2, e2.val2);

					BaseField::Add(tmp0, e1.val1, e1.val2);
					BaseField::Mul(tmp0, tmp0, e2.val2);
					BaseField::Sub(tmp0, tmp0, v2);
					BaseField::Mul(tmp0, tmp0, baseElem); //Special

					BaseField::Add(tmp1, e1.val0, e1.val1);
					BaseField::Mul(tmp1, tmp1, e2.val0);
					BaseField::Sub(tmp1, tmp1, v0);

					BaseField::Add(tmp2, e1.val0, e1.val2);
					BaseField::Add(tmp, e2.val0, e2.val2);
					BaseField::Mul(tmp2, tmp2, tmp);
					BaseField::Sub(tmp2, tmp2, v0);

					BaseField::Sub(result.val2, tmp2, v2);
					BaseField::Mul(v2, v2, baseElem); //Special
					BaseField::Add(result.val1, tmp1, v2);
					BaseField::Add(result.val0, v0, tmp0);
					return;
				}
				if (e2.val2_is_zero)
				{
					typename BaseField::Elem v0, v1, tmp0, tmp1, tmp2, tmp;
					BaseField::Mul(v0, e1.val0, e2.val0);
					BaseField::Mul(v1, e1.val1, e2.val1);

					BaseField::Add(tmp0, e1.val1, e1.val2);
					BaseField::Mul(tmp0, tmp0, e2.val1);
					BaseField::Sub(tmp0, tmp0, v1);
					BaseField::Mul(tmp0, tmp0, baseElem); //Special

					BaseField::Add(tmp1, e1.val0, e1.val1);
					BaseField::Add(tmp, e2.val0, e2.val1);
					BaseField::Mul(tmp1, tmp1, tmp);
					BaseField::Sub(tmp1, tmp1, v0);

					BaseField::Add(tmp2, e1.val0, e1.val2);
					BaseField::Mul(tmp2, tmp2, e2.val0);
					BaseField::Sub(tmp2, tmp2, v0);

					BaseField::Add(result.val2, tmp2, v1);
					BaseField::Sub(result.val1, tmp1, v1);
					BaseField::Add(result.val0, v0, tmp0);
					return;
				}
			}
		}

		static void Mul(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			BaseField::Mul(result.val0, e1.val0, e2);
			BaseField::Mul(result.val1, e1.val1, e2);
			BaseField::Mul(result.val2, e1.val2, e2);
		}

		static void Inv(Elem& result, const Elem& e1) // TODO! Optimize
		{
			/*
				Elem result;
				BaseField::Elem b0 = val2.get_square().get_mult_spec(baseElem) - val0 * val1;
				BaseField::Elem b1 = val0 * val2 - val1.get_square();
				BaseField::Elem b00 = b0.get_square();
				BaseField::Elem b01 = b0 * b1;
				BaseField::Elem b11 = b1.get_square();
				BaseField::Elem v = ((b00 * b0) + (b11 * b1).get_mult_spec(baseElem)).get_inv();
				result.val2 = -(val2 * b01 + val1 * b11) * v;
				result.val1 = (val2 * b00 + val1 * b01) * v;
				result.val0 = (val2 * b11.get_mult_spec(baseElem) - val1 * b00) * v;
				return result;
			*/

			if (!BaseField::IsZero(e1.val2))
			{
				typename BaseField::Elem v, tmp0, tmp1, tmp2, b00, b01, b11;
				BaseField::Sqr(tmp0, e1.val2);
				BaseField::Mul(tmp0, tmp0, baseElem); // Special
				BaseField::Mul(tmp2, e1.val0, e1.val1);
				BaseField::Sub(tmp0, tmp0, tmp2);
				BaseField::Mul(tmp1, e1.val0, e1.val2);
				BaseField::Sqr(tmp2, e1.val1);
				BaseField::Sub(tmp1, tmp1, tmp2);
				BaseField::Sqr(b00, tmp0);
				BaseField::Mul(b01, tmp0, tmp1);
				BaseField::Sqr(b11, tmp1);

				BaseField::Mul(tmp0, tmp0, b00);
				BaseField::Mul(tmp1, tmp1, b11);
				BaseField::Mul(tmp1, tmp1, baseElem); //Special
				BaseField::Add(tmp0, tmp0, tmp1);
				BaseField::Inv(v, tmp0);

				BaseField::Mul(tmp2, e1.val2, b01);
				BaseField::Mul(tmp0, e1.val1, b11);
				BaseField::Add(tmp2, tmp2, tmp0);
				BaseField::Neg(tmp2, tmp2);

				BaseField::Mul(tmp1, e1.val2, b00);
				BaseField::Mul(tmp0, e1.val1, b01);
				BaseField::Add(tmp1, tmp1, tmp0);

				BaseField::Mul(tmp0, e1.val2, b11);
				BaseField::Mul(tmp0, tmp0, baseElem); // Special
				BaseField::Mul(b01, e1.val1, b00); // Don't need b01 anymore
				BaseField::Sub(tmp0, tmp0, b01);

				BaseField::Mul(result.val0, tmp0, v);
				BaseField::Mul(result.val1, tmp1, v);
				BaseField::Mul(result.val2, tmp2, v);
			}
			else if (!BaseField::IsZero(e1.val1))
			{
				typename BaseField::Elem v, tmp0, tmp1, tmp2, a00, a01, a11;
				BaseField::Sqr(a00, e1.val0);
				BaseField::Mul(a01, e1.val0, e1.val1);
				BaseField::Sqr(a11, e1.val1);

				BaseField::Mul(tmp0, e1.val0, a00);
				BaseField::Mul(tmp1, e1.val1, a11);
				BaseField::Mul(tmp1, tmp1, baseElem); //Special
				BaseField::Add(tmp0, tmp0, tmp1);
				BaseField::Inv(v, tmp0);

				BaseField::Mul(result.val0, a00, v);
				BaseField::Mul(result.val1, a01, v);
				BaseField::Neg(result.val1, result.val1);
				BaseField::Mul(result.val2, a11, v);
			}
			else // TODO! ORLY?
			{
				BaseField::Inv(result.val0, e1.val0);
				BaseField::SetZero(result.val1);
				BaseField::SetZero(result.val2);
			}
		}

		static void Div(Elem& result, const Elem& e1, const Elem& e2)
		{
			Elem e2_inv;
			Inv(e2_inv, e2);
			Mul(result, e1, e2_inv);
		}

		static void Exp(Elem& result, const Elem& e1, const Power& power)
		{
			power.Exponentiate<CubicExtensionField<BaseField, kBaseElem>, SetOne, Sqr, Copy, Mul>(result, e1);
		}

		static bool CheckEqual(const Elem& e1, const Elem& e2)
		{
			return BaseField::CheckEqual(e1.val0, e2.val0) &&
				BaseField::CheckEqual(e1.val1, e2.val1) &&
				BaseField::CheckEqual(e1.val2, e2.val2);
		}

		static string ToString(const Elem& e1)
		{
			return ("[" + BaseField::ToString(e1.val2) + ", " + BaseField::ToString(e1.val1) + ", " + BaseField::ToString(e1.val0) + "]");
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
			BaseField::GetRandomElement(result.val0);
			BaseField::GetRandomElement(result.val1);
			BaseField::GetRandomElement(result.val2);
		}

		static void Init()
		{
			BaseField::Init();
			BaseField::Copy(baseElem, kBaseElem);
			baseElem.ToSpecialForm();
			SetZero(zero);
			SetOne(one);
		}

		static typename BaseField::Elem baseElem;
		static Elem zero;
		static Elem one;
	};

	template<class BaseField, const typename BaseField::Elem& kBaseElem>
	typename BaseField::Elem CubicExtensionField<BaseField, kBaseElem>::baseElem;

	template<class BaseField, const typename BaseField::Elem& kBaseElem>
	typename CubicExtensionField<BaseField, kBaseElem>::Elem CubicExtensionField<BaseField, kBaseElem>::zero;

	template<class BaseField, const typename BaseField::Elem& kBaseElem>
	typename CubicExtensionField<BaseField, kBaseElem>::Elem CubicExtensionField<BaseField, kBaseElem>::one;

}