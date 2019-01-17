#pragma once
#include "EllipticCurve.h"

namespace ec_lib
{

	template <class TwistedEllipticCurve, class TwistedBaseField, class ExtendedEllipticCurve, class ExtendedBaseField, const typename ExtendedBaseField::Elem& kOmega>
	class EllipticCurveTwister
	{
		static typename ExtendedBaseField::Elem omega;
		static typename ExtendedBaseField::Elem omega_2;
		static typename ExtendedBaseField::Elem omega_3;
		static typename ExtendedBaseField::Elem omega_inv;
		static typename ExtendedBaseField::Elem omega_inv_2;
		static typename ExtendedBaseField::Elem omega_inv_3;

	public:
		static void Twist(typename TwistedEllipticCurve::Elem& result, const typename ExtendedEllipticCurve::Elem& e1)
		{
			typename ExtendedEllipticCurve::Elem tmp;
			ExtendedEllipticCurve::Normalize(tmp, e1);
			typename ExtendedBaseField::Elem x, y;
			ExtendedBaseField::Mul(x, e1.x, omega_2);
			ExtendedBaseField::Mul(y, e1.y, omega_3);
			TwistedBaseField::Copy(result.x, x.val0.val0);
			TwistedBaseField::Copy(result.y, y.val0.val0);
			TwistedBaseField::SetOne(result.z);
		}

		static void Untwist(typename ExtendedEllipticCurve::Elem& result, const typename TwistedEllipticCurve::Elem& e1)
		{
			typename TwistedEllipticCurve::Elem tmp;
			TwistedEllipticCurve::Normalize(tmp, e1);
			ExtendedBaseField::Copy(result.x, tmp.x);
			ExtendedBaseField::Copy(result.y, tmp.y);
			ExtendedBaseField::Mul(result.x, result.x, omega_inv_2);
			ExtendedBaseField::Mul(result.y, result.y, omega_inv_3);
			ExtendedBaseField::SetOne(result.z);
		}

		static void Init()
		{
			ExtendedBaseField::Copy(omega, kOmega);
			ExtendedBaseField::Sqr(omega_2, omega);
			omega_2.ToSpecialForm();
			ExtendedBaseField::Mul(omega_3, omega_2, omega);
			omega_3.ToSpecialForm();
			ExtendedBaseField::Inv(omega_inv, omega);
			ExtendedBaseField::Sqr(omega_inv_2, omega_inv);
			omega_inv_2.ToSpecialForm();
			ExtendedBaseField::Mul(omega_inv_3, omega_inv_2, omega_inv);
			omega_inv_3.ToSpecialForm();
		}
	};

	template <class TwistedEllipticCurve, class TwistedBaseField, class ExtendedEllipticCurve, class ExtendedBaseField, const typename ExtendedBaseField::Elem& kOmega>
	typename ExtendedBaseField::Elem EllipticCurveTwister<TwistedEllipticCurve, TwistedBaseField, ExtendedEllipticCurve, ExtendedBaseField, kOmega>::omega;

	template <class TwistedEllipticCurve, class TwistedBaseField, class ExtendedEllipticCurve, class ExtendedBaseField, const typename ExtendedBaseField::Elem& kOmega>
	typename ExtendedBaseField::Elem EllipticCurveTwister<TwistedEllipticCurve, TwistedBaseField, ExtendedEllipticCurve, ExtendedBaseField, kOmega>::omega_2;

	template <class TwistedEllipticCurve, class TwistedBaseField, class ExtendedEllipticCurve, class ExtendedBaseField, const typename ExtendedBaseField::Elem& kOmega>
	typename ExtendedBaseField::Elem EllipticCurveTwister<TwistedEllipticCurve, TwistedBaseField, ExtendedEllipticCurve, ExtendedBaseField, kOmega>::omega_3;

	template <class TwistedEllipticCurve, class TwistedBaseField, class ExtendedEllipticCurve, class ExtendedBaseField, const typename ExtendedBaseField::Elem& kOmega>
	typename ExtendedBaseField::Elem EllipticCurveTwister<TwistedEllipticCurve, TwistedBaseField, ExtendedEllipticCurve, ExtendedBaseField, kOmega>::omega_inv;

	template <class TwistedEllipticCurve, class TwistedBaseField, class ExtendedEllipticCurve, class ExtendedBaseField, const typename ExtendedBaseField::Elem& kOmega>
	typename ExtendedBaseField::Elem EllipticCurveTwister<TwistedEllipticCurve, TwistedBaseField, ExtendedEllipticCurve, ExtendedBaseField, kOmega>::omega_inv_2;

	template <class TwistedEllipticCurve, class TwistedBaseField, class ExtendedEllipticCurve, class ExtendedBaseField, const typename ExtendedBaseField::Elem& kOmega>
	typename ExtendedBaseField::Elem EllipticCurveTwister<TwistedEllipticCurve, TwistedBaseField, ExtendedEllipticCurve, ExtendedBaseField, kOmega>::omega_inv_3;

}