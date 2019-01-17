#pragma once
#include <stdio.h>
#include <mpir.h>
#include <string>
#include "Random.h"

using namespace std;

typedef signed char int8;

class BigInt
{
protected:
	static Random* rnd;
	mpz_t m_mpzNumber;

public:
	BigInt();
	BigInt(const BigInt &other);
	~BigInt();

	static BigInt zero;
	static BigInt one;

	static BigInt FromInt(mpir_si iNumber);
	static BigInt FromUint(mpir_ui uiNumber);
	static BigInt FromDec(string sNumber);
	static BigInt FromHex(string sNumber);
	static BigInt FromBin(string sNumber, bool bLittleEndian = false);

	mpir_si ToInt() const;
	mpir_ui ToUint() const;
	string ToDec() const;
	string ToHex() const;
	// string ToBin(bool bLittleEndian = false) const;

	inline BigInt& operator=(BigInt&& other)
	{
		if (this != &other)
		{
			mpz_swap(m_mpzNumber, other.m_mpzNumber);
		}
		return *this;
	}

	inline BigInt& operator=(const BigInt& other)
	{
		if (this != &other)
		{
			mpz_set(m_mpzNumber, other.m_mpzNumber);
		}
		return *this;
	}

	inline BigInt & operator=(const unsigned long long other)
	{
		mpz_set_ui(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const unsigned long other)
	{
		mpz_set_ui(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const unsigned int other)
	{
		mpz_set_ui(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const unsigned short other)
	{
		mpz_set_ui(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const unsigned char other)
	{
		mpz_set_ui(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const signed long long other)
	{
		mpz_set_si(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const signed long other)
	{
		mpz_set_si(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const signed int other)
	{
		mpz_set_si(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const signed short other)
	{
		mpz_set_si(m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator=(const signed char other)
	{
		mpz_set_si(m_mpzNumber, other);
		return *this;
	}

	/*inline BigInt & operator=(BigInt other)
	{
		mpz_swap(m_mpzNumber, other.m_mpzNumber);
		return *this;
	}*/

	inline BigInt operator+() const
	{
		return *this;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_add(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator+(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator+(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator+(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator+(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator+(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator+(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned long long) - bigInt1);
		return res;
	}

	inline friend BigInt operator+(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned long)-bigInt1);
		return res;
	}

	inline friend BigInt operator+(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned int)-bigInt1);
		return res;
	}

	inline friend BigInt operator+(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned short)-bigInt1);
		return res;
	}

	inline friend BigInt operator+(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned char)-bigInt1);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned long long) - bigInt2);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned long)-bigInt2);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned int)-bigInt2);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned short)-bigInt2);
		return res;
	}

	inline friend BigInt operator+(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned char)-bigInt2);
		return res;
	}

	inline BigInt & operator+=(const BigInt & other)
	{
		mpz_add(m_mpzNumber, m_mpzNumber, other.m_mpzNumber);
		return *this;
	}

	inline BigInt & operator+=(const unsigned long long other)
	{
		mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator+=(const unsigned long other)
	{
		mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator+=(const unsigned int other)
	{
		mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator+=(const unsigned short other)
	{
		mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator+=(const unsigned char other)
	{
		mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator+=(const signed long long other)
	{
		if (other >= 0)
			mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, (const unsigned long long) - other);
		return *this;
	}

	inline BigInt & operator+=(const signed long other)
	{
		if (other >= 0)
			mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, (const unsigned long)-other);
		return *this;
	}

	inline BigInt & operator+=(const signed int other)
	{
		if (other >= 0)
			mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, (const unsigned int)-other);
		return *this;
	}

	inline BigInt & operator+=(const signed short other)
	{
		if (other >= 0)
			mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, (const unsigned short)-other);
		return *this;
	}

	inline BigInt & operator+=(const signed char other)
	{
		if (other >= 0)
			mpz_add_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, (const unsigned char)-other);
		return *this;
	}

	inline BigInt & operator++()
	{
		mpz_add_ui(m_mpzNumber, m_mpzNumber, 1);
		return *this;
	}

	inline BigInt operator++(int)
	{
		BigInt res(*this);
		operator++();
		return res;
	}

	inline BigInt operator-() const
	{
		BigInt res(*this);
		mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_sub(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator-(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator-(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator-(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator-(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator-(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator-(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
		{
			mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		}
		else
		{
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned long long) - bigInt1);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator-(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
		{
			mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		}
		else
		{
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned long)-bigInt1);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator-(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
		{
			mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		}
		else
		{
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned int)-bigInt1);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator-(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
		{
			mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		}
		else
		{
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned short)-bigInt1);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator-(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
		{
			mpz_ui_sub(res.m_mpzNumber, bigInt1, bigInt2.m_mpzNumber);
		}
		else
		{
			mpz_add_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, (const unsigned char)-bigInt1);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned long long) - bigInt2);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned long)-bigInt2);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned int)-bigInt2);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned short)-bigInt2);
		return res;
	}

	inline friend BigInt operator-(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		if (bigInt1 >= 0)
			mpz_sub_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_add_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned char)-bigInt2);
		return res;
	}

	inline BigInt & operator-=(const BigInt & other)
	{
		mpz_sub(m_mpzNumber, m_mpzNumber, other.m_mpzNumber);
		return *this;
	}

	inline BigInt & operator-=(const unsigned long long other)
	{
		mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator-=(const unsigned long other)
	{
		mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator-=(const unsigned int other)
	{
		mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator-=(const unsigned short other)
	{
		mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator-=(const unsigned char other)
	{
		mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator-=(const signed long long other)
	{
		if (other >= 0)
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_add_ui(m_mpzNumber, m_mpzNumber, (const unsigned long long) - other);
		return *this;
	}

	inline BigInt & operator-=(const signed long other)
	{
		if (other >= 0)
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_add_ui(m_mpzNumber, m_mpzNumber, (const unsigned long)-other);
		return *this;
	}

	inline BigInt & operator-=(const signed int other)
	{
		if (other >= 0)
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_add_ui(m_mpzNumber, m_mpzNumber, (const unsigned int)-other);
		return *this;
	}

	inline BigInt & operator-=(const signed short other)
	{
		if (other >= 0)
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_add_ui(m_mpzNumber, m_mpzNumber, (const unsigned short)-other);
		return *this;
	}

	inline BigInt & operator-=(const signed char other)
	{
		if (other >= 0)
			mpz_sub_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_add_ui(m_mpzNumber, m_mpzNumber, (const unsigned char)-other);
		return *this;
	}

	inline BigInt & operator--()
	{
		mpz_sub_ui(m_mpzNumber, m_mpzNumber, 1);
		return *this;
	}

	inline BigInt operator--(int)
	{
		BigInt res(*this);
		operator--();
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator*(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_mul_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt2.m_mpzNumber, bigInt1);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator*(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		mpz_mul_si(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline BigInt & operator*=(const BigInt & other)
	{
		mpz_mul(m_mpzNumber, m_mpzNumber, other.m_mpzNumber);
		return *this;
	}

	inline BigInt & operator*=(const unsigned long long other)
	{
		mpz_mul_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const unsigned long other)
	{
		mpz_mul_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const unsigned int other)
	{
		mpz_mul_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const unsigned short other)
	{
		mpz_mul_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const unsigned char other)
	{
		mpz_mul_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const signed long long other)
	{
		mpz_mul_si(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const signed long other)
	{
		mpz_mul_si(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const signed int other)
	{
		mpz_mul_si(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const signed short other)
	{
		mpz_mul_si(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator*=(const signed char other)
	{
		mpz_mul_si(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_fdiv_q(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator/(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
		{
			mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		}
		else
		{
			mpz_cdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned long long) - bigInt2);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
		{
			mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		}
		else
		{
			mpz_cdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned long)-bigInt2);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
		{
			mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		}
		else
		{
			mpz_cdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned int)-bigInt2);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
		{
			mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		}
		else
		{
			mpz_cdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned short)-bigInt2);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline friend BigInt operator/(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
		{
			mpz_fdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		}
		else
		{
			mpz_cdiv_q_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned char)-bigInt2);
			mpz_neg(res.m_mpzNumber, res.m_mpzNumber);
		}
		return res;
	}

	inline BigInt & operator/=(const BigInt & other)
	{
		mpz_fdiv_q(m_mpzNumber, m_mpzNumber, other.m_mpzNumber);
		return *this;
	}

	inline BigInt & operator/=(const unsigned long long other)
	{
		mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator/=(const unsigned long other)
	{
		mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator/=(const unsigned int other)
	{
		mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator/=(const unsigned short other)
	{
		mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator/=(const unsigned char other)
	{
		mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator/=(const signed long long other)
	{
		if (other >= 0)
		{
			mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		}
		else
		{
			mpz_cdiv_q_ui(m_mpzNumber, m_mpzNumber, (const unsigned long long) - other);
			mpz_neg(m_mpzNumber, m_mpzNumber);
		}
		return *this;
	}

	inline BigInt & operator/=(const signed long other)
	{
		if (other >= 0)
		{
			mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		}
		else
		{
			mpz_cdiv_q_ui(m_mpzNumber, m_mpzNumber, (const unsigned long)-other);
			mpz_neg(m_mpzNumber, m_mpzNumber);
		}
		return *this;
	}

	inline BigInt & operator/=(const signed int other)
	{
		if (other >= 0)
		{
			mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		}
		else
		{
			mpz_cdiv_q_ui(m_mpzNumber, m_mpzNumber, (const unsigned int)-other);
			mpz_neg(m_mpzNumber, m_mpzNumber);
		}
		return *this;
	}

	inline BigInt & operator/=(const signed short other)
	{
		if (other >= 0)
		{
			mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		}
		else
		{
			mpz_cdiv_q_ui(m_mpzNumber, m_mpzNumber, (const unsigned short)-other);
			mpz_neg(m_mpzNumber, m_mpzNumber);
		}
		return *this;
	}

	inline BigInt & operator/=(const signed char other)
	{
		if (other >= 0)
		{
			mpz_fdiv_q_ui(m_mpzNumber, m_mpzNumber, other);
		}
		else
		{
			mpz_cdiv_q_ui(m_mpzNumber, m_mpzNumber, (const unsigned char)-other);
			mpz_neg(m_mpzNumber, m_mpzNumber);
		}
		return *this;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_fdiv_r(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator%(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_r(res.m_mpzNumber, res.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_cdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned long long) - bigInt2);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_cdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned long)-bigInt2);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_cdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned int)-bigInt2);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_cdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned short)-bigInt2);
		return res;
	}

	inline friend BigInt operator%(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_cdiv_r_ui(res.m_mpzNumber, bigInt1.m_mpzNumber, (const unsigned char)-bigInt2);
		return res;
	}

	inline BigInt & operator%=(const BigInt & other)
	{
		mpz_fdiv_r(m_mpzNumber, m_mpzNumber, other.m_mpzNumber);
		return *this;
	}

	inline BigInt & operator%=(const unsigned long long other)
	{
		mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator%=(const unsigned long other)
	{
		mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator%=(const unsigned int other)
	{
		mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator%=(const unsigned short other)
	{
		mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator%=(const unsigned char other)
	{
		mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator%=(const signed long long other)
	{
		if (other >= 0)
			mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_cdiv_r_ui(m_mpzNumber, m_mpzNumber, (const unsigned long long) - other);
		return *this;
	}

	inline BigInt & operator%=(const signed long other)
	{
		if (other >= 0)
			mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_cdiv_r_ui(m_mpzNumber, m_mpzNumber, (const unsigned long)-other);
		return *this;
	}

	inline BigInt & operator%=(const signed int other)
	{
		if (other >= 0)
			mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_cdiv_r_ui(m_mpzNumber, m_mpzNumber, (const unsigned int)-other);
		return *this;
	}

	inline BigInt & operator%=(const signed short other)
	{
		if (other >= 0)
			mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_cdiv_r_ui(m_mpzNumber, m_mpzNumber, (const unsigned short)-other);
		return *this;
	}

	inline BigInt & operator%=(const signed char other)
	{
		if (other >= 0)
			mpz_fdiv_r_ui(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_cdiv_r_ui(m_mpzNumber, m_mpzNumber, (const unsigned char)-other);
		return *this;
	}

	inline void DivRem(const BigInt & divisor, BigInt & quotient, BigInt & remainder)
	{
		mpz_fdiv_qr(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor.m_mpzNumber);
	}

	inline void DivRem(const unsigned long long divisor, BigInt & quotient, BigInt & remainder)
	{
		mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
	}

	inline void DivRem(const unsigned long divisor, BigInt & quotient, BigInt & remainder)
	{
		mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
	}

	inline void DivRem(const unsigned int divisor, BigInt & quotient, BigInt & remainder)
	{
		mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
	}

	inline void DivRem(const unsigned short divisor, BigInt & quotient, BigInt & remainder)
	{
		mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
	}

	inline void DivRem(const unsigned char divisor, BigInt & quotient, BigInt & remainder)
	{
		mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
	}

	inline void DivRem(const signed long long divisor, BigInt & quotient, BigInt & remainder)
	{
		if (divisor >= 0)
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
		}
		else
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, (const unsigned long long) - divisor);
			mpz_neg(quotient.m_mpzNumber, quotient.m_mpzNumber);
		}
	}

	inline void DivRem(const signed long divisor, BigInt & quotient, BigInt & remainder)
	{
		if (divisor >= 0)
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
		}
		else
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, (const unsigned long)-divisor);
			mpz_neg(quotient.m_mpzNumber, quotient.m_mpzNumber);
		}
	}

	inline void DivRem(const signed int divisor, BigInt & quotient, BigInt & remainder)
	{
		if (divisor >= 0)
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
		}
		else
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, (const unsigned int)-divisor);
			mpz_neg(quotient.m_mpzNumber, quotient.m_mpzNumber);
		}
	}

	inline void DivRem(const signed short divisor, BigInt & quotient, BigInt & remainder)
	{
		if (divisor >= 0)
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
		}
		else
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, (const unsigned short)-divisor);
			mpz_neg(quotient.m_mpzNumber, quotient.m_mpzNumber);
		}
	}

	inline void DivRem(const signed char divisor, BigInt & quotient, BigInt & remainder)
	{
		if (divisor >= 0)
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, divisor);
		}
		else
		{
			mpz_fdiv_qr_ui(quotient.m_mpzNumber, remainder.m_mpzNumber, m_mpzNumber, (const unsigned char)-divisor);
			mpz_neg(quotient.m_mpzNumber, quotient.m_mpzNumber);
		}
	}

	inline BigInt operator~() const
	{
		BigInt res;
		mpz_com(res.m_mpzNumber, m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_and(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator&(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_and(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline BigInt & operator&=(const BigInt & other)
	{
		mpz_and(m_mpzNumber, m_mpzNumber, other.m_mpzNumber);
		return *this;
	}

	inline BigInt & operator&=(const unsigned long long other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const unsigned long other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const unsigned int other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const unsigned short other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const unsigned char other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const signed long long other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const signed long other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const signed int other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const signed short other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator&=(const signed char other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_and(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_ior(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator|(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_ior(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline BigInt & operator|=(const BigInt & other)
	{
		mpz_ior(m_mpzNumber, m_mpzNumber, other.m_mpzNumber);
		return *this;
	}

	inline BigInt & operator|=(const unsigned long long other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const unsigned long other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const unsigned int other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const unsigned short other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const unsigned char other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const signed long long other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const signed long other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const signed int other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const signed short other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator|=(const signed char other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_ior(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt1);
		mpz_xor(res.m_mpzNumber, tmp, bigInt2.m_mpzNumber);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline friend BigInt operator^(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_si(tmp, bigInt2);
		mpz_xor(res.m_mpzNumber, bigInt1.m_mpzNumber, tmp);
		return res;
	}

	inline BigInt & operator^=(const BigInt & other)
	{
		mpz_xor(m_mpzNumber, m_mpzNumber, other.m_mpzNumber);
		return *this;
	}

	inline BigInt & operator^=(const unsigned long long other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const unsigned long other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const unsigned int other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const unsigned short other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const unsigned char other)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const signed long long other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const signed long other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const signed int other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const signed short other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline BigInt & operator^=(const signed char other)
	{
		mpz_t tmp;
		mpz_init_set_si(tmp, other);
		mpz_xor(m_mpzNumber, m_mpzNumber, tmp);
		return *this;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_fdiv_q_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline friend BigInt operator >> (const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline BigInt & operator>>=(const BigInt & other)
	{
		mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other.ToUint());
		return *this;
	}

	inline BigInt & operator>>=(const unsigned long long other)
	{
		mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator>>=(const unsigned long other)
	{
		mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator>>=(const unsigned int other)
	{
		mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator>>=(const unsigned short other)
	{
		mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator>>=(const unsigned char other)
	{
		mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator>>=(const signed long long other)
	{
		if (other >= 0)
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline BigInt & operator>>=(const signed long other)
	{
		if (other >= 0)
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline BigInt & operator>>=(const signed int other)
	{
		if (other >= 0)
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline BigInt & operator>>=(const signed short other)
	{
		if (other >= 0)
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline BigInt & operator>>=(const signed char other)
	{
		if (other >= 0)
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_ui(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		BigInt res;
		mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		BigInt res;
		mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		BigInt res;
		mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		BigInt res;
		mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		BigInt res;
		mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const signed long long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const signed long bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const signed int bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const signed short bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const signed char bigInt1, const BigInt & bigInt2)
	{
		BigInt res;
		mpz_set_si(res.m_mpzNumber, bigInt1);
		mpz_mul_2exp(res.m_mpzNumber, res.m_mpzNumber, bigInt2.ToUint());
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const signed long long bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const signed long bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const signed int bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const signed short bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline friend BigInt operator<<(const BigInt & bigInt1, const signed char bigInt2)
	{
		BigInt res;
		if (bigInt2 >= 0)
			mpz_mul_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, bigInt2);
		else
			mpz_fdiv_q_2exp(res.m_mpzNumber, bigInt1.m_mpzNumber, -bigInt2);
		return res;
	}

	inline BigInt & operator<<=(const BigInt & other)
	{
		mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other.ToUint());
		return *this;
	}

	inline BigInt & operator<<=(const unsigned long long other)
	{
		mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator<<=(const unsigned long other)
	{
		mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator<<=(const unsigned int other)
	{
		mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator<<=(const unsigned short other)
	{
		mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator<<=(const unsigned char other)
	{
		mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		return *this;
	}

	inline BigInt & operator<<=(const signed long long other)
	{
		if (other >= 0)
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline BigInt & operator<<=(const signed long other)
	{
		if (other >= 0)
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline BigInt & operator<<=(const signed int other)
	{
		if (other >= 0)
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline BigInt & operator<<=(const signed short other)
	{
		if (other >= 0)
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline BigInt & operator<<=(const signed char other)
	{
		if (other >= 0)
			mpz_mul_2exp(m_mpzNumber, m_mpzNumber, other);
		else
			mpz_fdiv_q_2exp(m_mpzNumber, m_mpzNumber, -other);
		return *this;
	}

	inline friend bool operator==(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp(bigInt1.m_mpzNumber, bigInt2.m_mpzNumber) == 0);
	}

	inline friend bool operator==(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const signed long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const signed long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const signed int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const signed short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const signed char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const signed long long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const signed long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const signed int bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const signed short bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator==(const BigInt & bigInt1, const signed char bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) == 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp(bigInt1.m_mpzNumber, bigInt2.m_mpzNumber) != 0);
	}

	inline friend bool operator!=(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const signed long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const signed long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const signed int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const signed short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const signed char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const signed long long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const signed long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const signed int bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const signed short bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator!=(const BigInt & bigInt1, const signed char bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) != 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp(bigInt1.m_mpzNumber, bigInt2.m_mpzNumber) > 0);
	}

	inline friend bool operator>(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const signed long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const signed long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const signed int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const signed short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const signed char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) < 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const signed long long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const signed long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const signed int bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const signed short bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>(const BigInt & bigInt1, const signed char bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) > 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp(bigInt1.m_mpzNumber, bigInt2.m_mpzNumber) >= 0);
	}

	inline friend bool operator>=(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const signed long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const signed long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const signed int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const signed short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const signed char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) <= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const signed long long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const signed long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const signed int bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const signed short bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator>=(const BigInt & bigInt1, const signed char bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) >= 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp(bigInt1.m_mpzNumber, bigInt2.m_mpzNumber) < 0);
	}

	inline friend bool operator<(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const signed long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const signed long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const signed int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const signed short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const signed char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) > 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const signed long long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const signed long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const signed int bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const signed short bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<(const BigInt & bigInt1, const signed char bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) < 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp(bigInt1.m_mpzNumber, bigInt2.m_mpzNumber) <= 0);
	}

	inline friend bool operator<=(const unsigned long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const unsigned long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const unsigned int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const unsigned short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const unsigned char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_ui(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const unsigned long long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const unsigned long bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const unsigned int bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const unsigned short bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const unsigned char bigInt2)
	{
		return (mpz_cmp_ui(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const signed long long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const signed long bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const signed int bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const signed short bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const signed char bigInt1, const BigInt & bigInt2)
	{
		return (mpz_cmp_si(bigInt2.m_mpzNumber, bigInt1) >= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const signed long long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const signed long bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const signed int bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const signed short bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline friend bool operator<=(const BigInt & bigInt1, const signed char bigInt2)
	{
		return (mpz_cmp_si(bigInt1.m_mpzNumber, bigInt2) <= 0);
	}

	inline BigInt PowMod(const BigInt & pow, const BigInt & mod) const
	{
		BigInt res;
		mpz_powm(res.m_mpzNumber, m_mpzNumber, pow.m_mpzNumber, mod.m_mpzNumber);
		return res;
	}

	inline BigInt PowMod(const unsigned long long pow, const BigInt & mod) const
	{
		BigInt res;
		mpz_powm_ui(res.m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
		return res;
	}

	inline BigInt PowMod(const unsigned long pow, const BigInt & mod) const
	{
		BigInt res;
		mpz_powm_ui(res.m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
		return res;
	}

	inline BigInt PowMod(const unsigned int pow, const BigInt & mod) const
	{
		BigInt res;
		mpz_powm_ui(res.m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
		return res;
	}

	inline BigInt PowMod(const unsigned short pow, const BigInt & mod) const
	{
		BigInt res;
		mpz_powm_ui(res.m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
		return res;
	}

	inline BigInt PowMod(const unsigned char pow, const BigInt & mod) const
	{
		BigInt res;
		mpz_powm_ui(res.m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
		return res;
	}

	inline void PowModSelf(const BigInt & pow, const BigInt & mod)
	{
		mpz_powm(m_mpzNumber, m_mpzNumber, pow.m_mpzNumber, mod.m_mpzNumber);
	}

	inline void PowModSelf(const unsigned long long pow, const BigInt & mod)
	{
		mpz_powm_ui(m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
	}

	inline void PowModSelf(const unsigned long pow, const BigInt & mod)
	{
		mpz_powm_ui(m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
	}

	inline void PowModSelf(const unsigned int pow, const BigInt & mod)
	{
		mpz_powm_ui(m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
	}

	inline void PowModSelf(const unsigned short pow, const BigInt & mod)
	{
		mpz_powm_ui(m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
	}

	inline void PowModSelf(const unsigned char pow, const BigInt & mod)
	{
		mpz_powm_ui(m_mpzNumber, m_mpzNumber, pow, mod.m_mpzNumber);
	}

	inline BigInt InvMod(const BigInt & mod) const
	{
		BigInt res;
		mpz_invert(res.m_mpzNumber, m_mpzNumber, mod.m_mpzNumber);
		return res;
	}

	inline BigInt InvMod(const unsigned long long mod) const
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(res.m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
		return res;
	}

	inline BigInt InvMod(const unsigned long mod) const
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(res.m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
		return res;
	}

	inline BigInt InvMod(const unsigned int mod) const
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(res.m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
		return res;
	}

	inline BigInt InvMod(const unsigned short mod) const
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(res.m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
		return res;
	}

	inline BigInt InvMod(const unsigned char mod) const
	{
		BigInt res;
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(res.m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
		return res;
	}

	inline void InvModSelf(const BigInt & mod)
	{
		mpz_invert(m_mpzNumber, m_mpzNumber, mod.m_mpzNumber);
	}

	inline void InvModSelf(const unsigned long long mod)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
	}

	inline void InvModSelf(const unsigned long mod)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
	}

	inline void InvModSelf(const unsigned int mod)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
	}

	inline void InvModSelf(const unsigned short mod)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
	}

	inline void InvModSelf(const unsigned char mod)
	{
		mpz_t tmp;
		mpz_init_set_ui(tmp, mod);
		mpz_invert(m_mpzNumber, m_mpzNumber, tmp);
		mpz_clear(tmp);
	}

	inline unsigned long int BitCount()
	{
		return mpz_popcount(m_mpzNumber);
	}

	vector<int8> GetNaf() const;
	vector<int8> GetBits() const;

	inline static BigInt Random(const BigInt & max)
	{
		if (rnd == nullptr)
			rnd = Random::GetInstance();

		BigInt res;
		size_t nRequiredBitSize = mpz_sizeinbase(max.m_mpzNumber, 2);
		unsigned char ucFirstByteMod = ((nRequiredBitSize % 8 == 0) ? (0xff) : ((1 << (nRequiredBitSize % 8)) - 1));
		size_t nBufSize = (nRequiredBitSize + 7) / 8;
		unsigned char* pBuf = new unsigned char[nBufSize];
		do
		{
			rnd->GenBytes(pBuf, nBufSize);
			pBuf[0] &= ucFirstByteMod;
			mpz_import(res.m_mpzNumber, nBufSize, 1, 1, 1, 0, pBuf);
		} while (mpz_cmp(res.m_mpzNumber, max.m_mpzNumber) >= 0);
		return res;
	}

};
