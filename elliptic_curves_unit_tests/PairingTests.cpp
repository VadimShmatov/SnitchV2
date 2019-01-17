#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pairing_tests
{
	const char p[] = "49555733732057424293751983680609532697367724823777067591938430465001662709761";
	typedef PrimeField<p, 255> Fp;
	const StaticInvoker<Fp> Fp_initializer;

	const Fp::Elem base1("7");
	typedef QuadraticExtensionField<Fp, base1> Fp2;
	const StaticInvoker<Fp2> Fp2_initializer;

	const Fp2::Elem base2("1", "0");
	typedef QuadraticExtensionField<Fp2, base2> Fp4;
	const StaticInvoker<Fp4> Fp4_initializer;

	const Fp4::Elem base3("1", "0");
	typedef CubicExtensionField<Fp4, base3> Fp12;
	const StaticInvoker<Fp12> Fp12_initializer;

	const Fp::Elem kA_in_Fp("0");
	const Fp::Elem kB_in_Fp("63");
	const Fp::Elem kX0_in_Fp("10731999208998728047178266002361582242023963696001467289254813755554285624326");
	const Fp::Elem kY0_in_Fp("7635955866078984543196749524155191114396662718231915334373594812166753290410");
	typedef EllipticCurve<Fp, kA_in_Fp, kB_in_Fp, kX0_in_Fp, kY0_in_Fp> EC;
	const StaticInvoker<EC> EC_initializer;

	const Fp12::Elem kA_in_Fp12("0");
	const Fp12::Elem kB_in_Fp12("63");
	const Fp12::Elem kX0_in_Fp12(Fp4::Elem(Fp2::Elem(Fp::Elem("0"), 
														Fp::Elem("0")), Fp2::Elem(
														Fp::Elem("0"), 
														Fp::Elem("0"))), Fp4::Elem(Fp2::Elem(
														Fp::Elem("28390811723126554369689362255550933161170622203421313306650631248413086382955"), 
														Fp::Elem("41278123387756571846964868764877467619276938591751632092523605018514649491137")), Fp2::Elem(
														Fp::Elem("0"), 
														Fp::Elem("0"))), Fp4::Elem(Fp2::Elem(
														Fp::Elem("0"), 
														Fp::Elem("0")), Fp2::Elem(
														Fp::Elem("0"), 
														Fp::Elem("0"))));
	const Fp12::Elem kY0_in_Fp12(Fp4::Elem(Fp2::Elem(Fp::Elem("0"), 
														Fp::Elem("0")), Fp2::Elem(
														Fp::Elem("0"), 
														Fp::Elem("0"))), Fp4::Elem(Fp2::Elem(
														Fp::Elem("0"), 
														Fp::Elem("0")), Fp2::Elem(
														Fp::Elem("0"), 
														Fp::Elem("0"))), Fp4::Elem(Fp2::Elem(
														Fp::Elem("18211160114835212134191782191934776579671491648522177013366247766692917312740"), 
														Fp::Elem("19557410572695471311150399859744484389853916336273029688917268072254913057641")), Fp2::Elem(
														Fp::Elem("0"), 
														Fp::Elem("0"))));
	typedef EllipticCurve<Fp12, kA_in_Fp12, kB_in_Fp12, kX0_in_Fp12, kY0_in_Fp12> ECex;
	const StaticInvoker<ECex> ECex_initializer;

	const char r[] = "49555733732057424293751983680609532697145113652152511821339778989694553948161";
	typedef PrimeField<r, 255> Fr;
	const StaticInvoker<Fr> Fr_initializer;

	typedef TatePairingEvaluator<EC, Fp12, ECex, r, p, 12> TPE;

	const Fp2::Elem kA_in_Fp2("0");
	const Fp2::Elem kB_in_Fp2(Fp::Elem("63"), Fp::Elem("0"));
	const Fp2::Elem kX0_in_Fp2(Fp::Elem("41278123387756571846964868764877467619276938591751632092523605018514649491137"),
								Fp::Elem("512747133656183412817601066418401338723456128840922778800696878884953841641"));
	const Fp2::Elem kY0_in_Fp2(Fp::Elem("19557410572695471311150399859744484389853916336273029688917268072254913057641"),
								Fp::Elem("28366653339731636351838507982324370662964991892101103909686873436847095769658"));
	typedef EllipticCurve<Fp2, kA_in_Fp2, kB_in_Fp2, kX0_in_Fp2, kY0_in_Fp2> ECtw;
	const StaticInvoker<ECtw> ECtw_initializer;

	const Fp12::Elem kOmega("0", "1", "0");
	typedef EllipticCurveTwister<ECtw, Fp2, ECex, Fp12, kOmega> ECT;
	const StaticInvoker<ECT> ECT_initializer;

	TEST_CLASS(PairingTests)
	{
		TEST_METHOD(Twist)
		{
			ECex::Elem P(kX0_in_Fp12, kY0_in_Fp12);
			ECtw::Elem Q;
			ECT::Twist(Q, P);
			ECex::Elem Px, Res;
			ECtw::Elem Qx;
			Power power("111");
			ECex::Mul(Px, P, power);
			ECtw::Mul(Qx, Q, power);
			ECT::Untwist(Res, Qx);

			Assert::AreEqual(ECex::ToString(Px).c_str(), ECex::ToString(Res).c_str());
		}

		TEST_METHOD(Pairing)
		{
			//EC::fout.open("EC.txt", ofstream::out);
			//ECex::fout.open("ECex.txt", ofstream::out);
			EC::Elem P(kX0_in_Fp, kY0_in_Fp);
			ECex::Elem Q(kX0_in_Fp12, kY0_in_Fp12);

			EC::Elem P3;
			EC::Mul(P3, P, "2");
			ECex::Elem Q3;
			ECex::Mul(Q3, Q, "2");
			//string s = ECex::ToString(Q3);
			Fp12::Elem pairing_result_1, pairing_result_2, pairing_result_3;
			TPE tpe;
			tpe.TatePairing(pairing_result_3, P3, Q);
			string s_0 = EC::ToString(P);
			string s0 = EC::ToString(P3);
			tpe.TatePairing(pairing_result_1, P, Q);
			Fp12::Exp(pairing_result_2, pairing_result_1, "2");
			tpe.TatePairing(pairing_result_1, P3, Q);
			tpe.TatePairing(pairing_result_3, P, Q3);
			string pp = EC::ToString(P3);
			string pq = ECex::ToString(Q3);
			//string s_0 = ECex::ToString(Q);
			//string s0 = ECex::ToString(Q3);
			string s1 = Fp12::ToString(pairing_result_1);
			string s2 = Fp12::ToString(pairing_result_2);
			string s3 = Fp12::ToString(pairing_result_3);

			Assert::AreEqual(s1.c_str(), s2.c_str());
			Assert::AreEqual(s2.c_str(), s3.c_str());

			//EC::fout.close();
			//ECex::fout.close();
		}
	};
}