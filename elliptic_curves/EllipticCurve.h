#pragma once
#include "FiniteFieldPrime.h"
#include <vector>

namespace ec_lib
{

	const PrimeField<example, 255>::Elem a_example, b_example, x0_example, y0_example;

	template<class BaseField, const typename BaseField::Elem& kA, const typename BaseField::Elem& kB, const typename BaseField::Elem& kX0, const typename BaseField::Elem& kY0>
	class EllipticCurve
	{
	public:
		class Elem
		{
			friend class EllipticCurve<BaseField, kA, kB, kX0, kY0>;

		public:
			typename BaseField::Elem x;
			typename BaseField::Elem y;
			typename BaseField::Elem z;

		public:
			Elem()
			{

			}

			Elem(const Elem& other)
			{
				BaseField::Copy(x, other.x);
				BaseField::Copy(y, other.y);
				BaseField::Copy(z, other.z);
			}

			Elem(const typename BaseField::Elem & x_val, const typename BaseField::Elem & y_val)
			{
				BaseField::Copy(x, x_val);
				BaseField::Copy(y, y_val);
				BaseField::SetOne(z);
			}

			friend std::istream& operator>> (std::istream& stream, Elem& e)
			{
				unsigned char is_zero;
				stream.read((char*)&is_zero, 1);
				stream >> e.x >> e.y;
				if (is_zero)
				{
					BaseField::SetZero(e.z);
				}
				else
				{
					BaseField::SetOne(e.z);
				}
				return stream;
			}

