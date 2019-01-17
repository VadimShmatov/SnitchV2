#include "r1cs.h"

size_t R1CSGenerator::mimc_step_number = 0;
size_t R1CSGenerator::add_step_number = 0;
size_t R1CSGenerator::mul_step_number = 0;
size_t R1CSGenerator::cmp_step_number = 0;
shared_ptr<MiMC5> R1CSGenerator::mimc5 = nullptr;

R1CS::R1CS()
{
    R1CS(0, 0, 0, {});
}

R1CS::R1CS(size_t in_n, size_t in_m, size_t in_input_size, const vector<string>& in_V)
    : n(in_n), m(in_m), input_size(in_input_size), V(in_V)
{
    for (size_t i = 0; i < V.size(); i++)
        mV_index.insert({ V[i], i });
}

R1CS R1CSGenerator::Generate()
{
	if (mimc5 == nullptr)
	{
		mimc5 = MiMC5::GetInstance();
	}

	R1CS r1cs(0, 21, 21, { "~one", "~root", "~topic_hash", "~topic_password_hash", "~comment_hash", 
		"~N_limb_0", "~N_limb_1", "~N_limb_2", "~N_limb_3", "~N_limb_4", "~N_limb_5", "~N_limb_6", "~N_limb_7", 
		"~N_limb_8", "~N_limb_9", "~N_limb_10", "~N_limb_11", "~N_limb_12", "~N_limb_13", "~N_limb_14", "~N_limb_15" });

	// Check that bits are bits
	// b_x * (1 - b_x) = 0
	for (size_t i = 0; i < 31; i++)
	{
		map<string, Fr::Elem> c3[3] = {
			{ { "b_" + to_string(i), "1" } },
			{ { "~one", "1" }, { "b_" + to_string(i), "-1" } },
			{  }
		};
		add_constraint(c3, r1cs);
	}

	// Check password hash
	mimc_step_number = 0;
	add_mimc5_hash({ "pass" }, "hash_0", r1cs);

	// All iterations
	for (size_t i = 0; i < 31; i++)
	{
		map<string, Fr::Elem> c3_1[3] = {
			{ { "b_" + to_string(i), "1" } },
			{ { "co_hash_" + to_string(i), "1" }, { "hash_" + to_string(i), "-1" } },
			{ { "part_" + to_string(i) + "_left", "1" }, { "hash_" + to_string(i), "-1" } }
		};
		add_constraint(c3_1, r1cs);
		map<string, Fr::Elem> c3_2[3] = {
			{ { "b_" + to_string(i), "1" } },
			{ { "hash_" + to_string(i), "1" }, { "co_hash_" + to_string(i), "-1" } },
			{ { "part_" + to_string(i) + "_right", "1" }, { "co_hash_" + to_string(i), "-1" } }
		};
		add_constraint(c3_2, r1cs);
		if (i != 30)
			add_mimc5_hash({ "part_" + to_string(i) + "_left", "part_" + to_string(i) + "_right" }, "hash_" + to_string(i + 1), r1cs);
		else
			add_mimc5_hash({ "part_" + to_string(i) + "_left", "part_" + to_string(i) + "_right" }, "~root", r1cs);
	}

	// Check comment uniqueness
	add_mimc5_hash({ "pass", "~topic_hash" }, "~topic_password_hash", r1cs);

	// Check comment integrity
	map<string, Fr::Elem> c3[3] = {
			{ { "~comment_hash", "1" } },
			{ { "~one", "1" } },
			{ { "comment_hash", "1" } }
	};
	add_constraint(c3, r1cs);

	// Check RSA knowledge
	vector<string> Sig_limbs,
		Sig2_limbs, Sig2Q_limbs, Sig2R_limbs,
		Sig4_limbs, Sig4Q_limbs, Sig4R_limbs,
		Sig8_limbs, Sig8Q_limbs, Sig8R_limbs,
		Sig16_limbs, Sig16Q_limbs, Sig16R_limbs,
		Msg_limbs, MsgQ_limbs, MsgR_limbs,
		N_limbs;
	add_big_number_check(64, 16, "Sig", Sig_limbs, r1cs);
	add_big_number_check(64, 32, "Sig2", Sig2_limbs, r1cs);
	add_big_number_check(64, 16, "Sig2Q", Sig2Q_limbs, r1cs);
	add_big_number_check(64, 16, "Sig2R", Sig2R_limbs, r1cs);
	add_big_number_check(64, 32, "Sig4", Sig4_limbs, r1cs);
	add_big_number_check(64, 16, "Sig4Q", Sig4Q_limbs, r1cs);
	add_big_number_check(64, 16, "Sig4R", Sig4R_limbs, r1cs);
	add_big_number_check(64, 32, "Sig8", Sig8_limbs, r1cs);
	add_big_number_check(64, 16, "Sig8Q", Sig8Q_limbs, r1cs);
	add_big_number_check(64, 16, "Sig8R", Sig8R_limbs, r1cs);
	add_big_number_check(64, 32, "Sig16", Sig16_limbs, r1cs);
	add_big_number_check(64, 16, "Sig16Q", Sig16Q_limbs, r1cs);
	add_big_number_check(64, 16, "Sig16R", Sig16R_limbs, r1cs);
	add_big_number_check(64, 32, "Msg", Msg_limbs, r1cs);
	add_big_number_check(64, 16, "MsgQ", MsgQ_limbs, r1cs);
	add_big_number_check(64, 16, "MsgR", MsgR_limbs, r1cs);
	add_big_number_check(64, 16, "~N", N_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig_limbs, Sig_limbs, N_limbs, Sig2Q_limbs, Sig2R_limbs, Sig2_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig2R_limbs, Sig2R_limbs, N_limbs, Sig4Q_limbs, Sig4R_limbs, Sig4_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig4R_limbs, Sig4R_limbs, N_limbs, Sig8Q_limbs, Sig8R_limbs, Sig8_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig8R_limbs, Sig8R_limbs, N_limbs, Sig16Q_limbs, Sig16R_limbs, Sig16_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig16R_limbs, Sig_limbs, N_limbs, MsgQ_limbs, MsgR_limbs, Msg_limbs, r1cs);

	// Check Msg correctness
	add_mimc5_hash({ "hash_0", "~topic_hash" }, "msg_to_sign", r1cs);
	map<string, Fr::Elem> c3_1[3] = {
			{  },
			{ { "~one", "1" } },
			{ { "msg_to_sign", "1" } }
	};
	Fr::Elem cc = "1";
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			c3_1[0]["MsgR_limb_" + to_string(i) + "_b_" + to_string(j)] = cc;
			Fr::Dbl(cc, cc);
		}
	}
	add_constraint(c3_1, r1cs);

	cout << "Size of R1CS (before padding): " << r1cs.n << '\n';
	size_t n_orig = r1cs.n;

	// Padding to power of 2
	map<string, Fr::Elem> dummy[3] = {
		{  },
		{  },
		{  }
	};

	unsigned long long t1 = r1cs.n & (r1cs.n - 1);
	unsigned long long t2 = t1 & (t1 - 1);
	while (t2 != 0)
	{
		add_constraint(dummy, r1cs);
		t1 = r1cs.n & (r1cs.n - 1);
		t2 = t1 & (t1 - 1);
	}

	cout << "Size of R1CS (after padding):  " << r1cs.n << '\n';
	cout << "Efficiency: " << fixed << setprecision(3) << ((double)n_orig / r1cs.n) << '\n';

	return r1cs;

    /*R1CS r1cs(0, 11, 11, { "~one", "~v1", "~v2", "~v3", "~v4", "~v5", "~v6", "~v7", "~v8", "~v9", "~w1" });
    mimc_step_number = 0;
    add_mimc5_hash({ "~v1", "~v2", "~v3", "~v4", "~v5", "~v6", "~v7", "~v8", "~v9" }, "~w1", r1cs);

    cout << "Size of R1CS (before padding): " << r1cs.n << '\n';
    size_t n_orig = r1cs.n;

    map<string, Fr::Elem> dummy[3] = {
        {  },
        {  },
        {  }
    };
    while (!is_power_of_2(r1cs.n))
        add_constraint(dummy, r1cs);

    cout << "Size of R1CS (after padding):  " << r1cs.n << '\n';
    cout << "Efficiency: " << fixed << setprecision(3) << ((double)n_orig / r1cs.n) << '\n';

    return r1cs;*/

	/*R1CS r1cs(0, 1, 1, { "~one" });
	vector<string> Sig_limbs, 
		Sig2_limbs, Sig2Q_limbs, Sig2R_limbs, 
		Sig4_limbs, Sig4Q_limbs, Sig4R_limbs, 
		Sig8_limbs, Sig8Q_limbs, Sig8R_limbs,
		Sig16_limbs, Sig16Q_limbs, Sig16R_limbs,
		Msg_limbs, MsgQ_limbs, MsgR_limbs,
		N_limbs;
	add_big_number_check(64, 16, "Sig", Sig_limbs, r1cs);
	add_big_number_check(64, 32, "Sig2", Sig2_limbs, r1cs);
	add_big_number_check(64, 16, "Sig2Q", Sig2Q_limbs, r1cs);
	add_big_number_check(64, 16, "Sig2R", Sig2R_limbs, r1cs);
	add_big_number_check(64, 32, "Sig4", Sig4_limbs, r1cs);
	add_big_number_check(64, 16, "Sig4Q", Sig4Q_limbs, r1cs);
	add_big_number_check(64, 16, "Sig4R", Sig4R_limbs, r1cs);
	add_big_number_check(64, 32, "Sig8", Sig8_limbs, r1cs);
	add_big_number_check(64, 16, "Sig8Q", Sig8Q_limbs, r1cs);
	add_big_number_check(64, 16, "Sig8R", Sig8R_limbs, r1cs);
	add_big_number_check(64, 32, "Sig16", Sig16_limbs, r1cs);
	add_big_number_check(64, 16, "Sig16Q", Sig16Q_limbs, r1cs);
	add_big_number_check(64, 16, "Sig16R", Sig16R_limbs, r1cs);
	add_big_number_check(64, 32, "Msg", Msg_limbs, r1cs);
	add_big_number_check(64, 16, "MsgQ", MsgQ_limbs, r1cs);
	add_big_number_check(64, 16, "MsgR", MsgR_limbs, r1cs);
	add_big_number_check(64, 16, "N", N_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig_limbs, Sig_limbs, N_limbs, Sig2Q_limbs, Sig2R_limbs, Sig2_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig2R_limbs, Sig2R_limbs, N_limbs, Sig4Q_limbs, Sig4R_limbs, Sig4_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig4R_limbs, Sig4R_limbs, N_limbs, Sig8Q_limbs, Sig8R_limbs, Sig8_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig8R_limbs, Sig8R_limbs, N_limbs, Sig16Q_limbs, Sig16R_limbs, Sig16_limbs, r1cs);
	add_mul_mod_check(64, 16, Sig16R_limbs, Sig_limbs, N_limbs, MsgQ_limbs, MsgR_limbs, Msg_limbs, r1cs);

	// Padding to power of 2
	cout << "Size of R1CS (before padding): " << r1cs.n << '\n';
	size_t n_orig = r1cs.n;

	map<string, Fr::Elem> dummy[3] = {
		{  },
		{  },
		{  }
	};

	unsigned long long t1 = r1cs.n & (r1cs.n - 1);
	unsigned long long t2 = t1 & (t1 - 1);
	while (t2 != 0)
	{
		add_constraint(dummy, r1cs);
		t1 = r1cs.n & (r1cs.n - 1);
		t2 = t1 & (t1 - 1);
	}

	cout << "Size of R1CS (after padding):  " << r1cs.n << '\n';
	cout << "Efficiency: " << fixed << setprecision(3) << ((double)n_orig / r1cs.n) << '\n';

	return r1cs;*/

	/*R1CS r1cs(0, 1, 1, { "~one" });
	vector<string> A_limbs, B_limbs, M_limbs, Q_limbs, R_limbs, Res_limbs;
	add_big_number_check(64, 16, "A", A_limbs, r1cs);
	//add_big_number_check(64, 16, "A", A_limbs, r1cs);
	add_big_number_check(64, 16, "M", M_limbs, r1cs);
	add_big_number_check(64, 16, "Q", Q_limbs, r1cs);
	add_big_number_check(64, 16, "R", R_limbs, r1cs);
	add_big_number_check(64, 32, "Res", Res_limbs, r1cs);
	add_mul_mod_check(64, 16, A_limbs, A_limbs, M_limbs, Q_limbs, R_limbs, Res_limbs, r1cs);

	// Padding to power of 2
	cout << "Size of R1CS (before padding): " << r1cs.n << '\n';
	size_t n_orig = r1cs.n;

	map<string, Fr::Elem> dummy[3] = {
		{  },
		{  },
		{  }
	};

	unsigned long long t1 = r1cs.n & (r1cs.n - 1);
	unsigned long long t2 = t1 & (t1 - 1);
	while (t2 != 0)
	{
		add_constraint(dummy, r1cs);
		t1 = r1cs.n & (r1cs.n - 1);
		t2 = t1 & (t1 - 1);
	}

	cout << "Size of R1CS (after padding):  " << r1cs.n << '\n';
	cout << "Efficiency: " << fixed << setprecision(3) << ((double)n_orig / r1cs.n) << '\n';

	return r1cs;*/

    /*R1CS r1cs(0, 6, 6, { "~one", "~root", "~topic_hash", "~topic_password_hash", "~comment_hash", "~comment_password_hash" });

    // Check that bits are bits
    // b_x * (1 - b_x) = 0
    for (size_t i = 0; i < 31; i++)
    {
        map<string, Fr::Elem> c3[3] = {
            { { "b_" + to_string(i), "1" } },
            { { "~one", "1" }, { "b_" + to_string(i), "-1" } },
            {  }
        };
        add_constraint(c3, r1cs);
    }

    // Check password hash
    mimc_step_number = 0;
    add_mimc5_hash({ "pass" }, "hash_0", r1cs);
    
    // All iterations
    for (size_t i = 0; i < 31; i++)
    {
        map<string, Fr::Elem> c3_1[3] = {
            { { "b_" + to_string(i), "1" } },
            { { "co_hash_" + to_string(i), "1" }, { "hash_" + to_string(i), "-1" } },
            { { "part_" + to_string(i) + "_left", "1" }, { "hash_" + to_string(i), "-1" } }
        };
        add_constraint(c3_1, r1cs);
        map<string, Fr::Elem> c3_2[3] = {
            { { "b_" + to_string(i), "1" } },
            { { "hash_" + to_string(i), "1" }, { "co_hash_" + to_string(i), "-1" } },
            { { "part_" + to_string(i) + "_right", "1" }, { "co_hash_" + to_string(i), "-1" } }
        };
        add_constraint(c3_2, r1cs);
        if (i != 30 )
            add_mimc5_hash({ "part_" + to_string(i) + "_left", "part_" + to_string(i) + "_right" }, "hash_" + to_string(i + 1), r1cs);
        else
            add_mimc5_hash({ "part_" + to_string(i) + "_left", "part_" + to_string(i) + "_right" }, "~root", r1cs);
    }

    // Check comment uniqueness
    add_mimc5_hash({ "pass", "~topic_hash" }, "~topic_password_hash", r1cs);

    // Check comment validity
    add_mimc5_hash({ "pass", "~comment_hash" }, "~comment_password_hash", r1cs);

    //cout << "Size of R1CS (before padding): " << r1cs.n << '\n';
    //size_t n_orig = r1cs.n;

    // Padding to power of 2
    map<string, Fr::Elem> dummy[3] = {
        {  },
        {  },
        {  }
    };

	unsigned long long t1 = r1cs.n & (r1cs.n - 1);
	unsigned long long t2 = t1 & (t1 - 1);
	while (t2 != 0)
	{
		add_constraint(dummy, r1cs);
		t1 = r1cs.n & (r1cs.n - 1);
		t2 = t1 & (t1 - 1);
	}

    //cout << "Size of R1CS (after padding):  " << r1cs.n << '\n';
    //cout << "Efficiency: " << fixed << setprecision(3) << ((double)n_orig / r1cs.n) << '\n';
    
    return r1cs;*/

	/*R1CS r1cs(0, 3, 3, { "~one", "~x", "~y" });

	map<string, Fr::Elem> first_statement[3] = {
			{ { "~x", "1" } },
			{ { "~one", "2" } },
			{ { "tmp_1", "1" } }
	};
	add_constraint(first_statement, r1cs);

	for (int i = 1; i <= 4; i++)
	{
		map<string, Fr::Elem> inter_statement[3] = {
			{ { "tmp_" + to_string(i), "1" } },
			{ { "~one", "2" } },
			{ { "tmp_" + to_string(i+1), "1" } }
		};
		add_constraint(inter_statement, r1cs);
	}

	map<string, Fr::Elem> last_statement[3] = {
			{ { "tmp_5", "1" } },
			{ { "~one", "2" } },
			{ { "~y", "1" } }
	};
	add_constraint(last_statement, r1cs);*/

	// Padding to power of 2
	/*map<string, Fr::Elem> dummy[3] = {
		{  },
		{  },
		{  }
	};

	unsigned long long t1 = r1cs.n & (r1cs.n - 1);
	unsigned long long t2 = t1 & (t1 - 1);
	while (t1 != 0)
	{
		add_constraint(dummy, r1cs);
		t1 = r1cs.n & (r1cs.n - 1);
		t2 = t1 & (t1 - 1);
	}*/

	return r1cs;
}

