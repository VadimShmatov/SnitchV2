#pragma once
#include "StaticInvoker.h"
#include "Power.h"
#include "FiniteFieldPrime.h"
#include "FiniteFieldQuadraticExtension.h"
#include "FiniteFieldCubicExtension.h"
#include "EllipticCurve.h"
#include "EllipticCurveTwister.h"
#include "FFT.h"

using namespace ec_lib;

extern const char p[];
typedef PrimeField<p, 255> Fp;
extern const StaticInvoker<Fp> Fp_initializer;

extern const Fp::Elem base1;
typedef QuadraticExtensionField<Fp, base1> Fp2;
extern const StaticInvoker<Fp2> Fp2_initializer;

extern const Fp2::Elem base2;
typedef QuadraticExtensionField<Fp2, base2> Fp4;
extern const StaticInvoker<Fp4> Fp4_initializer;

extern const Fp4::Elem base3;
typedef CubicExtensionField<Fp4, base3> Fp12;
extern const StaticInvoker<Fp12> Fp12_initializer;

extern const Fp::Elem kA_in_Fp;
extern const Fp::Elem kB_in_Fp;
extern const Fp::Elem kX0_in_Fp;
extern const Fp::Elem kY0_in_Fp;
typedef EllipticCurve<Fp, kA_in_Fp, kB_in_Fp, kX0_in_Fp, kY0_in_Fp> EC;
extern const StaticInvoker<EC> EC_initializer;

extern const Fp12::Elem kA_in_Fp12;
extern const Fp12::Elem kB_in_Fp12;
extern const Fp12::Elem kX0_in_Fp12;
extern const Fp12::Elem kY0_in_Fp12;
typedef EllipticCurve<Fp12, kA_in_Fp12, kB_in_Fp12, kX0_in_Fp12, kY0_in_Fp12> ECex;
extern const StaticInvoker<ECex> ECex_initializer;

extern const char r[];
typedef PrimeField<r, 255> Fr;
extern const StaticInvoker<Fr> Fr_initializer;

typedef TatePairingEvaluator<EC, Fp12, ECex, r, p, 12> TPE;
	
extern const Fp2::Elem kA_in_Fp2;
extern const Fp2::Elem kB_in_Fp2;
extern const Fp2::Elem kX0_in_Fp2;
extern const Fp2::Elem kY0_in_Fp2;
typedef EllipticCurve<Fp2, kA_in_Fp2, kB_in_Fp2, kX0_in_Fp2, kY0_in_Fp2> ECtw;
extern const StaticInvoker<ECtw> ECtw_initializer;

extern const Fp12::Elem kOmega;
typedef EllipticCurveTwister<ECtw, Fp2, ECex, Fp12, kOmega> ECT;
extern const StaticInvoker<ECT> ECT_initializer;

const unsigned long long kSmallDomainSize = 196608;//16384;
//const unsigned long long kSmallDomainSize = 98304;
//const unsigned long long kSmallDomainSize = 6;
typedef FFTEvaluator<Fr, r, kSmallDomainSize> SmallDomain;
extern const StaticInvoker<SmallDomain> SmallDomain_initializer;

const unsigned long long kBigDomainSize = 262144;//16386;
//const unsigned long long kBigDomainSize = 131072;
//const unsigned long long kBigDomainSize = 8;
typedef FFTEvaluator<Fr, r, kBigDomainSize> BigDomain;
extern const StaticInvoker<BigDomain> BigDomain_initializer;