			friend std::ostream& operator<< (std::ostream& stream, const Elem& e)
			{
				Elem tmp;
				Normalize(tmp, e);
				unsigned char is_zero = IsZero(e);
				stream.write((const char*)&is_zero, 1);
				stream << tmp.x << tmp.y;
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

		static typename BaseField::Elem a;
		static typename BaseField::Elem b;
		static /*typename*/ Elem zero;
		static /*typename*/ Elem one;

		static void Normalize(Elem& result, const Elem& p1)
		{
			if (BaseField::IsZero(p1.z))
			{
				SetZero(result);
				return;
			}
			if (BaseField::IsOne(p1.z))
			{
				Copy(result, p1);
				return;
			}
			typename BaseField::Elem z_inv, tmp;
			BaseField::Inv(z_inv, p1.z);
			BaseField::Sqr(tmp, z_inv);
			BaseField::Mul(result.x, p1.x, tmp);
			BaseField::Mul(tmp, tmp, z_inv);
			BaseField::Mul(result.y, p1.y, tmp);
			BaseField::SetOne(result.z);
		}

		static bool IsZero(const Elem& p1)
		{
			return BaseField::IsZero(p1.z);
		}

		static bool IsOne(const Elem& p1)
		{
			return CheckEqual(p1, one);
		}

		static void SetZero(Elem& result)
		{
			Copy(result, zero);
		}

		static void SetOne(Elem& result)
		{
			Copy(result, one);
		}

		static void Copy(Elem& result, const Elem& p1)
		{
			BaseField::Copy(result.x, p1.x);
			BaseField::Copy(result.y, p1.y);
			BaseField::Copy(result.z, p1.z);
		}

		static void Add(Elem& result, const Elem& p1, const Elem& p2) // TODO! Chudnovsky upgrade? TODO! Optimize memory usage
		{
			if (IsZero(p1))
			{
				Copy(result, p2);
				return;
			}
			if (IsZero(p2))
			{
				Copy(result, p1);
				return;
			}
			/*
				Z1Z1:=Z1^2;
				Z2Z2:=Z2^2;
				U1:=X1*Z2Z2;
				U2:=X2*Z1Z1;
				S1:=Y1*Z2*Z2Z2;
				S2:=Y2*Z1*Z1Z1;
				if (U1 == U2)
				  if (S1 != S2)
					return POINT_AT_INFINITY
				  else
					return POINT_DOUBLE(X1, Y1, Z1)
				H:=U2-U1;
				I:=(2*H)^2;
				J:=H*I;
				r:=2*(S2-S1);
				V:=U1*I;
				X3:=r^2-J-2*V;
				Y3:=r*(V-X3)-2*S1*J;
				Z3:=((Z1+Z2)^2-Z1Z1-Z2Z2)*H;
				return (X3, Y3, Z3)
			*/
			typename BaseField::Elem z1z1, z2z2, u1, u2, s1, s2, h, i, j, r, v;
			BaseField::Sqr(z2z2, p2.z);
			BaseField::Mul(s1, z2z2, p1.y);
			BaseField::Mul(u1, z2z2, p1.x);
			BaseField::Mul(s1, s1, p2.z);
			BaseField::Sqr(z1z1, p1.z);
			BaseField::Mul(s2, z1z1, p2.y);
			BaseField::Mul(u2, z1z1, p2.x);
			BaseField::Mul(s2, s2, p1.z);
			if (BaseField::CheckEqual(u1, u2))
			{
				if (BaseField::CheckEqual(s1, s2))
				{
					Dbl(result, p1);
					return;
				}
				else
				{
					SetZero(result);
					return;
				}
			}
			BaseField::Sub(h, u2, u1);
			BaseField::Dbl(i, h);
			BaseField::Sqr(i, i);
			BaseField::Mul(j, h, i);
			BaseField::Sub(r, s2, s1);
			BaseField::Dbl(r, r);
			BaseField::Mul(v, u1, i);

			BaseField::Add(u1, p1.z, p2.z);
			BaseField::Sqr(result.z, u1);
			BaseField::Sub(result.z, result.z, z1z1);
			BaseField::Sub(result.z, result.z, z2z2);
			BaseField::Mul(result.z, result.z, h);

			BaseField::Sqr(result.x, r);
			BaseField::Sub(result.x, result.x, j);
			BaseField::Dbl(u1, v);
			BaseField::Sub(result.x, result.x, u1);

			BaseField::Sub(result.y, v, result.x);
			BaseField::Mul(result.y, result.y, r);
			BaseField::Mul(u1, s1, j);
			BaseField::Dbl(u1, u1);
			BaseField::Sub(result.y, result.y, u1);
		}

		static void Neg(Elem& result, const Elem& p1)
		{
			BaseField::Copy(result.x, p1.x);
			BaseField::Neg(result.y, p1.y);
			BaseField::Copy(result.z, p1.z);
		}

		static void Sub(Elem& result, const Elem& p1, const Elem& p2) // TODO! Chudnovsky upgrade? TODO! Optimize memory usage
		{
			if (IsZero(p1))
			{
				Neg(result, p2);
				return;
			}
			if (IsZero(p2))
			{
				Copy(result, p1);
				return;
			}
			/*
				Z1Z1:=Z1^2;
				Z2Z2:=Z2^2;
				U1:=X1*Z2Z2;
				U2:=X2*Z1Z1;
				S1:=Y1*Z2*Z2Z2;
				S2:=-Y2*Z1*Z1Z1;
				if (U1 == U2)
				  if (S1 != S2)
					return POINT_AT_INFINITY
				  else
					return POINT_DOUBLE(X1, Y1, Z1)
				H:=U2-U1;
				I:=(2*H)^2;
				J:=H*I;
				r:=2*(S2-S1);
				V:=U1*I;
				X3:=r^2-J-2*V;
				Y3:=r*(V-X3)-2*S1*J;
				Z3:=((Z1+Z2)^2-Z1Z1-Z2Z2)*H;
				return (X3, Y3, Z3)
			*/
			typename BaseField::Elem z1z1, z2z2, u1, u2, s1, s2, h, i, j, r, v;
			BaseField::Sqr(z2z2, p2.z);
			BaseField::Mul(s1, z2z2, p1.y);
			BaseField::Mul(u1, z2z2, p1.x);
			BaseField::Mul(s1, s1, p2.z);
			BaseField::Sqr(z1z1, p1.z);
			BaseField::Mul(s2, z1z1, p2.y);
			BaseField::Neg(s2, s2);
			BaseField::Mul(u2, z1z1, p2.x);
			BaseField::Mul(s2, s2, p1.z);
			if (BaseField::CheckEqual(u1, u2))
			{
				if (BaseField::CheckEqual(s1, s2))
				{
					Dbl(result, p1);
					return;
				}
				else
				{
					SetZero(result);
					return;
				}
			}
			BaseField::Sub(h, u2, u1);
			BaseField::Dbl(i, h);
			BaseField::Sqr(i, i);
			BaseField::Mul(j, h, i);
			BaseField::Sub(r, s2, s1);
			BaseField::Dbl(r, r);
			BaseField::Mul(v, u1, i);

			BaseField::Add(u1, p1.z, p2.z);
			BaseField::Sqr(result.z, u1);
			BaseField::Sub(result.z, result.z, z1z1);
			BaseField::Sub(result.z, result.z, z2z2);
			BaseField::Mul(result.z, result.z, h);

			BaseField::Sqr(result.x, r);
			BaseField::Sub(result.x, result.x, j);
			BaseField::Dbl(u1, v);
			BaseField::Sub(result.x, result.x, u1);

			BaseField::Sub(result.y, v, result.x);
			BaseField::Mul(result.y, result.y, r);
			BaseField::Mul(u1, s1, j);
			BaseField::Dbl(u1, u1);
			BaseField::Sub(result.y, result.y, u1);
		}

		static void Dbl(Elem& result, const Elem& p1) // TODO! Chudnovsky upgrade? TODO! Optimize memory usage
		{
			if (IsZero(p1))
			{
				SetZero(result);
				return;
			}
			/*
				XX:=X1^2;
				YY:=Y1^2;
				YYYY:=YY^2;
				ZZ:=Z1^2;
				S:=2*((X1+YY)^2-XX-YYYY);
				M:=3*XX+a*ZZ^2;
				T:=M^2-2*S;
				X3:=T;
				Y3:=M*(S-T)-8*YYYY;
				Z3:=(Y1+Z1)^2-YY-ZZ;
				return (X3, Y3, Z3)
			*/
			typename BaseField::Elem xx, yy, yyyy, zz, s, m, t;
			BaseField::Sqr(xx, p1.x);
			BaseField::Sqr(yy, p1.y);
			BaseField::Sqr(yyyy, yy);
			BaseField::Sqr(zz, p1.z);
			BaseField::Add(s, p1.x, yy);
			BaseField::Sqr(s, s);
			BaseField::Sub(s, s, xx);
			BaseField::Sub(s, s, yyyy);
			BaseField::Dbl(s, s);
			BaseField::Dbl(m, xx);
			BaseField::Add(m, m, xx);
			BaseField::Sqr(t, zz);
			BaseField::Mul(t, t, a);
			BaseField::Add(m, m, t);
			BaseField::Sqr(t, m);
			BaseField::Dbl(xx, s);
			BaseField::Sub(result.x, t, xx);
			BaseField::Add(result.z, p1.z, p1.y);
			BaseField::Sqr(result.z, result.z);
			BaseField::Sub(result.z, result.z, yy);
			BaseField::Sub(result.z, result.z, zz);
			BaseField::Sub(result.y, s, result.x);
			BaseField::Mul(result.y, result.y, m);
			BaseField::Dbl(t, yyyy);
			BaseField::Dbl(t, t);
			BaseField::Dbl(t, t);
			BaseField::Sub(result.y, result.y, t);
		}

		static void Mul(Elem& result, const Elem& p1, const Power& power)
		{
			power.Exponentiate<EllipticCurve<BaseField, kA, kB, kX0, kY0>, SetZero, Dbl, Copy, Add>(result, p1);
		}

		static void MultiMul(Elem& result, Elem* elems, Power* powers, unsigned long long count)
		{
			Power::MultiExponentiate<EllipticCurve<BaseField, kA, kB, kX0, kY0>, SetZero, Dbl, Copy, Add>(result, elems, powers, count);
		}

		static bool CheckEqual(const Elem& p1, const Elem& p2)
		{
			typename BaseField::Elem u1, u2, s1, s2;
			BaseField::Sqr(u1, p2.z);
			BaseField::Mul(s1, u1, p1.y);
			BaseField::Mul(u1, u1, p1.x);
			BaseField::Mul(s1, s1, p2.z);
			BaseField::Sqr(u2, p1.z);
			BaseField::Mul(s2, u2, p2.y);
			BaseField::Mul(u2, u2, p2.x);
			BaseField::Mul(s2, s2, p1.z);
			return BaseField::CheckEqual(u1, u2) && BaseField::CheckEqual(s1, s2);
		}

		static string ToString(const Elem& p1)
		{
			if (BaseField::IsZero(p1.z))
			{
				return "POINT_AT_INFINITY";
			}
			else
			{
				//fout << "Before normalization: ( " << BaseField::ToString(p1.x) << " : " << BaseField::ToString(p1.y) << " : " << BaseField::ToString(p1.z) << " )\n" << flush;
				Elem tmp;
				Normalize(tmp, p1);
				return ("[" + BaseField::ToString(tmp.x) + ", " + BaseField::ToString(tmp.y) + "]");
			}
		}

		template<class ExtendedField, class ExtendedEllipticCurve>
		static void AddEx(Elem& result, typename ExtendedField::Elem& f_update, const Elem& p1, const Elem& p2, const typename ExtendedEllipticCurve::Elem& pQ)
		{
			typename BaseField::Elem z1z1, z1z1z1, z2z2, u1, u2, s1, s2, h, i, j, r, v;
			typename BaseField::Elem cy, cx, c0, tmp;
			BaseField::Sqr(z2z2, p2.z);
			BaseField::Mul(s1, z2z2, p1.y);
			BaseField::Mul(u1, z2z2, p1.x);
			BaseField::Mul(s1, s1, p2.z);
			BaseField::Sqr(z1z1, p1.z);
			BaseField::Mul(u2, z1z1, p2.x);
			BaseField::Mul(z1z1z1, z1z1, p1.z);
			BaseField::Mul(s2, z1z1z1, p2.y);
			if (BaseField::CheckEqual(u1, u2))
			{
				if (BaseField::CheckEqual(s1, s2))
				{
					DblEx<ExtendedField, ExtendedEllipticCurve>(result, f_update, p1, pQ);
					return;
				}
				else
				{
					BaseField::SetZero(cy);
					BaseField::Neg(cx, z1z1);
					BaseField::Copy(c0, p1.x);
					/*BaseField::Div(cx, cx, z1z1);
					BaseField::Div(c0, c0, z1z1);*/
					//fout << "cy = " << BaseField::ToString(cy) << '\n' << flush;
					//fout << "cx = " << BaseField::ToString(cx) << '\n' << flush;
					//fout << "c0 = " << BaseField::ToString(c0) << '\n' << flush;
					ExtendedEllipticCurve::EvaluateDivisor(f_update, pQ, cy, cx, c0);
					SetZero(result);
					//fout << "Miller: point after addition: " << ToString(result) << '\n' << flush;
					return;
				}
			}
			BaseField::Sub(h, u2, u1);
			BaseField::Dbl(i, h);
			BaseField::Sqr(i, i);
			BaseField::Mul(j, h, i);
			BaseField::Sub(r, s2, s1);
			// result_update calculation, part 1
			BaseField::Mul(cx, r, z1z1);
			BaseField::Mul(tmp, r, p1.x);
			// part 1 ends here
			BaseField::Dbl(r, r);
			BaseField::Mul(v, u1, i);

			BaseField::Mul(s2, h, p2.z); // s2 is free by now
			// result_update calculation, part 2
			BaseField::Mul(cy, s2, z1z1z1);
			BaseField::Mul(c0, s2, p1.y);
			BaseField::Sub(c0, c0, tmp);
			// part 2 ends here
			BaseField::Mul(result.z, s2, p1.z);
			BaseField::Dbl(result.z, result.z);

			BaseField::Sqr(result.x, r);
			BaseField::Sub(result.x, result.x, j);
			BaseField::Dbl(u1, v);
			BaseField::Sub(result.x, result.x, u1);

			BaseField::Sub(result.y, v, result.x);
			BaseField::Mul(result.y, result.y, r);
			BaseField::Mul(u1, s1, j);
			BaseField::Dbl(u1, u1);
			BaseField::Sub(result.y, result.y, u1);

			// result_update calculation, part 3
			/*BaseField::Div(cx, cx, z1z1z1);
			BaseField::Div(c0, c0, z1z1z1);
			BaseField::Div(cy, cy, z1z1z1);
			BaseField::Div(cx, cx, z1z1);
			BaseField::Div(c0, c0, z1z1);
			BaseField::Div(cy, cy, z1z1);
			BaseField::Div(cx, cx, z2z2);
			BaseField::Div(c0, c0, z2z2);
			BaseField::Div(cy, cy, z2z2);
			BaseField::Div(cx, cx, p2.z);
			BaseField::Div(c0, c0, p2.z);
			BaseField::Div(cy, cy, p2.z);*/
			ExtendedEllipticCurve::EvaluateDivisor(f_update, pQ, cy, cx, c0);
			// part 3 ends here

			//fout << "Miller: point after addition: " << ToString(result) << '\n' << flush;
			//fout << "cy = " << BaseField::ToString(cy) << '\n' << flush;
			//fout << "cx = " << BaseField::ToString(cx) << '\n' << flush;
			//fout << "c0 = " << BaseField::ToString(c0) << '\n' << flush;
		}

		template<class ExtendedField, class ExtendedEllipticCurve>
		static void DblEx(Elem& result, typename ExtendedField::Elem& f_update, const Elem& p1, const typename ExtendedEllipticCurve::Elem& pQ)
		{
			typename BaseField::Elem xx, yy, yyyy, zz, s, m, t;
			typename BaseField::Elem cy, cx, c0, tmp;
			BaseField::Sqr(xx, p1.x);
			BaseField::Sqr(yy, p1.y);
			BaseField::Sqr(yyyy, yy);
			BaseField::Sqr(zz, p1.z);
			BaseField::Add(s, p1.x, yy);
			BaseField::Sqr(s, s);
			BaseField::Sub(s, s, xx);
			BaseField::Sub(s, s, yyyy);
			BaseField::Dbl(s, s);
			BaseField::Dbl(m, xx);
			BaseField::Add(m, m, xx);
			BaseField::Sqr(t, zz);
			BaseField::Mul(t, t, a);
			BaseField::Add(m, m, t);
			// result_update calculation, part 1
			BaseField::Mul(cx, m, zz);
			BaseField::Dbl(c0, yy);
			BaseField::Mul(tmp, p1.x, m);
			BaseField::Sub(c0, c0, tmp);
			// part 1 ends here
			BaseField::Sqr(t, m);
			BaseField::Dbl(xx, s);
			BaseField::Sub(result.x, t, xx);
			BaseField::Add(result.z, p1.z, p1.y);
			BaseField::Sqr(result.z, result.z);
			BaseField::Sub(result.z, result.z, yy);
			BaseField::Sub(result.z, result.z, zz);
			// result_update calculation, part 2
			BaseField::Mul(cy, zz, result.z);
			// part 2 ends here
			BaseField::Sub(result.y, s, result.x);
			BaseField::Mul(result.y, result.y, m);
			BaseField::Dbl(t, yyyy);
			BaseField::Dbl(t, t);
			BaseField::Dbl(t, t);
			BaseField::Sub(result.y, result.y, t);

			// result_update calculation, part 3
			/*BaseField::Div(cx, cx, zz);
			BaseField::Div(c0, c0, zz);
			BaseField::Div(cy, cy, zz);
			BaseField::Div(cx, cx, zz);
			BaseField::Div(c0, c0, zz);
			BaseField::Div(cy, cy, zz);
			BaseField::Div(cx, cx, zz);
			BaseField::Div(c0, c0, zz);
			BaseField::Div(cy, cy, zz);*/
			ExtendedEllipticCurve::EvaluateDivisor(f_update, pQ, cy, cx, c0);
			// part 3 ends here

			//fout << "Miller: point after doubling: " << ToString(result) << '\n' << flush;
			//fout << "cy = " << BaseField::ToString(cy) << '\n' << flush;
			//fout << "cx = " << BaseField::ToString(cx) << '\n' << flush;
			//fout << "c0 = " << BaseField::ToString(c0) << '\n' << flush;
		}

		template<class SomeFieldElem>
		static void EvaluateDivisor(typename BaseField::Elem& result, const Elem& pQ, const SomeFieldElem& cy, const SomeFieldElem& cx, const SomeFieldElem& c0)
		{
			typename BaseField::Elem tmp;
			BaseField::Mul(result, pQ.y, cy);
			BaseField::Mul(tmp, pQ.x, cx);
			BaseField::Sub(result, result, tmp);
			BaseField::Sub(result, result, c0);
		}

		static void Init()
		{
			BaseField::Init();
			BaseField::Copy(a, kA);
			a.ToSpecialForm();
			BaseField::Copy(b, kA);
			b.ToSpecialForm();
			BaseField::SetOne(zero.x);
			BaseField::SetOne(zero.y);
			BaseField::SetZero(zero.z);
			BaseField::Copy(one.x, kX0);
			BaseField::Copy(one.y, kY0);
			BaseField::SetOne(one.z);
		}

		//static ofstream fout;
	};