void R1CSGenerator::add_constraint(map<string, Fr::Elem> constraint[3], R1CS& r1cs)
{
    r1cs.n++;
    
    for (auto var = constraint[0].begin(); var != constraint[0].end(); var++)
    {
        size_t name_pos;
        auto it = r1cs.mV_index.find(var->first);
        if (it != r1cs.mV_index.end())
        {
            name_pos = it->second;
        }
        else
        {
            name_pos = r1cs.m;
            r1cs.m++;
            r1cs.V.push_back(var->first);
            r1cs.mV_index.insert({ var->first, name_pos });
        }
        r1cs.mA[{ name_pos, r1cs.n - 1 }] = var->second;
    }
    for (auto var = constraint[1].begin(); var != constraint[1].end(); var++)
    {
        size_t name_pos;
        auto it = r1cs.mV_index.find(var->first);
        if (it != r1cs.mV_index.end())
        {
            name_pos = it->second;
        }
        else
        {
            name_pos = r1cs.m;
            r1cs.m++;
            r1cs.V.push_back(var->first);
            r1cs.mV_index.insert({ var->first, name_pos });
        }
        r1cs.mB[{ name_pos, r1cs.n - 1 }] = var->second;
    }
    for (auto var = constraint[2].begin(); var != constraint[2].end(); var++)
    {
        size_t name_pos;
        auto it = r1cs.mV_index.find(var->first);
        if (it != r1cs.mV_index.end())
        {
            name_pos = it->second;
        }
        else
        {
            name_pos = r1cs.m;
            r1cs.m++;
            r1cs.V.push_back(var->first);
            r1cs.mV_index.insert({ var->first, name_pos });
        }
        r1cs.mC[{ name_pos, r1cs.n - 1 }] = var->second;
    }
}

