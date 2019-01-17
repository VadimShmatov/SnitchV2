#pragma once
#include "FiniteFieldPrime.h"

namespace ec_lib
{

	const PrimeField<example, 255>::Elem example2;

	template<class BaseField, const typename BaseField::Elem& kBaseElem>
	class QuadraticExtensionField
	{
	public:
		class Elem
		{
			friend class QuadraticExtensionField<BaseField, kBaseElem>;

		public:
			typename BaseField::Elem val0;
			typename BaseField::Elem val1;

			// Special form
			bool has_special_form;
			bool val0_is_zero;
			bool val1_is_zero;

			template<class SomeFieldElem>
			void CreateThis(const SomeFieldElem& other)
			{
				typename BaseField::Elem tmp(other);
				BaseField::Copy(val0, tmp);
				BaseField::SetZero(val1);
			}

			void CreateThis(const Elem& other)
			{
				BaseField::Copy(val0, other.val0);
				BaseField::Copy(val1, other.val1);
			}

			template<class SomeFieldElem1, class SomeFieldElem2>
			void CreateThis(const SomeFieldElem1 & v1, const SomeFieldElem2 & v0)
			{
				typename BaseField::Elem tmp0(v0);
				typename BaseField::Elem tmp1(v1);
				BaseField::Copy(val0, tmp0);
				BaseField::Copy(val1, tmp1);
			}