	template<class BaseField, const typename BaseField::Elem& kA, const typename BaseField::Elem& kB, const typename BaseField::Elem& kX0, const typename BaseField::Elem& kY0>
	typename BaseField::Elem EllipticCurve<BaseField, kA, kB, kX0, kY0>::a;

	template<class BaseField, const typename BaseField::Elem& kA, const typename BaseField::Elem& kB, const typename BaseField::Elem& kX0, const typename BaseField::Elem& kY0>
	typename BaseField::Elem EllipticCurve<BaseField, kA, kB, kX0, kY0>::b;

	template<class BaseField, const typename BaseField::Elem& kA, const typename BaseField::Elem& kB, const typename BaseField::Elem& kX0, const typename BaseField::Elem& kY0>
	typename EllipticCurve<BaseField, kA, kB, kX0, kY0>::Elem EllipticCurve<BaseField, kA, kB, kX0, kY0>::zero;

	template<class BaseField, const typename BaseField::Elem& kA, const typename BaseField::Elem& kB, const typename BaseField::Elem& kX0, const typename BaseField::Elem& kY0>
	typename EllipticCurve<BaseField, kA, kB, kX0, kY0>::Elem EllipticCurve<BaseField, kA, kB, kX0, kY0>::one;

	//template<class BaseField, const typename BaseField::Elem& kA, const typename BaseField::Elem& kB, const typename BaseField::Elem& kX0, const typename BaseField::Elem& kY0>
	//ofstream EllipticCurve<BaseField, kA, kB, kX0, kY0>::fout;