void R1CSGenerator::add_mimc5_round(size_t round_number, map<string, Fr::Elem> input, R1CS& r1cs)
{
    map<string, Fr::Elem> c1[3] = {
        input,
        input,
        { { "pow2_" + to_string(mimc_step_number) + "_" + to_string(round_number), "1" } }
    };
    add_constraint(c1, r1cs);

    map<string, Fr::Elem> c2[3] = {
        { { "pow2_" + to_string(mimc_step_number) + "_" + to_string(round_number), "1" } },
        input,
        { { "pow3_" + to_string(mimc_step_number) + "_" + to_string(round_number), "1" } }
    };
    add_constraint(c2, r1cs);

    map<string, Fr::Elem> c3[3] = {
        { { "pow2_" + to_string(mimc_step_number) + "_" + to_string(round_number), "1" } },
        { { "pow3_" + to_string(mimc_step_number) + "_" + to_string(round_number), "1" } },
        { { "mimc_" + to_string(mimc_step_number) + "_" + to_string(round_number), "1" } }
    };
    add_constraint(c3, r1cs);
}

void R1CSGenerator::add_mimc5_step(map<string, Fr::Elem> input, R1CS& r1cs)
{
    input["~one"] = mimc5->m_pRoundConstants[0];
    add_mimc5_round(0, input, r1cs);
    for (size_t i = 1; i < mimc5->m_uiNumberOfRounds; i++)
    {
        map<string, Fr::Elem> in = { { "mimc_" + to_string(mimc_step_number) + "_" + to_string(i - 1), "1" }, 
                                                { "~one", mimc5->m_pRoundConstants[i] } };
        add_mimc5_round(i, in, r1cs);
    }
    mimc_step_number++;
}

