#include "BigInt.h"

Random* BigInt::rnd = nullptr;
BigInt BigInt::zero = BigInt::FromUint(0);
BigInt BigInt::one = BigInt::FromUint(1);

BigInt::BigInt()
{
	mpz_init(m_mpzNumber);
}

BigInt::BigInt(const BigInt& other)
{
	mpz_init_set(m_mpzNumber, other.m_mpzNumber);
}

BigInt::~BigInt()
{
	// TODO! Reference count?
	mpz_clear(m_mpzNumber);
}

BigInt BigInt::FromInt(mpir_si iNumber)
{
	BigInt res;
	mpz_set_si(res.m_mpzNumber, iNumber);
	return res; // TODO! Redundant copy constructor call?
}

BigInt BigInt::FromUint(mpir_ui uiNumber)
{
	BigInt res;
	mpz_set_ui(res.m_mpzNumber, uiNumber);
	return res; // TODO! Redundant copy constructor call?
}

BigInt BigInt::FromDec(string sNumber)
{
	BigInt res;
	mpz_set_str(res.m_mpzNumber, sNumber.c_str(), 10);
	return res; // TODO! Redundant copy constructor call?
}

BigInt BigInt::FromHex(string sNumber)
{
	BigInt res;
	mpz_set_str(res.m_mpzNumber, sNumber.c_str(), 16);
	return res; // TODO! Redundant copy constructor call?
}

BigInt BigInt::FromBin(string sNumber, bool bLittleEndian)
{
	// TODO! Improve performance using low-level functions

	BigInt res;
	if (bLittleEndian)
	{
		for (auto it = sNumber.begin(); it != sNumber.end(); it++)
		{
			res <<= 8;
			res += FromUint((unsigned char)*it);
		}
	}
	else
	{
		for (auto it = sNumber.rbegin(); it != sNumber.rend(); it++)
		{
			res <<= 8;
			res += FromUint((unsigned char)*it);
		}
	}
	return res; // TODO! Redundant copy constructor call?
}

mpir_si BigInt::ToInt() const
{
	return mpz_get_si(m_mpzNumber);
}

mpir_ui BigInt::ToUint() const
{
	return mpz_get_ui(m_mpzNumber);
}

string BigInt::ToDec() const
{
	char* buf = new char[mpz_sizeinbase(m_mpzNumber, 10) + 2];
	mpz_get_str(buf, 10, m_mpzNumber);
	string res = buf;
	delete[] buf;
	return res;
}

string BigInt::ToHex() const
{
	char* buf = new char[mpz_sizeinbase(m_mpzNumber, 16) + 2];
	mpz_get_str(buf, 16, m_mpzNumber);
	string res = buf;
	delete[] buf;
	return res;
}

/*string BigInt::ToBin(bool bLittleEndian) const
{
	// TODO! Implement this method
}*/

vector<int8> BigInt::GetNaf() const
{
	BigInt k = *this;
	vector<int8> result;
	while (k >= 1)
	{
		auto r = (k % 4).ToInt();
		if (r == 0 || r == 2)
		{
			result.insert(result.begin(), 0);
		}
		else
		{
			r = 2 - r;
			result.insert(result.begin(), r);
			k -= r;
		}
		k /= 2;
	}
	return result;
}

vector<int8> BigInt::GetBits() const
{
	BigInt k = *this;
	vector<int8> result;
	while (k >= 1)
	{
		auto r = (k % 2).ToInt();
		result.insert(result.begin(), r);
		k /= 2;
	}
	return result;
}


