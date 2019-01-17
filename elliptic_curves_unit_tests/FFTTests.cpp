#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fft_tests
{
	const char p[] = "49555733732057424293751983680609532697145113652152511821339778989694553948161";
	typedef PrimeField<p, 255> Fp;
	const StaticInvoker<Fp> Fp_initializer;
	typedef FFTEvaluator<Fp, p, 8> E8;
	const StaticInvoker<E8> E8_initializer;
	typedef FFTEvaluator<Fp, p, 9> E9;
	const StaticInvoker<E9> E9_initializer;
	typedef FFTEvaluator<Fp, p, 10> E10;
	const StaticInvoker<E10> E10_initializer;
	typedef FFTEvaluator<Fp, p, 12> E12;
	const StaticInvoker<E12> E12_initializer;
	typedef FFTEvaluator<Fp, p, 15> E15;
	const StaticInvoker<E15> E15_initializer;

	TEST_CLASS(FFTTests)
	{
		TEST_METHOD(BasicTest_E8)
		{
			Fp::Elem* e = new Fp::Elem[E8::polynomial_size];
			for (int i = 0; i < 8; i++)
			{
				Fp::GetRandomElement(e[i]);
			}
			for (int i = 8; i < E8::polynomial_size; i++)
			{
				Fp::SetZero(e[i]);
			}
			Fp::Elem* res1 = new Fp::Elem[E8::polynomial_size];
			Fp::Elem* res2 = new Fp::Elem[E8::polynomial_size];
			E8::FFT(res1, e);
			E8::iFFT(res2, res1);
			Assert::AreEqual(Fp::ToString(e, E8::polynomial_size), Fp::ToString(res2, E8::polynomial_size));
		}

		TEST_METHOD(BasicTest_E9)
		{
			Fp::Elem* e = new Fp::Elem[E9::polynomial_size];
			for (int i = 0; i < 9; i++)
			{
				Fp::GetRandomElement(e[i]);
			}
			for (int i = 9; i < E9::polynomial_size; i++)
			{
				Fp::SetZero(e[i]);
			}
			Fp::Elem* res1 = new Fp::Elem[E9::polynomial_size];
			Fp::Elem* res2 = new Fp::Elem[E9::polynomial_size];
			E9::FFT(res1, e);
			E9::iFFT(res2, res1);
			Assert::AreEqual(Fp::ToString(e, E9::polynomial_size), Fp::ToString(res2, E9::polynomial_size));
		}

		TEST_METHOD(BasicTest_E10)
		{
			Fp::Elem* e = new Fp::Elem[E10::polynomial_size];
			for (int i = 0; i < 10; i++)
			{
				Fp::GetRandomElement(e[i]);
			}
			for (int i = 10; i < E10::polynomial_size; i++)
			{
				Fp::SetZero(e[i]);
			}
			Fp::Elem* res1 = new Fp::Elem[E10::polynomial_size];
			Fp::Elem* res2 = new Fp::Elem[E10::polynomial_size];
			E10::FFT(res1, e);
			E10::iFFT(res2, res1);
			Assert::AreEqual(Fp::ToString(e, E10::polynomial_size), Fp::ToString(res2, E10::polynomial_size));
		}

		TEST_METHOD(BasicTest_E12)
		{
			Fp::Elem* e = new Fp::Elem[E12::polynomial_size];
			for (int i = 0; i < 12; i++)
			{
				Fp::GetRandomElement(e[i]);
			}
			for (int i = 12; i < E12::polynomial_size; i++)
			{
				Fp::SetZero(e[i]);
			}
			Fp::Elem* res1 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* res2 = new Fp::Elem[E12::polynomial_size];
			E12::FFT(res1, e);
			E12::iFFT(res2, res1);
			Assert::AreEqual(Fp::ToString(e, E12::polynomial_size), Fp::ToString(res2, E12::polynomial_size));
		}

		TEST_METHOD(BasicTest_E15)
		{
			Fp::Elem* e = new Fp::Elem[E15::polynomial_size];
			for (int i = 0; i < 15; i++)
			{
				Fp::GetRandomElement(e[i]);
			}
			for (int i = 15; i < E15::polynomial_size; i++)
			{
				Fp::SetZero(e[i]);
			}
			Fp::Elem* res1 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* res2 = new Fp::Elem[E15::polynomial_size];
			E15::FFT(res1, e);
			E15::iFFT(res2, res1);
			Assert::AreEqual(Fp::ToString(e, E15::polynomial_size), Fp::ToString(res2, E15::polynomial_size));
		}

		TEST_METHOD(Multiplication_E12)
		{
			Fp::Elem* e1 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* e2 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* e3 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem tmp;

			for (int i = 0; i < 5; i++)
			{
				Fp::GetRandomElement(e1[i]);
			}
			for (int i = 5; i < E12::polynomial_size; i++)
			{
				Fp::SetZero(e1[i]);
			}

			for (int i = 0; i < 6; i++)
			{
				Fp::GetRandomElement(e2[i]);
			}
			for (int i = 6; i < E12::polynomial_size; i++)
			{
				Fp::SetZero(e2[i]);
			}

			for (int i = 0; i < E12::polynomial_size; i++)
			{
				Fp::SetZero(e3[i]);
			}
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					Fp::Mul(tmp, e1[i], e2[j]);
					Fp::Add(e3[i + j], e3[i + j], tmp);
				}
			}

			Fp::Elem* tmp1 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* tmp2 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* tmp3 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* res = new Fp::Elem[E12::polynomial_size];
			E12::FFT(tmp1, e1);
			E12::FFT(tmp2, e2);
			for (int i = 0; i < E12::polynomial_size; i++)
			{
				Fp::Mul(tmp3[i], tmp1[i], tmp2[i]);
			}
			E12::iFFT(res, tmp3);
			Assert::AreEqual(Fp::ToString(e3, E12::polynomial_size), Fp::ToString(res, E12::polynomial_size));
		}

		TEST_METHOD(Multiplication_E15)
		{
			Fp::Elem* e1 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* e2 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* e3 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem tmp;

			for (int i = 0; i < 7; i++)
			{
				Fp::GetRandomElement(e1[i]);
			}
			for (int i = 7; i < E15::polynomial_size; i++)
			{
				Fp::SetZero(e1[i]);
			}

			for (int i = 0; i < 7; i++)
			{
				Fp::GetRandomElement(e2[i]);
			}
			for (int i = 7; i < E15::polynomial_size; i++)
			{
				Fp::SetZero(e2[i]);
			}

			for (int i = 0; i < E15::polynomial_size; i++)
			{
				Fp::SetZero(e3[i]);
			}
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					Fp::Mul(tmp, e1[i], e2[j]);
					Fp::Add(e3[i + j], e3[i + j], tmp);
				}
			}

			Fp::Elem* tmp1 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* tmp2 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* tmp3 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* res = new Fp::Elem[E15::polynomial_size];
			E15::FFT(tmp1, e1);
			E15::FFT(tmp2, e2);
			for (int i = 0; i < E15::polynomial_size; i++)
			{
				Fp::Mul(tmp3[i], tmp1[i], tmp2[i]);
			}
			E15::iFFT(res, tmp3);
			Assert::AreEqual(Fp::ToString(e3, E15::polynomial_size), Fp::ToString(res, E15::polynomial_size));
		}

		TEST_METHOD(Multiplication_coset_E12)
		{
			Fp::Elem* e1 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* e2 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* e3 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem tmp;

			for (int i = 0; i < 5; i++)
			{
				Fp::GetRandomElement(e1[i]);
			}
			for (int i = 5; i < E12::polynomial_size; i++)
			{
				Fp::SetZero(e1[i]);
			}

			for (int i = 0; i < 6; i++)
			{
				Fp::GetRandomElement(e2[i]);
			}
			for (int i = 6; i < E12::polynomial_size; i++)
			{
				Fp::SetZero(e2[i]);
			}

			for (int i = 0; i < E12::polynomial_size; i++)
			{
				Fp::SetZero(e3[i]);
			}
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					Fp::Mul(tmp, e1[i], e2[j]);
					Fp::Add(e3[i + j], e3[i + j], tmp);
				}
			}

			Fp::Elem* tmp1 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* tmp2 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* tmp3 = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* res = new Fp::Elem[E12::polynomial_size];

			Fp::Elem coset_elem("3");
			Fp::Elem coset_elem_inv;
			Fp::Inv(coset_elem_inv, coset_elem);

			E12::MoveToCoset(tmp1, tmp1, coset_elem);
			E12::MoveToCoset(tmp2, tmp2, coset_elem);

			E12::FFT(tmp1, e1);
			E12::FFT(tmp2, e2);
			for (int i = 0; i < E12::polynomial_size; i++)
			{
				Fp::Mul(tmp3[i], tmp1[i], tmp2[i]);
			}
			E12::iFFT(res, tmp3);

			E12::MoveToCoset(tmp3, tmp3, coset_elem_inv);

			Assert::AreEqual(Fp::ToString(e3, E12::polynomial_size), Fp::ToString(res, E12::polynomial_size));
		}

		TEST_METHOD(Multiplication_coset_E15)
		{
			Fp::Elem* e1 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* e2 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* e3 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem tmp;

			for (int i = 0; i < 7; i++)
			{
				Fp::GetRandomElement(e1[i]);
			}
			for (int i = 7; i < E15::polynomial_size; i++)
			{
				Fp::SetZero(e1[i]);
			}

			for (int i = 0; i < 7; i++)
			{
				Fp::GetRandomElement(e2[i]);
			}
			for (int i = 7; i < E15::polynomial_size; i++)
			{
				Fp::SetZero(e2[i]);
			}

			for (int i = 0; i < E15::polynomial_size; i++)
			{
				Fp::SetZero(e3[i]);
			}
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					Fp::Mul(tmp, e1[i], e2[j]);
					Fp::Add(e3[i + j], e3[i + j], tmp);
				}
			}

			Fp::Elem* tmp1 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* tmp2 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* tmp3 = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* res = new Fp::Elem[E15::polynomial_size];

			Fp::Elem coset_elem("3");
			Fp::Elem coset_elem_inv;
			Fp::Inv(coset_elem_inv, coset_elem);

			E15::MoveToCoset(tmp1, tmp1, coset_elem);
			E15::MoveToCoset(tmp2, tmp2, coset_elem);

			E15::FFT(tmp1, e1);
			E15::FFT(tmp2, e2);
			for (int i = 0; i < E15::polynomial_size; i++)
			{
				Fp::Mul(tmp3[i], tmp1[i], tmp2[i]);
			}
			E15::iFFT(res, tmp3);

			E15::MoveToCoset(tmp3, tmp3, coset_elem_inv);

			Assert::AreEqual(Fp::ToString(e3, E15::polynomial_size), Fp::ToString(res, E15::polynomial_size));
		}

		TEST_METHOD(Lagrange_E9)
		{
			Fp::Elem* e = new Fp::Elem[E9::polynomial_size];
			Fp::Elem* res = new Fp::Elem[E9::polynomial_size];
			Fp::Elem* res1 = new Fp::Elem[E9::polynomial_size];

			Fp::Elem target, tmp;
			Fp::GetRandomElement(target);

			E9::EvaluateLagrangePolynomials(res, target);

			for (int i = 0; i < E9::polynomial_size; i++)
			{
				E9::GetNthPoint(e[i], i);
			}

			for (int i = 0; i < E9::polynomial_size; i++)
			{
				Fp::SetOne(res1[i]);
				for (int j = 0; j < E9::polynomial_size; j++)
				{
					if (i != j)
					{
						Fp::Sub(tmp, target, e[j]);
						Fp::Mul(res1[i], res1[i], tmp);
						Fp::Sub(tmp, e[i], e[j]);
						Fp::Div(res1[i], res1[i], tmp);
					}
				}
			}

			Assert::AreEqual(Fp::ToString(res1, E9::polynomial_size), Fp::ToString(res, E9::polynomial_size));
		}

		TEST_METHOD(Lagrange_E12)
		{
			Fp::Elem* e = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* res = new Fp::Elem[E12::polynomial_size];
			Fp::Elem* res1 = new Fp::Elem[E12::polynomial_size];

			Fp::Elem target, tmp;
			Fp::GetRandomElement(target);

			E12::EvaluateLagrangePolynomials(res, target);

			for (int i = 0; i < E12::polynomial_size; i++)
			{
				E12::GetNthPoint(e[i], i);
			}

			for (int i = 0; i < E12::polynomial_size; i++)
			{
				Fp::SetOne(res1[i]);
				for (int j = 0; j < E12::polynomial_size; j++)
				{
					if (i != j)
					{
						Fp::Sub(tmp, target, e[j]);
						Fp::Mul(res1[i], res1[i], tmp);
						Fp::Sub(tmp, e[i], e[j]);
						Fp::Div(res1[i], res1[i], tmp);
					}
				}
			}

			Assert::AreEqual(Fp::ToString(res1, E12::polynomial_size), Fp::ToString(res, E12::polynomial_size));
		}

		TEST_METHOD(Lagrange_E15)
		{
			Fp::Elem* e = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* res = new Fp::Elem[E15::polynomial_size];
			Fp::Elem* res1 = new Fp::Elem[E15::polynomial_size];

			Fp::Elem target, tmp;
			Fp::GetRandomElement(target);
			string str_target = Fp::ToString(target);

			E15::EvaluateLagrangePolynomials(res, target);

			for (int i = 0; i < E15::polynomial_size; i++)
			{
				E15::GetNthPoint(e[i], i);
			}

			string points = Fp::ToString(e, E15::polynomial_size);

			for (int i = 0; i < E15::polynomial_size; i++)
			{
				Fp::SetOne(res1[i]);
				for (int j = 0; j < E15::polynomial_size; j++)
				{
					if (i != j)
					{
						Fp::Sub(tmp, target, e[j]);
						Fp::Mul(res1[i], res1[i], tmp);
						Fp::Sub(tmp, e[i], e[j]);
						Fp::Div(res1[i], res1[i], tmp);
					}
				}
			}

			string expected = Fp::ToString(res1, E15::polynomial_size);
			string actual = Fp::ToString(res, E15::polynomial_size);

			Assert::AreEqual(Fp::ToString(res1, E15::polynomial_size), Fp::ToString(res, E15::polynomial_size));
		}

		TEST_METHOD(EvalVan_E12)
		{
			Fp::Elem e;
			Fp::Elem res;
			Fp::Elem res1;

			Fp::Elem target, tmp;
			Fp::GetRandomElement(target);

			E12::EvaluateVanishingPolynomial(res, target);

			Fp::SetOne(res1);
			for (int i = 0; i < E12::polynomial_size; i++)
			{
				E12::GetNthPoint(e, i);
				Fp::Sub(tmp, target, e);
				Fp::Mul(res1, res1, tmp);
			}

			Assert::AreEqual(Fp::ToString(res1), Fp::ToString(res));
		}

		TEST_METHOD(EvalVan_E15)
		{
			Fp::Elem e;
			Fp::Elem res;
			Fp::Elem res1;

			Fp::Elem target, tmp;
			Fp::GetRandomElement(target);

			E15::EvaluateVanishingPolynomial(res, target);

			Fp::SetOne(res1);
			for (int i = 0; i < E15::polynomial_size; i++)
			{
				E15::GetNthPoint(e, i);
				Fp::Sub(tmp, target, e);
				Fp::Mul(res1, res1, tmp);
			}

			Assert::AreEqual(Fp::ToString(res1), Fp::ToString(res));
		}

	};

}