void R1CSGenerator::add_mimc5_hash_step(string v1, const Fr::Elem& k1, string v2, const Fr::Elem& k2, string w1, R1CS& r1cs)
{
    map<string, Fr::Elem> x1 = {
        { v1, /*Fr::Elem(1) * */k1 },
        { v2, /*Fr::Elem(2) * */k2 }
    };
	Fr::Dbl(x1[v2], x1[v2]);
    add_mimc5_step(x1, r1cs);

    map<string, Fr::Elem> x2 = {
        { v1, /*Fr::Elem(2) * */k1 },
        { v2, /*Fr::Elem(2) * */k2 },
        { "mimc_" + to_string(mimc_step_number - 1) + "_160", "1" }
    };
	Fr::Dbl(x2[v1], x2[v1]);
	Fr::Dbl(x2[v2], x2[v2]);
    add_mimc5_step(x2, r1cs);

    map<string, Fr::Elem> x3 = {
        { v1, /*Fr::Elem(2) * */k1 },
        { v2, /*Fr::Elem(1) * */k2 },
        { "mimc_" + to_string(mimc_step_number - 1) + "_160", "1" }
    };
	Fr::Dbl(x3[v1], x3[v1]);
    add_mimc5_step(x3, r1cs);

    // Final check
    // TODO! Remove this check (it can be combined with the previous one)
    map<string, Fr::Elem> c[3] = {
        { { v1, /*Fr::Elem(1) * */k1 },
          { "mimc_" + to_string(mimc_step_number - 3) + "_160", "1" },
          { "mimc_" + to_string(mimc_step_number - 2) + "_160", "1" },
          { "mimc_" + to_string(mimc_step_number - 1) + "_160", "2" },
          { w1, "1" } },
        { { "~one", "1" } },
        { { w1, "2" } }
    };
    add_constraint(c, r1cs);
}

