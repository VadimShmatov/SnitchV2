#pragma once
#include <vector>
#include "BigInt.h"
//#include "FiniteFieldQuadraticExtension.h"

using namespace std;

template <class Field>
class BinaryNafExponentiator
{
protected:
	vector<int8> m_vNaf;

public:
	BinaryNafExponentiator(const BigInt& biPower)
	{
		m_vNaf = biPower.GetNaf();
	}

	~BinaryNafExponentiator()
	{

	}

	typename Field::Elem Exponentiate(const typename Field::Elem& elem_to_exp) const
	{
		// TODO! Zero check!
		typename Field::Elem result = elem_to_exp;
		typename Field::Elem elem_to_exp_inv;
		bool elem_to_exp_inv_calculated = false;
		for (auto it = m_vNaf.begin() + 1; it != m_vNaf.end(); it++)
		{
			result.square();
			string tmp = result.ToString();
			if (*it == 1)
			{
				result *= elem_to_exp;
			}
			else if (*it == -1)
			{
				if (!elem_to_exp_inv_calculated)
				{
					elem_to_exp_inv = elem_to_exp.get_inv();
					elem_to_exp_inv_calculated = true;
				}
				result *= elem_to_exp_inv;
			}
		}
		return result;
	}
};

template <class Field>
class BinaryNafAdditionalExponentiator
{
protected:
	vector<int8> m_vNaf;

public:
	BinaryNafAdditionalExponentiator(const BigInt& biPower)
	{
		m_vNaf = biPower.GetNaf();
	}

	~BinaryNafAdditionalExponentiator()
	{

	}

	typename Field::Elem Exponentiate(const typename Field::Elem& elem_to_exp) const
	{
		// TODO! Zero check!
		typename Field::Elem result = elem_to_exp;
		typename Field::Elem elem_to_exp_inv;
		bool elem_to_exp_inv_calculated = false;
		for (auto it = m_vNaf.begin() + 1; it != m_vNaf.end(); it++)
		{
			result.dbl();
			string tmp = result.ToString();
			if (*it == 1)
			{
				result += elem_to_exp;
			}
			else if (*it == -1)
			{
				if (!elem_to_exp_inv_calculated)
				{
					elem_to_exp_inv = -elem_to_exp;
					elem_to_exp_inv_calculated = true;
				}
				result += elem_to_exp_inv;
			}
		}
		return result;
	}
};