	template<class EllipticCurve, class ExtendedField, class ExtendedEllipticCurve, const char* kR, const char* kQ, const mp_limb_t kK>
	class TatePairingEvaluator
	{
	public:

		unsigned char* r_bits;
		int r_bits_length;
		Power final_exp;

		void TatePairing(typename ExtendedField::Elem& result, const typename EllipticCurve::Elem& P, const typename ExtendedEllipticCurve::Elem& Q)
		{
			//EllipticCurve::fout << "Computing pairing...\n" << flush;
			//EllipticCurve::fout << "Point 1: " << EllipticCurve::ToString(P) << '\n' << flush;
			//EllipticCurve::fout << "Point 2: " << ExtendedEllipticCurve::ToString(Q) << '\n' << flush;
			//EllipticCurve::fout << "Computing pairing...\n" << flush;
			typename EllipticCurve::Elem R;
			EllipticCurve::Copy(R, P);
			typename ExtendedField::Elem result_update;
			ExtendedField::SetOne(result);
			typename ExtendedEllipticCurve::Elem Q_norm;
			ExtendedEllipticCurve::Normalize(Q_norm, Q);
			for (int i = r_bits_length - 2; i >= 0; i--)
			{
				ExtendedField::Sqr(result, result);
				EllipticCurve::template DblEx<ExtendedField, ExtendedEllipticCurve>(R, result_update, R, Q_norm);
				//EllipticCurve::fout << "f_update value: " << ExtendedField::ToString(result_update) << '\n' << flush;
				ExtendedField::Mul(result, result, result_update);
				//EllipticCurve::fout << "New f value: " << ExtendedField::ToString(result) << '\n' << flush;
				if (r_bits[i])
				{
					EllipticCurve::template AddEx<ExtendedField, ExtendedEllipticCurve>(R, result_update, R, P, Q_norm);
					//EllipticCurve::fout << "f_update value: " << ExtendedField::ToString(result_update) << '\n' << flush;
					ExtendedField::Mul(result, result, result_update);
					//EllipticCurve::fout << "New f value: " << ExtendedField::ToString(result) << '\n' << flush;
				}
			}
			ExtendedField::Exp(result, result, final_exp); // TODO! Optimize.
			//EllipticCurve::fout << "Final result: " << ExtendedField::ToString(result) << '\n' << flush;
		}

		TatePairingEvaluator()
			: r_bits(nullptr)
		{
			mpz_t r, q;
			mpz_init_set_str(r, kR, 10);
			mpz_init_set_str(q, kQ, 10);
			r_bits_length = mpz_sizeinbase(r, 2);
			r_bits = (unsigned char*)malloc(r_bits_length * sizeof(unsigned char));
			for (mp_bitcnt_t i = 0; i < r_bits_length; i++)
			{
				r_bits[i] = mpz_tstbit(r, i);
			}
			mpz_pow_ui(q, q, kK);
			mpz_sub_ui(q, q, 1);
			mpz_divexact(q, q, r);
			final_exp = Power(mpz_limbs_read(q), mpz_size(q));
			//final_exp = Power("20781675289058557957622819280");
			mpz_clear(r);
			mpz_clear(q);
		}

		~TatePairingEvaluator()
		{
			if (r_bits != nullptr)
			{
				free(r_bits);
				r_bits = nullptr;
			}
		}
	};

}