void R1CSGenerator::add_mimc5_hash(vector<string> blocks, string w1, R1CS& r1cs)
{
    if (blocks.size() == 1)
    {
        add_mimc5_hash_step("~one", mimc5->m_initialValue, blocks[0], "1", w1, r1cs);
        return;
    }

    for (size_t i = 0; i < blocks.size(); i++)
    {
        if (i == 0)
            add_mimc5_hash_step("~one", mimc5->m_initialValue, blocks[i], "1", "mimc_" + to_string(mimc_step_number + 2), r1cs);
        else if (i == blocks.size() - 1)
            add_mimc5_hash_step("mimc_" + to_string(mimc_step_number - 1), "1", blocks[i], "1", w1, r1cs);
        else
            add_mimc5_hash_step("mimc_" + to_string(mimc_step_number - 1), "1", blocks[i], "1", "mimc_" + to_string(mimc_step_number + 2), r1cs);
    }
}

bool R1CSGenerator::is_power_of_2(size_t n)
{
    return ((n & (n - 1)) == 0);
}

string GetBitName(string variable_name, int bit_number)
{
	if (variable_name[0] == '~')
	{
		variable_name = variable_name.substr(1);
	}
	return variable_name + "_b_" + to_string(bit_number);
}

void R1CSGenerator::add_limb_check(size_t limb_size, const string& limb, R1CS& r1cs)
{
	// b_0 + 2*b_1 + 4*b_2 + ... = limb
	map<string, Fr::Elem> c[3] = {
		{  },
		{ { "~one", "1" } },
		{ { limb, "1" } }
	};

	// Check that bits are bits
	// b_x * (1 - b_x) = 0
	Fr::Elem cc = "1";
	for (size_t i = 0; i < limb_size; i++)
	{
		map<string, Fr::Elem> c3[3] = {
			{ { GetBitName(limb, i), "1" } },
			{ { "~one", "1" }, { GetBitName(limb, i), "-1" } },
			{  }
		};
		add_constraint(c3, r1cs);
		c[0][GetBitName(limb, i)] = cc;
		Fr::Dbl(cc, cc);
	}
	add_constraint(c, r1cs);
}

