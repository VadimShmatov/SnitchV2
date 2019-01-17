//#include <iostream>
//
//#include "BigInt.h"
//#include "FiniteFieldPrime.h"
//
//const char mod7[] = "7";
//
//int main()
//{
//	BigInt x = BigInt::FromInt(10);
//	BigInt y = BigInt::FromInt(3);
//	BigInt z = x / y;
//	//printf_s("%lli", (x /= y).ToInt());
//
//
//	PrimeField<mod7>::Elem e1(BigInt::FromInt(3));
//	PrimeField<mod7>::Elem e2(BigInt::FromInt(5));
//	PrimeField<mod7>::Elem e3 = e1 + e2;
//	std::cout << e1.ToString() << " + " << e2.ToString() << " = " << e3.ToString() << '\n';
//	return 0;
//}