#pragma once
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

#include "r1cs.h"

#include "mimc5.h"

using namespace std;
using namespace ec_lib;

struct R1CS
{
    size_t n, m, input_size;
    map<pair<size_t, size_t>, Fr::Elem> mA, mB, mC;
    vector<string> V;
    unordered_map<string, size_t> mV_index;

    R1CS();
    R1CS(size_t in_n, size_t in_m, size_t in_input_size, const vector<string>& in_V);
};

class R1CSGenerator
{
public:
    static R1CS Generate();

private:
    static size_t mimc_step_number;
	static size_t add_step_number;
	static size_t mul_step_number;
	static size_t cmp_step_number;
    static shared_ptr<MiMC5> mimc5;
    static void add_constraint(map<string, Fr::Elem> constraint[3], R1CS& r1cs);
    static void add_mimc5_round(size_t round_number, map<string, Fr::Elem> input, R1CS& r1cs);
    static void add_mimc5_step(map<string, Fr::Elem> input, R1CS& r1cs);
    static void add_mimc5_hash_step(string v1, const Fr::Elem& k1, string v2, const Fr::Elem& k2, string w1, R1CS& r1cs);
    static void add_mimc5_hash(vector<string> blocks, string w1, R1CS& r1cs);
    static bool is_power_of_2(size_t n);

	static void add_limb_check(size_t limb_size, const string& limb, R1CS& r1cs);
	static void add_big_number_check(size_t limb_size, size_t limb_count, const string& number_name, vector<string>& number_limbs, R1CS& r1cs);
	static void add_add_check(size_t limb_count, const vector<string>& A_limbs, const vector<string>& B_limbs, const vector<string>& C_limbs, R1CS& r1cs);
	static void add_mul_check(size_t limb_count, const vector<string>& A_limbs, const vector<string>& B_limbs, const vector<string>& C_limbs, R1CS& r1cs);
	static void add_norm_check(size_t limb_size, size_t limb_count, const vector<string>& A_limbs, const vector<string>& B_limbs, R1CS& r1cs);
	static void add_mul_mod_check(size_t limb_size, size_t limb_count, const vector<string>& A_limbs, const vector<string>& B_limbs, const vector<string>& M_limbs, const vector<string>& Q_limbs, const vector<string>& R_limbs, const vector<string>& Result_limbs, R1CS& r1cs);
};