void R1CSGenerator::add_big_number_check(size_t limb_size, size_t limb_count, const string & number_name, vector<string>& number_limbs, R1CS & r1cs)
{
	for (size_t i = 0; i < limb_count; i++)
	{
		string limb_name = number_name + "_limb_" + to_string(i);
		add_limb_check(limb_size, limb_name, r1cs);
		number_limbs.push_back(limb_name);
	}
}

// TODO! Remove this!
// A_limbs + B_limbs = C_limbs
void R1CSGenerator::add_add_check(size_t limb_count, const vector<string>& A_limbs, const vector<string>& B_limbs, const vector<string>& C_limbs, R1CS & r1cs)
{
	for (size_t i = 0; i < limb_count; i++)
	{
		map<string, Fr::Elem> c3[3] = {
			{ { A_limbs[i], "1" }, { B_limbs[i], "1" } },
			{ { "~one", "1" } },
			{ { C_limbs[i], "1" } }
		};
		add_constraint(c3, r1cs);
	}
}

// A_limbs * B_limbs = C_limbs
void R1CSGenerator::add_mul_check(size_t limb_count, const vector<string>& A_limbs, const vector<string>& B_limbs, const vector<string>& C_limbs, R1CS & r1cs)
{
	for (size_t c = 1; c < 2 * limb_count; c++)
	{
		map<string, Fr::Elem> c3[3] = {
			{  },
			{  },
			{  }
		};
		Fr::Elem c0 = to_string(c).c_str();
		Fr::Elem cc = "1";
		for (size_t i = 0; i < 2 * limb_count; i++)
		{
			if (i < limb_count)
			{
				c3[0][A_limbs[i]] = cc;
				c3[1][B_limbs[i]] = cc;
			}
			c3[2][C_limbs[i]] = cc;
			Fr::Mul(cc, cc, c0);
		}
		add_constraint(c3, r1cs);
	}
}