			void CreateThis(const typename BaseField::Elem & v1, const typename BaseField::Elem & v0)
			{
				BaseField::Copy(val0, v0);
				BaseField::Copy(val1, v1);
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

			template<class SomeFieldElem1, class SomeFieldElem2>
			Elem(const SomeFieldElem1 & v1, const SomeFieldElem2 & v0)
				: has_special_form(false)
			{
				CreateThis(v1, v0);
			}

			void ToSpecialForm()
			{
				val0.ToSpecialForm();
				val1.ToSpecialForm();
				val0_is_zero = BaseField::IsZero(val0);
				val1_is_zero = BaseField::IsZero(val1);
				has_special_form = (val0_is_zero || val1_is_zero);
			}

			friend std::istream& operator>> (std::istream& stream, Elem& e)
			{
				stream >> e.val0 >> e.val1;
				return stream;
			}

			friend std::ostream& operator<< (std::ostream& stream, const Elem& e)
			{
				stream << e.val0 << e.val1;
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
			return BaseField::IsZero(e1.val0) && BaseField::IsZero(e1.val1);
		}

		static bool IsOne(const Elem& e1)
		{
			return BaseField::IsOne(e1.val0) && BaseField::IsZero(e1.val1);
		}

		static void SetZero(Elem& e1)
		{
			BaseField::SetZero(e1.val0);
			BaseField::SetZero(e1.val1);
		}

		static void SetOne(Elem& e1)
		{
			BaseField::SetOne(e1.val0);
			BaseField::SetZero(e1.val1);
		}

		static void Copy(Elem& result, const Elem& e1)
		{
			BaseField::Copy(result.val0, e1.val0);
			BaseField::Copy(result.val1, e1.val1);
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
		}

		static void Add(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			BaseField::Add(result.val0, e1.val0, e2);
		}

		static void Dbl(Elem& result, const Elem& e1)
		{
			BaseField::Dbl(result.val0, e1.val0);
			BaseField::Dbl(result.val1, e1.val1);
		}

		static void Neg(Elem& result, const Elem& e1)
		{
			BaseField::Neg(result.val0, e1.val0);
			BaseField::Neg(result.val1, e1.val1);
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
		}

		static void Sub(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			BaseField::Sub(result.val0, e1.val0, e2);
		}

		static void Sqr(Elem& result, const Elem& e1)
		{
			typename BaseField::Elem v, tmp0, tmp1;
			BaseField::Mul(v, e1.val0, e1.val1);
			BaseField::Add(tmp0, e1.val0, e1.val1);
			BaseField::Mul(tmp1, e1.val1, baseElem); // Special
			BaseField::Add(tmp1, e1.val0, tmp1);
			BaseField::Mul(tmp0, tmp0, tmp1);
			BaseField::Sub(tmp0, tmp0, v);
			BaseField::Mul(tmp1, v, baseElem); // Special
			BaseField::Sub(result.val0, tmp0, tmp1);
			BaseField::Dbl(result.val1, v);
		}

		template<class SomeFieldElem>
		static void Mul(Elem& result, const Elem& e1, const SomeFieldElem& e2)
		{
			BaseField::Mul(result.val0, e1.val0, e2);
			BaseField::Mul(result.val1, e1.val1, e2);
		}

		static void Mul(Elem& result, const Elem& e1, const Elem& e2)
		{
			if (!e2.has_special_form)
			{
				typename BaseField::Elem v0, v1, tmp0, tmp1;
				BaseField::Mul(v0, e1.val0, e2.val0);
				BaseField::Mul(v1, e1.val1, e2.val1);
				BaseField::Add(tmp0, e1.val0, e1.val1);
				BaseField::Add(tmp1, e2.val0, e2.val1);
				BaseField::Mul(tmp0, tmp0, tmp1);
				BaseField::Sub(tmp0, tmp0, v0);
				BaseField::Sub(result.val1, tmp0, v1);
				BaseField::Mul(tmp1, v1, baseElem); // Special
				BaseField::Add(result.val0, tmp1, v0);
				return;
			}
			else
			{
				if (e2.val0_is_zero)
				{
					if (e2.val1_is_zero)
					{
						SetZero(result);
						return;
					}
					typename BaseField::Elem tmp0;
					BaseField::Mul(tmp0, e1.val1, e2.val1);
					BaseField::Mul(result.val1, e1.val0, e2.val1);
					BaseField::Mul(result.val0, tmp0, baseElem); // Special
					return;
				}
				BaseField::Mul(result.val0, e1.val0, e2.val0);
				BaseField::Mul(result.val1, e1.val1, e2.val0);
				return;
			}
		}

		static void Mul(Elem& result, const Elem& e1, const mp_limb_t e2)
		{
			BaseField::Mul(result.val0, e1.val0, e2);
			BaseField::Mul(result.val1, e1.val1, e2);
		}

		static void Inv(Elem& result, const Elem& e1)
		{
			if (!BaseField::IsZero(e1.val1))
			{
				typename BaseField::Elem tmp0, tmp1;
				BaseField::Sqr(tmp0, e1.val0);
				BaseField::Sqr(tmp1, e1.val1);
				BaseField::Mul(tmp1, tmp1, baseElem); // Special
				BaseField::Sub(tmp0, tmp0, tmp1);
				BaseField::Inv(tmp0, tmp0);
				BaseField::Mul(tmp1, e1.val1, tmp0);
				BaseField::Neg(result.val1, tmp1);
				BaseField::Mul(result.val0, e1.val0, tmp0);
			}
			else // TODO! ORLY?
			{
				BaseField::Inv(result.val0, e1.val0);
				BaseField::SetZero(result.val1);
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
			power.Exponentiate<QuadraticExtensionField<BaseField, kBaseElem>, SetOne, Sqr, Copy, Mul>(result, e1);
		}

		static bool CheckEqual(const Elem& e1, const Elem& e2)
		{
			return BaseField::CheckEqual(e1.val0, e2.val0) &&
				BaseField::CheckEqual(e1.val1, e2.val1);
		}

		static string ToString(const Elem& e1)
		{
			return ("[" + BaseField::ToString(e1.val1) + ", " + BaseField::ToString(e1.val0) + "]");
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
	typename BaseField::Elem QuadraticExtensionField<BaseField, kBaseElem>::baseElem;

	template<class BaseField, const typename BaseField::Elem& kBaseElem>
	typename QuadraticExtensionField<BaseField, kBaseElem>::Elem QuadraticExtensionField<BaseField, kBaseElem>::zero;

	template<class BaseField, const typename BaseField::Elem& kBaseElem>
	typename QuadraticExtensionField<BaseField, kBaseElem>::Elem QuadraticExtensionField<BaseField, kBaseElem>::one;

}