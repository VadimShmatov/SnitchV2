#pragma once
#include <mpir.h>
#include "r1cs.h"

class ModMultiplier
{
protected:
	static size_t mul_step_number, add_step_number, cmp_step_number;

	static string GetBitName(string variable_name, int bit_number)
	{
		if (variable_name[0] == '~')
		{
			variable_name = variable_name.substr(1);
		}
		return variable_name + "_b_" + to_string(bit_number);
	}

	static void GetAndCheckLimbs(
		mpz_t N, 
		const string& N_name, 
		size_t libm_count, 
		size_t limb_size, 
		vector<Fr::Elem>& limbs, 
		vector<string>& limb_names, 
		map<string, Fr::Elem>& wit_map)
	{
		mpz_t Q, R;
		char buf[10000];

		mpz_init_set(Q, N);
		mpz_init(R);
		for (size_t i = 0; i < libm_count; i++)
		{
			mpz_tdiv_r_2exp(R, Q, limb_size);
			mpz_tdiv_q_2exp(Q, Q, limb_size);
			mpz_get_str(buf, 10, R);
			Fr::Elem tmp = buf;
			limbs.push_back(tmp);

			// Get limb name and value
			string limb_name = N_name + "_limb_" + to_string(i);
			limb_names.push_back(limb_name);
			wit_map[limb_name] = limbs[i];

			// Get limb bits
			for (size_t j = 0; j < limb_size; j++)
			{
				Fr::Elem bit = ((mpz_tstbit(R, j)) ? ("1") : ("0"));
				string bit_name = GetBitName(limb_name, j);
				wit_map[bit_name] = bit;
			}
		}
	}

	static void AddLimbs(
		size_t libm_count,
		const string& C_name,
		const vector<Fr::Elem>& A_limbs,
		const vector<Fr::Elem>& B_limbs,
		vector<Fr::Elem>& C_limbs,
		vector<string>& C_limb_names,
		map<string, Fr::Elem>& wit_map)
	{
		Fr::Elem tmp = "0";
		for (size_t i = 0; i < libm_count; i++)
		{
			C_limbs.push_back(tmp);
			C_limb_names.push_back(C_name + "_limb_" + to_string(i));
		}
		for (size_t i = 0; i < libm_count; i++)
		{
			Fr::Add(C_limbs[i], A_limbs[i], B_limbs[i]);
			wit_map[C_limb_names[i]] = C_limbs[i];
		}
	}

	static void MulLimbs(
		size_t libm_count,
		const string& C_name,
		const vector<Fr::Elem>& A_limbs,
		const vector<Fr::Elem>& B_limbs,
		vector<Fr::Elem>& C_limbs,
		vector<string>& C_limb_names,
		map<string, Fr::Elem>& wit_map)
	{
		Fr::Elem tmp = "0";
		for (size_t i = 0; i < 2 * libm_count; i++)
		{
			C_limbs.push_back(tmp);
			C_limb_names.push_back(C_name + "_limb_" + to_string(i));
		}
		for (size_t i = 0; i < libm_count; i++)
		{
			for (size_t j = 0; j < libm_count; j++)
			{
				Fr::Mul(tmp, A_limbs[i], B_limbs[j]);
				Fr::Add(C_limbs[i + j], C_limbs[i + j], tmp);
			}
		}
		for (size_t i = 0; i < 2 * libm_count; i++)
		{
			wit_map[C_limb_names[i]] = C_limbs[i];
		}
	}

	static void NormLimbs(
		size_t limb_count,
		size_t limb_size,
		vector<Fr::Elem>& limbs,
		vector<string>& limb_names,
		map<string, Fr::Elem>& wit_map)
	{
		mpz_t cur, res;
		mpz_init(cur);
		mpz_init_set_ui(res, 0);

		size_t tmp = limb_count;
		size_t log2_limb_count = 0;
		while (tmp > 0)
		{
			tmp >>= 1;
			log2_limb_count++;
		}
		size_t bit_size = 2 * limb_size + log2_limb_count + 1;

		for (size_t i = 0; i < 2 * limb_count; i++)
		{
			string limb_str = Fr::ToString(limbs[i]);
			mpz_set_str(cur, limb_str.c_str(), 10);
			mpz_add(cur, cur, res);
			mpz_tdiv_q_2exp(res, cur, limb_size);
			for (size_t j = 0; j < bit_size; j++)
			{
				Fr::Elem bit = ((mpz_tstbit(cur, j)) ? ("1") : ("0"));
				string bit_name = GetBitName(limb_names[i], j);
				wit_map[bit_name] = bit;
			}
		}

		// That's all???
	}

public:
	static void MulModGetWitness(mpz_t A, mpz_t B, mpz_t M, const string& A_name, const string& B_name, const string& M_name, const string& Q_name, const string& R_name, const string& Res_name, mpz_t& R, map<string, Fr::Elem>& wit_map)
	{
		const size_t libm_count = 16;
		const size_t limb_size = 64;

		mpz_t AB, Q;
		mpz_init(AB);
		mpz_init(Q);
		mpz_init(R);
		mpz_mul(AB, A, B);
		mpz_tdiv_qr(Q, R, AB, M);

		vector<Fr::Elem> A_limbs, B_limbs, M_limbs, Q_limbs, R_limbs, Res_limbs, Mul0_limbs, Mul1_limbs, Add0_limbs;
		vector<string> A_limb_names, B_limb_names, M_limb_names, Q_limb_names, R_limb_names, Res_limb_names, Mul0_limb_names, Mul1_limb_names, Add0_limb_names;
		GetAndCheckLimbs(A, A_name, libm_count, limb_size, A_limbs, A_limb_names, wit_map);
		GetAndCheckLimbs(B, B_name, libm_count, limb_size, B_limbs, B_limb_names, wit_map);
		GetAndCheckLimbs(M, M_name, libm_count, limb_size, M_limbs, M_limb_names, wit_map);
		GetAndCheckLimbs(Q, Q_name, libm_count, limb_size, Q_limbs, Q_limb_names, wit_map);
		GetAndCheckLimbs(R, R_name, libm_count, limb_size, R_limbs, R_limb_names, wit_map);
		GetAndCheckLimbs(AB, Res_name, 2 * libm_count, limb_size, Res_limbs, Res_limb_names, wit_map);

		MulLimbs(libm_count, "mul_" + to_string(mul_step_number++), A_limbs, B_limbs, Mul0_limbs, Mul0_limb_names, wit_map);
		MulLimbs(libm_count, "mul_" + to_string(mul_step_number++), Q_limbs, M_limbs, Mul1_limbs, Mul1_limb_names, wit_map);
		AddLimbs(libm_count, "add_" + to_string(add_step_number++), Mul1_limbs, R_limbs, Add0_limbs, Add0_limb_names, wit_map);
		for (size_t i = libm_count; i < 2 * libm_count; i++)
		{
			Add0_limbs.push_back(Mul1_limbs[i]);
			Add0_limb_names.push_back(Mul1_limb_names[i]);
		}
		NormLimbs(libm_count, limb_size, Mul0_limbs, Mul0_limb_names, wit_map);
		NormLimbs(libm_count, limb_size, Add0_limbs, Add0_limb_names, wit_map);

		// TODO! Compare R and M!

		// That's all???
	}
};