// Important! *B_limbs* must be normalized and checked before call
// Every limb in *A_limbs* is a sum of not more than *limb_count* multiplications (and maybe one or two more limb) so it's bit size in not more than 2 * limb_size + 1 + |log2(limb_count)|  (rounded up) 
void R1CSGenerator::add_norm_check(size_t limb_size, size_t limb_count, const vector<string>& A_limbs, const vector<string>& B_limbs, R1CS & r1cs)
{
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
		// Check that bits are bits
		for (size_t j = 0; j < bit_size; j++)
		{
			map<string, Fr::Elem> c3[3] = {
				{ { GetBitName(A_limbs[i], j), "1" } },
				{ { "~one", "1" }, { GetBitName(A_limbs[i], j), "-1" } },
				{  }
			};
			add_constraint(c3, r1cs);
		}

		// Check that lowest bits form B_limbs[i]
		map<string, Fr::Elem> c3_1[3] = {
			{  },
			{ { "~one", "1" } },
			{ { B_limbs[i], "1" } }
		};
		Fr::Elem cc = "1";
		for (size_t j = 0; j < limb_size; j++)
		{
			c3_1[0][GetBitName(A_limbs[i], j)] = cc;
			Fr::Dbl(cc, cc);
		}
		add_constraint(c3_1, r1cs);

		// Check that all bits (plus previous ones) form A_limbs[i]
		map<string, Fr::Elem> c3_2[3] = {
			{  },
			{ { "~one", "1" } },
			{ { A_limbs[i], "1" } }
		};
		Fr::Elem cc0 = "1";
		Fr::Elem cc1 = "1";
		for (size_t j = 0; j < bit_size; j++)
		{
			c3_2[0][GetBitName(A_limbs[i], j)] = cc0;
			Fr::Dbl(cc0, cc0);
			if ((i > 0) && (j >= limb_size))
			{
				c3_2[2][GetBitName(A_limbs[i - 1], j)] = cc1;
				Fr::Dbl(cc1, cc1);
			}
		}
		add_constraint(c3_2, r1cs);
	}

	// Hmmm... That's all?
}

