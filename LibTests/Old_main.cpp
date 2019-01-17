//// LibTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
//#include <FiniteFieldPrime.h>
//#include <iostream>
//#include <cstdio>
//#include <ctime>
//
//using namespace std;
//
//const char mod[] = "41609267124995133285768740581002262150816576467207736915423657302317611036853";
//
//int main()
//{
//	clock_t start;
//	PrimeField<mod, 255>::Init();
//	PrimeField<mod, 255>::Elem e1("5727030983365422404648469636657308112770374107258382174706631886831014111794"),
//		e2("38184148273571209321313127483946345782553934400668527527095353088700544045478"),
//		e3,
//		e4("1"),
//		sum_res("2301912131941498440192856539601391744507732040719172786378327673213947120419"),
//		mul_res("9633487048858616622044781259267292230528507551601158641341390311081983659863"),
//		div_res("37592778724191529760748759480140500206602671902645673291731336564613870640552"),
//		exp_res("27233946609430044595601437989890552365367696861953931426755820161822087279394");
//
//	mpz_t P, E1, E2, E3;
//	mpz_init_set_str(P, mod, 10);
//	mpz_init_set_str(E1, "5727030983365422404648469636657308112770374107258382174706631886831014111794", 10);
//	mpz_init_set_str(E2, "38184148273571209321313127483946345782553934400668527527095353088700544045478", 10);
//	mpz_init(E3);
//
//	// Addition
//	start = std::clock();
//	for (int i = 0; i < 10000000; i++)
//	{
//		PrimeField<mod, 255>::Add(e3, e1, e2);
//	}
//	cout << "Addition 1: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//
//	start = std::clock();
//	for (int i = 0; i < 10000000; i++)
//	{
//		mpz_add(E3, E1, E2);
//		mpz_mod(E3, E3, P);
//	}
//	cout << "Addition 2: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//	cout << PrimeField<mod, 255>::CheckEqual(e3, sum_res) << '\n';
//
//	// Multiplication
//	start = std::clock();
//	for (int i = 0; i < 10000000; i++)
//	{
//		PrimeField<mod, 255>::Mul(e3, e1, e4);
//	}
//	cout << "Multiplication 1: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//	e4.ToSpecialForm();
//	PrimeField<mod, 255>::Elem e3_spec;
//	start = std::clock();
//	for (int i = 0; i < 10000000; i++)
//	{
//		PrimeField<mod, 255>::Mul(e3_spec, e1, e4);
//	}
//	cout << "Multiplication (spec) 1: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//
//	start = std::clock();
//	for (int i = 0; i < 10000000; i++)
//	{
//		mpz_mul(E3, E1, E2);
//		mpz_mod(E3, E3, P);
//	}
//	cout << "Multiplication 2: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//	cout << PrimeField<mod, 255>::CheckEqual(e3, e3_spec) << '\n';
//
//	// Division
//	start = std::clock();
//	for (int i = 0; i < 1000000; i++)
//	{
//		PrimeField<mod, 255>::Div(e3, e1, e2);
//	}
//	cout << "Division 1: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//
//	start = std::clock();
//	for (int i = 0; i < 1000000; i++)
//	{
//		mpz_invert(E3, E2, P);
//		mpz_mul(E3, E1, E3);
//		mpz_mod(E3, E3, P);
//	}
//	cout << "Division 2: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//	cout << PrimeField<mod, 255>::CheckEqual(e3, div_res) << '\n';
//
//	// Exponentiation
//	start = std::clock();
//	for (int i = 0; i < 100000; i++)
//	{
//		PrimeField<mod, 255>::Exp(e3, e1, e2);
//	}
//	cout << "Exponentiation 1: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//
//	start = std::clock();
//	for (int i = 0; i < 100000; i++)
//	{
//		mpz_powm(E3, E1, E2, P);
//	}
//	cout << "Exponentiation 2: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
//	cout << PrimeField<mod, 255>::CheckEqual(e3, exp_res) << '\n';
//
//	return 0;
//}