// All limbs must be normalized and checked
// A_limbs * B_limbs = Q_limbs * M_limbs + R_limbs = Result_limbs
// TODO !!! Check that R_limbs < M_limbs !!!
// Let b0[...] be the bits of M_limbs, b1[...] be the bits of R_limbs,
// bitcnt be the count of bits, mem[bitcnt] = 1
// for i in { bitcnt - 1, bitcnt - 2, ..., 1, 0 }
//    mem[i] = mem[i+1]*(1 - b0[i] + b1[i])
//    mem[i] * (1 - mem[i]) = 0
void R1CSGenerator::add_mul_mod_check(size_t limb_size, size_t limb_count, const vector<string>& A_limbs, const vector<string>& B_limbs, const vector<string>& M_limbs, const vector<string>& Q_limbs, const vector<string>& R_limbs, const vector<string>& Result_limbs, R1CS& r1cs)
{
	// The plot is relatively simple:
	// 1. A_limbs * B_limbs = Mul1_limbs
	// 2. Q_limbs * M_limbs = Mul2_limbs
	// 3. Mul2_limbs + R_limbs = Add_limbs
	// 4. Norm(Mul1_limbs) = Result_limbs
	// 5. Norm(Add_limbs) = Result_limbs

	size_t tmp = limb_count;
	size_t log2_limb_count = 0;
	while (tmp > 0)
	{
		tmp >>= 1;
		log2_limb_count++;
	}
	size_t bit_size = 2 * limb_size + log2_limb_count + 1;

	// Prepare unique names
	vector<string> Mul1_limbs, Mul2_limbs, Add_limbs;
	for (size_t i = 0; i < limb_count; i++)
	{
		Mul1_limbs.push_back("mul_" + to_string(mul_step_number) + "_limb_" + to_string(i));
		Mul2_limbs.push_back("mul_" + to_string(mul_step_number + 1) + "_limb_" + to_string(i));
		Add_limbs.push_back("add_" + to_string(add_step_number) + "_limb_" + to_string(i));
	}
	for (size_t i = limb_count; i < bit_size; i++)
	{
		Mul1_limbs.push_back("mul_" + to_string(mul_step_number) + "_limb_" + to_string(i));
		Mul2_limbs.push_back("mul_" + to_string(mul_step_number + 1) + "_limb_" + to_string(i));
		Add_limbs.push_back("mul_" + to_string(mul_step_number + 1) + "_limb_" + to_string(i));
	}
	mul_step_number += 2;
	add_step_number++;

	// Check 1
	add_mul_check(limb_count, A_limbs, B_limbs, Mul1_limbs, r1cs);

	// Check 2
	add_mul_check(limb_count, Q_limbs, M_limbs, Mul2_limbs, r1cs);

	// Check 3
	add_add_check(limb_count, Mul2_limbs, R_limbs, Add_limbs, r1cs);

	// Check 4
	add_norm_check(limb_size, limb_count, Mul1_limbs, Result_limbs, r1cs);

	// Check 5
	add_norm_check(limb_size, limb_count, Add_limbs, Result_limbs, r1cs);

	// Profit???
}


