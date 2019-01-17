//#define MULTICORE
#define USE_MIXED_ADDITION
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <ctime>
#include <omp.h>
#include <map>

#include "Structures.h"

#include "mimc5.h"
#include "r1cs.h"
#include "merkle.h"
#include "picosha2.h"

#include "ModMultiplier.h"
#include "RSA.h"

using namespace std;
using namespace ec_lib;

shared_ptr<MiMC5> mimc5;// = MiMC5::GetInstance();
MerkleTree* mt;
Fr::Elem pass;
size_t pass_pos;
Fr::Elem topic_hash, pass_topic_hash;
Fr::Elem comment_hash, pass_comment_hash;
mpz_t n, msg, sig;
vector<Fr::Elem> n_limbs;

vector<Fr::Elem> GetWitness(const vector<string>& V)
{
	size_t step = 0;
	map<string, Fr::Elem> wit_map = {
		{ "~one", "1" },
		{ "~root", mt->Root() },
		{ "~topic_hash", topic_hash },
		{ "~topic_password_hash", pass_topic_hash },
		{ "~comment_hash", comment_hash },
		{ "comment_hash", comment_hash },
		{ "pass", pass }
	};

	Fr::Elem hash = mimc5->Hash_witness({ pass }, wit_map, step);
	wit_map["hash_0"] = hash;

	pair<vector<Fr::Elem>, size_t> path = mt->Path(hash);
	for (size_t i = 0; i < 31; i++)
	{
		wit_map["co_hash_" + to_string(i)] = path.first[i];
		if (path.second & (1 << i))
		{
			wit_map["b_" + to_string(i)] = "1";
			wit_map["part_" + to_string(i) + "_left"] = path.first[i];
			wit_map["part_" + to_string(i) + "_right"] = hash;
			hash = mimc5->Hash_witness({ path.first[i], hash }, wit_map, step);
		}
		else
		{
			wit_map["b_" + to_string(i)] = "0";
			wit_map["part_" + to_string(i) + "_left"] = hash;
			wit_map["part_" + to_string(i) + "_right"] = path.first[i];
			hash = mimc5->Hash_witness({ hash, path.first[i] }, wit_map, step);
		}
		wit_map["hash_" + to_string(i + 1)] = hash;
	}

	Fr::Elem hash_1 = mimc5->Hash_witness({ pass, topic_hash }, wit_map, step);
	wit_map["~topic_password_hash"] = hash_1;

	// Check RSA knowledge
	mpz_t sig2, sig4, sig8, sig16, sig17;
	/*mpz_roinit_n(msg, e1.limbs, Fr::uiLimbCount);
	RSA::KeyGen(n, d);
	char buf[10000];
	cout << "RSA params:\n";
	mpz_get_str(buf, 10, n);
	cout << "n = " << buf << '\n';
	mpz_get_str(buf, 10, d);
	cout << "d = " << buf << '\n';
	cout << "e = 17\n";
	cout << "msg = " << Fr::ToString(e1) << '\n';*/

	ModMultiplier::MulModGetWitness(sig, sig, n, "Sig", "Sig", "~N", "Sig2Q", "Sig2R", "Sig2", sig2, wit_map);
	ModMultiplier::MulModGetWitness(sig2, sig2, n, "Sig2R", "Sig2R", "~N", "Sig4Q", "Sig4R", "Sig4", sig4, wit_map);
	ModMultiplier::MulModGetWitness(sig4, sig4, n, "Sig4R", "Sig4R", "~N", "Sig8Q", "Sig8R", "Sig8", sig8, wit_map);
	ModMultiplier::MulModGetWitness(sig8, sig8, n, "Sig8R", "Sig8R", "~N", "Sig16Q", "Sig16R", "Sig16", sig16, wit_map);
	ModMultiplier::MulModGetWitness(sig16, sig, n, "Sig16R", "Sig", "~N", "MsgQ", "MsgR", "Msg", sig17, wit_map);
	cout << ((mpz_cmp(sig17, msg) == 0) ? ("RSA: correct") : ("RSA: incorrect")) << '\n';

	// Message correctness
	Fr::Elem hash_2 = mimc5->Hash_witness({ wit_map["hash_0"], topic_hash }, wit_map, step);
	wit_map["msg_to_sign"] = hash_2;

	// Build actual witness
	vector<Fr::Elem> wit;
	for (auto it = V.begin(); it != V.end(); it++)
	{
		wit.push_back(wit_map[*it]);
	}
	return wit;


	/*size_t step = 0;
	map<string, Fr::Elem> wit_map = {
		{ "~one", "1" },
		{ "~v1", v1 },
		{ "~v2", v2 },
		{ "~v3", v3 },
		{ "~v4", v4 },
		{ "~v5", v5 },
		{ "~v6", v6 },
		{ "~v7", v7 },
		{ "~v8", v8 },
		{ "~v9", v9 }
	};
	Fr::Elem w1 = mimc5->Hash_witness({ v1, v2, v3, v4, v5, v6, v7, v8, v9 }, wit_map, step);
	wit_map["~w1"] = w1;

	vector<Fr::Elem> wit;
	for (auto it = V.begin(); it != V.end(); it++)
	{
		wit.push_back(wit_map[*it]);
	}
	return wit;*/

	/*map<string, Fr::Elem> wit_map = {
		{ "~one", "1" }
	};*/
	/*mpz_t A, B, M, Res;
	mpz_init_set_str(A, "3256220842970565771203998651127506681284583681451128321203627319018180360683767273991759264354167067418252054783174454439328873500390065918390562834886917812216839983533756620413323551746427771815717344858747132593310656580463507964691789507027659280087687038950945857054408793208456146770717198919838165958", 10);
	mpz_init_set_str(B, "34125947510035115716848054395458934311291763253594937886109737835528652838706898991823921127716885345306805059718182541521593935155731562956474115643091704648677376954912544477642977118252492686807760474911884146133112188194878594415792952742024232950823356677892109721099991892344409299038939281207575532915", 10);
	mpz_init_set_str(M, "48680452413382204781786575693421489912267420018010652624425493809734451868484368484648960902005243533876562207339168296315410347618239053918502240576122277762672946730850971346574649789178072166391134388100539157190788326042522554493702491903848038035429106896307691240155367387447949281301431206519798791067", 10);
	ModMultiplier::MulModGetWitness(A, A, M, "A", "A", "M", "Q", "R", "Res", Res, wit_map);*/
	/*Fr::Elem e1;
	Fr::GetRandomElement(e1);
	mpz_t msg, n, d, sig, sig2, sig4, sig8, sig16, sig17;
	mpz_roinit_n(msg, e1.limbs, Fr::uiLimbCount);
	RSA::KeyGen(n, d);
	char buf[10000];
	cout << "RSA params:\n";
	mpz_get_str(buf, 10, n);
	cout << "n = " << buf << '\n';
	mpz_get_str(buf, 10, d);
	cout << "d = " << buf << '\n';
	cout << "e = 17\n";
	cout << "msg = " << Fr::ToString(e1) << '\n';


	RSA::Sign(n, d, msg, sig);
	ModMultiplier::MulModGetWitness(sig, sig, n, "Sig", "Sig", "N", "Sig2Q", "Sig2R", "Sig2", sig2, wit_map);
	ModMultiplier::MulModGetWitness(sig2, sig2, n, "Sig2R", "Sig2R", "N", "Sig4Q", "Sig4R", "Sig4", sig4, wit_map);
	ModMultiplier::MulModGetWitness(sig4, sig4, n, "Sig4R", "Sig4R", "N", "Sig8Q", "Sig8R", "Sig8", sig8, wit_map);
	ModMultiplier::MulModGetWitness(sig8, sig8, n, "Sig8R", "Sig8R", "N", "Sig16Q", "Sig16R", "Sig16", sig16, wit_map);
	ModMultiplier::MulModGetWitness(sig16, sig, n, "Sig16R", "Sig", "N", "MsgQ", "MsgR", "Msg", sig17, wit_map);
	cout << ((mpz_cmp(sig17, msg) == 0) ? ("RSA: correct") : ("RSA: incorrect")) << '\n';

	vector<Fr::Elem> wit;
	for (auto it = V.begin(); it != V.end(); it++)
	{
		wit.push_back(wit_map[*it]);
	}
	return wit;*/

	/*size_t step = 0;
	map<string, Fr::Elem> wit_map = {
		{ "~one", "1" },
		{ "~root", mt->Root() },
		{ "~topic_hash", topic_hash },
		{ "~topic_password_hash", pass_topic_hash },
		{ "~comment_hash", comment_hash },
		{ "~comment_password_hash", pass_comment_hash },
		{ "pass", pass }
	};
	Fr::Elem hash = mimc5->Hash_witness({ pass }, wit_map, step);
	wit_map["hash_0"] = hash;

	pair<vector<Fr::Elem>, size_t> path = mt->Path(hash);
	for (size_t i = 0; i < 31; i++)
	{
		wit_map["co_hash_" + to_string(i)] = path.first[i];
		if (path.second & (1 << i))
		{
			wit_map["b_" + to_string(i)] = "1";
			wit_map["part_" + to_string(i) + "_left"] = path.first[i];
			wit_map["part_" + to_string(i) + "_right"] = hash;
			hash = mimc5->Hash_witness({ path.first[i], hash }, wit_map, step);
		}
		else
		{
			wit_map["b_" + to_string(i)] = "0";
			wit_map["part_" + to_string(i) + "_left"] = hash;
			wit_map["part_" + to_string(i) + "_right"] = path.first[i];
			hash = mimc5->Hash_witness({ hash, path.first[i] }, wit_map, step);
		}
		wit_map["hash_" + to_string(i + 1)] = hash;
	}

	Fr::Elem hash_1 = mimc5->Hash_witness({ pass, topic_hash }, wit_map, step);
	wit_map["~topic_password_hash"] = hash_1;

	Fr::Elem hash_2 = mimc5->Hash_witness({ pass, comment_hash }, wit_map, step);
	wit_map["~comment_password_hash"] = hash_2;

	vector<Fr::Elem> wit;
	for (auto it = V.begin(); it != V.end(); it++)
	{
		wit.push_back(wit_map[*it]);
	}
	return wit;*/

	/*map<string, Fr::Elem> wit_map = {
		{ "~one", "1" },
		{ "~x", "2" },
		{ "~y", "128" },
		{ "tmp_1", "4" },
		{ "tmp_2", "8" },
		{ "tmp_3", "16" },
		{ "tmp_4", "32" },
		{ "tmp_5", "64" }
	};

	vector<Fr::Elem> wit;
	for (auto it = V.begin(); it != V.end(); it++)
	{
		wit.push_back(wit_map[*it]);
	}
	return wit;*/
}

void CheckWitness(R1CS& r1cs, const vector<Fr::Elem> wit)
{
	for (size_t i = 164034/*r1cs.n - 1*/; i >= 0; i--)
	{
		if (i > 0 && i % 100 == 0)
			cout << i << " / " << r1cs.n << '\n' << flush;

		Fr::Elem a("0");
		Fr::Elem b("0");
		Fr::Elem c("0");
		Fr::Elem tmp;
		map<string, string> vals;
		for (size_t j = 0; j < r1cs.m; j++)
		{
			if (r1cs.mA.count({ j, i }))
			{
				//a += wit[j] * r1cs.mA[{ j, i }];
				Fr::Mul(tmp, wit[j], r1cs.mA[{ j, i }]);
				Fr::Add(a, a, tmp);
			}
			if (r1cs.mB.count({ j, i }))
			{
				//b += wit[j] * r1cs.mB[{ j, i }];
				Fr::Mul(tmp, wit[j], r1cs.mB[{ j, i }]);
				Fr::Add(b, b, tmp);
			}
			if (r1cs.mC.count({ j, i }))
			{
				//c += wit[j] * r1cs.mC[{ j, i }];
				Fr::Mul(tmp, wit[j], r1cs.mC[{ j, i }]);
				Fr::Add(c, c, tmp);
			}
		}
		Fr::Mul(tmp, a, b);
		//if (a * b != c)
		if (!Fr::CheckEqual(tmp, c))
		{
			cout << "Error in statement " << i << ":\n";
			cout << '(';
			bool fl = false;
			for (size_t j = 0; j < r1cs.m; j++)
				if (r1cs.mA.count({ j, i }))
				{
					if (fl)
						cout << " + ";
					else
						fl = true;
					//r1cs.mA[{ j, i }].as_bigint().print();
					cout << Fr::ToString(r1cs.mA[{ j, i }]);
					cout << '*' << r1cs.V[j];
					vals[r1cs.V[j]] = Fr::ToString(wit[j]);
				}
			cout << ") * (";
			fl = false;
			for (size_t j = 0; j < r1cs.m; j++)
				if (r1cs.mB.count({ j, i }))
				{
					if (fl)
						cout << " + ";
					else
						fl = true;
					//r1cs.mB[{ j, i }].as_bigint().print();
					cout << Fr::ToString(r1cs.mB[{ j, i }]);
					cout << '*' << r1cs.V[j];
					vals[r1cs.V[j]] = Fr::ToString(wit[j]);
				}
			cout << ") = ";
			fl = false;
			for (size_t j = 0; j < r1cs.m; j++)
				if (r1cs.mC.count({ j, i }))
				{
					if (fl)
						cout << " + ";
					else
						fl = true;
					//r1cs.mC[{ j, i }].as_bigint().print();
					cout << Fr::ToString(r1cs.mC[{ j, i }]);
					cout << '*' << r1cs.V[j];
					vals[r1cs.V[j]] = Fr::ToString(wit[j]);
				}
			for (auto it = vals.begin(); it != vals.end(); it++)
			{
				cout << '\n' << it->first << " = " << it->second;
			}
			cout << '\n';
			break;
		}
	}
	cout.flush();
}

void CheckMerkleTree()
{
	MerkleTree mt("tree.bin", "index.bin");
	vector<Fr::Elem> vValuesIn, vValuesOut;
	Fr::Elem tmp;
	for (size_t i = 0; i < 27000; i++)
	{
		Fr::GetRandomElement(tmp);
		vValuesIn.push_back(tmp);
		if ((i + 1) % 1000 == 0)
		{
			cout << '.';
			cout.flush();
		}
	}
	cout << '\n';
	for (size_t i = 0; i < 27000; i++)
	{
		//Fr::Elem val;
		////do
		//{
		//	val = Fr::Elem::random_element();
		//}
		////while (find(vValuesIn.begin(), vValuesIn.end(), val) != vValuesIn.end());

		Fr::GetRandomElement(tmp);
		vValuesOut.push_back(tmp);
		if ((i + 1) % 1000 == 0)
		{
			cout << '.';
			cout.flush();
		}
	}
	cout << '\n';
	for (size_t i = 0; i < 27000; i++)
	{
		mt.Add(vValuesIn[i]);
		if ((i + 1) % 1000 == 0)
		{
			cout << '.';
			cout.flush();
		}
	}
	cout << '\n';
	for (size_t i = 0; i < 27000; i++)
	{
		if (!mt.Check(vValuesIn[i]))
		{
			cout << "ERROR!!! 0 " << i << '\n';
			return;
		}
		if ((i + 1) % 1000 == 0)
		{
			cout << '.';
			cout.flush();
		}
	}
	cout << '\n';
	for (size_t i = 0; i < 27000; i++)
	{
		if (mt.Check(vValuesOut[i]))
		{
			cout << "ERROR!!! 1 " << i << '\n';
			return;
		}
		if ((i + 1) % 1000 == 0)
		{
			cout << '.';
			cout.flush();
		}
	}
	cout << '\n';
	Fr::Elem val = vValuesIn[12345];
	pair<vector<Fr::Elem>, size_t> path = mt.Path(val);
	Fr::Elem root = mt.Root();
	for (size_t i = 0; i < 31; i++)
		if (path.second & (1 << i))
			val = mimc5->Hash({ path.first[i], val });
		else
			val = mimc5->Hash({ val, path.first[i] });
	//if (val != root)
	if (!Fr::CheckEqual(val, root))
	{
		cout << "ERROR!!! 2";
		return;
	}
}

struct ProvingKey
{
	vector<EC::Elem> A1;
	vector<EC::Elem> A2;
	vector<ECtw::Elem> B1;
	vector<EC::Elem> B2;
	vector<EC::Elem> C1;
	vector<EC::Elem> C2;
	vector<EC::Elem> K;
	vector<EC::Elem> H;
	vector<string> V;

	void Export(string file)
	{
		ofstream fout(file, ios::binary);

		/*fout << V.size() << '\n';
		for (auto it = V.begin(); it != V.end(); it++)
			fout << *it << '\n';*/
		unsigned char tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp = (V.size() >> (8 * i)) & 0xff;
			fout.write((const char*)&tmp, 1);
		}
		for (auto it = V.begin(); it != V.end(); it++)
		{
			fout.write(it->c_str(), it->length() + 1);
		}

		fout << A1 << A2 << B1 << B2 << C1 << C2 << K << H;

		fout.close();
	}

	void Import(string file)
	{
		size_t size;
		ifstream fin(file, ios::binary);

		/*fin >> size;
		V.resize(size);
		for (size_t i = 0; i < size; i++)
			fin >> V[i];*/
		unsigned int V_size = 0;
		unsigned char tmp;
		for (int i = 0; i < 4; i++)
		{
			fin.read((char*)&tmp, 1);
			V_size |= (unsigned int)tmp << (8 * i);
		}
		V.resize(V_size);
		for (size_t i = 0; i < V_size; i++)
		{
			fin.read((char*)&tmp, 1);
			while (tmp != 0)
			{
				V[i].push_back(tmp);
				fin.read((char*)&tmp, 1);
			}
		}

		fin >> A1 >> A2 >> B1 >> B2 >> C1 >> C2 >> K >> H;

		fin.close();
	}
};

struct VerifyingKey
{
	ECtw::Elem A;
	EC::Elem B;
	ECtw::Elem C;
	ECtw::Elem Gamma;
	EC::Elem BetaGamma1;
	ECtw::Elem BetaGamma2;
	ECtw::Elem T;
	vector<EC::Elem> IC;

	void Export(string file)
	{
		ofstream fout(file, ios::binary);

		fout << A << B << C << Gamma << BetaGamma1 << BetaGamma2 << T;

		fout << IC;
	}

	void Import(string file)
	{
		size_t size;
		ifstream fin(file, ios::binary);

		fin >> A >> B >> C >> Gamma >> BetaGamma1 >> BetaGamma2 >> T;

		fin >> IC;

		fin.close();
	}
};

Fr::Elem L[kSmallDomainSize];
//Fr::Elem A_val[kBigDomainSize], B_val[kBigDomainSize], C_val[kBigDomainSize], T_val[kBigDomainSize], H_val[kBigDomainSize];
//Fr::Elem A_coef[kBigDomainSize], B_coef[kBigDomainSize], C_coef[kBigDomainSize], T_coef[kBigDomainSize], H_coef[kBigDomainSize];
Fr::Elem buf1[kBigDomainSize], buf2[kBigDomainSize], buf3[kBigDomainSize], buf4[kBigDomainSize];
Power power_s[kBigDomainSize], power_h[kBigDomainSize];
ProvingKey pk;
R1CS r1cs;

void a1_work(EC::Elem* piA1)
{
	EC::MultiMul(*piA1, &pk.A1[r1cs.input_size], &power_s[r1cs.input_size], pk.A1.size() - r1cs.input_size);
}

void a2_work(EC::Elem* piA2)
{
	EC::MultiMul(*piA2, &pk.A2[r1cs.input_size], &power_s[r1cs.input_size], pk.A2.size() - r1cs.input_size);
}

void b1_work(ECtw::Elem* piB1)
{
	ECtw::MultiMul(*piB1, &pk.B1[0], power_s, pk.B1.size());
}

void b2_work(EC::Elem* piB2)
{
	EC::MultiMul(*piB2, &pk.B2[0], power_s, pk.B2.size());
}

void c1_work(EC::Elem* piC1)
{
	EC::MultiMul(*piC1, &pk.C1[0], power_s, pk.C1.size());
}

void c2_work(EC::Elem* piC2)
{
	EC::MultiMul(*piC2, &pk.C2[0], power_s, pk.C2.size());
}

void k_work(EC::Elem* piK)
{
	EC::MultiMul(*piK, &pk.K[0], power_s, pk.K.size());
}

void h_work(EC::Elem* piH)
{
	EC::MultiMul(*piH, &pk.H[0], power_h, pk.H.size());
}

int main()
{
	mimc5 = MiMC5::GetInstance();
	mt = new MerkleTree("tree.bin", "index.bin");

	clock_t start1, start2;
	//libff::inhibit_profiling_info = true;
	//libff::inhibit_profiling_counters = true;

	/*Fr::Elem e1, e2, e3;
	Fr::GetRandomElement(e1);
	Fr::GetRandomElement(e2);
	cout << "Checking addition...\n";
	start2 = clock();
	for (size_t i = 0; i < 10000000; i++)
	{
		Fr::Add(e3, e1, e2);
	}
	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();
	cout << "Checking multiplication...\n";
	start2 = clock();
	for (size_t i = 0; i < 10000000; i++)
	{
		Fr::Mul(e3, e1, e2);
	}
	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();
	cout << "Checking division...\n";
	start2 = clock();
	for (size_t i = 0; i < 10000000; i++)
	{
		Fr::Div(e3, e1, e2);
	}
	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();*/

	/*Fr::Elem e1;
	Fr::GetRandomElement(e1);
	mpz_t msg, n, d, sig;
	mpz_roinit_n(msg, e1.limbs, Fr::uiLimbCount);
	RSA::KeyGen(n, d);
	RSA::Sign(n, d, msg, sig);
	bool res = RSA::Verify(n, msg, sig);
	cout << ((res) ? ("RSA: correct") : ("RSA: incorrect")) << '\n';
	res = RSA::Verify(n, msg, msg);
	cout << ((res) ? ("RSA: incorrect") : ("RSA: correct")) << '\n';*/

	cout << "Creating Merkle tree...\n";
	cout.flush();
	start2 = clock();
	srand(time(nullptr));
	pass_pos = rand() % 100;
	Fr::Elem tmp, tmp1;
	for (size_t i = 0; i < 100; i++)
	{
		if (i != pass_pos)
		{
			Fr::GetRandomElement(tmp);
			mt->Add(tmp);
		}
		else
		{
			Fr::GetRandomElement(pass);
			mt->Add(mimc5->Hash({ pass }));
		}
	}
	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	/*
	 *  _____
	 *   RSA
	 *  ¯¯¯¯¯
	 */

	cout << "Creating RSA data...\n";
	start1 = clock();

	mpz_t d;
	RSA::KeyGen(n, d);
	Fr::GetRandomElement(topic_hash);
	Fr::GetRandomElement(comment_hash);
	Fr::Elem msg_to_sign = mimc5->Hash({ mimc5->Hash({ pass }), topic_hash });
	//mpz_roinit_n(msg, msg_to_sign.limbs, Fr::uiLimbCount);
	mpz_init_set_str(msg, Fr::ToString(msg_to_sign).c_str(), 10);
	RSA::Sign(n, d, msg, sig);

	mpz_t Q, R;
	mpz_init_set(Q, n);
	mpz_init(R);
	char buf[10000];
	for (size_t i = 0; i < 16; i++)
	{
		mpz_tdiv_r_2exp(R, Q, 64);
		mpz_tdiv_q_2exp(Q, Q, 64);
		mpz_get_str(buf, 10, R);
		Fr::Elem tmp = buf;
		n_limbs.push_back(tmp);
	}

	cout << "RSA data generation finished\n";
	cout << "Elapsed time: " << fixed << setprecision(3) << double(clock() - start1) / CLOCKS_PER_SEC << " sec\n";
	cout << '\n';
	cout.flush();

	/*
	 *  ______
	 *   R1CS
	 *  ¯¯¯¯¯¯
	 */

	cout << "R1CS generation started\n";
	start1 = clock();

	//libff::alt_bn128_pp::init_public_params();

	cout << "Creating R1CS...\n";
	cout.flush();
	start2 = clock();

	r1cs = R1CSGenerator::Generate();

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "Hashing...\n";
	cout.flush();
	start2 = clock();

	//topic_hash = Fr::Elem::random_element();
	pass_topic_hash = mimc5->Hash({ pass, topic_hash });
	//comment_hash = Fr::Elem::random_element();
	pass_comment_hash = mimc5->Hash({ pass, comment_hash });

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "R1CS generation finished\n";
	cout << "Elapsed time: " << fixed << setprecision(3) << double(clock() - start1) / CLOCKS_PER_SEC << " sec\n";
	cout << '\n';
	cout.flush();

	/*
	 *  ________________
	 *   Key Generation
	 *  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
	 */
	cout << "Key generation started\n";
	cout.flush();
	start1 = clock();

	// Toxic waste generation
	Fr::Elem tau, rho_a, rho_b, alpha_a, alpha_b, alpha_c, beta, gamma;
	Fr::GetRandomElement(tau);
	Fr::GetRandomElement(rho_a);
	Fr::GetRandomElement(rho_b);
	Fr::GetRandomElement(alpha_a);
	Fr::GetRandomElement(alpha_b);
	Fr::GetRandomElement(alpha_c);
	Fr::GetRandomElement(beta);
	Fr::GetRandomElement(gamma);

	EC::Elem zero_1, one_1, tmp_1;
	EC::SetZero(zero_1);
	EC::SetOne(one_1);
	ECtw::Elem zero_2, one_2, tmp_2;
	ECtw::SetZero(zero_2);
	ECtw::SetOne(one_2);

	/*cout << "Creating proving key...\n";
	cout.flush();
	start2 = clock();

	// Lagrange polynomial evaluation
	//shared_ptr<libfqfft::evaluation_domain<Fr::Elem>> domain = libfqfft::get_evaluation_domain<Fr::Elem>(r1cs.n);
	//vector<Fr::Elem> L = domain->evaluate_all_lagrange_polynomials(tau);
	SmallDomain::EvaluateLagrangePolynomials(L, tau);
	//Fr::Elem T_tau = domain->compute_vanishing_polynomial(tau);
	Fr::Elem T_tau;
	SmallDomain::EvaluateVanishingPolynomial(T_tau, tau);

	// Proving key evaluation  [ pk = (pkA1, pkA2, pkB1, pkB2, pkC1, pkC2, pkK, pkH) ]
	ProvingKey pk;
	vector<Fr::Elem> AA(r1cs.m, "0");
	for (auto it = r1cs.mA.begin(); it != r1cs.mA.end(); it++)
	{
		Fr::Mul(tmp, it->second, L[it->first.second]);
		Fr::Add(AA[it->first.first], AA[it->first.first], tmp);
	}
		//AA[it->first.first] += it->second * L[it->first.second];
	vector<Fr::Elem> BB(r1cs.m, "0");
	for (auto it = r1cs.mB.begin(); it != r1cs.mB.end(); it++)
	{
		Fr::Mul(tmp, it->second, L[it->first.second]);
		Fr::Add(BB[it->first.first], BB[it->first.first], tmp);
	}
		//BB[it->first.first] += it->second * L[it->first.second];
	vector<Fr::Elem> CC(r1cs.m, "0");
	for (auto it = r1cs.mC.begin(); it != r1cs.mC.end(); it++)
	{
		Fr::Mul(tmp, it->second, L[it->first.second]);
		Fr::Add(CC[it->first.first], CC[it->first.first], tmp);
	}
		//CC[it->first.first] += it->second * L[it->first.second];
	for (size_t i = 0; i < r1cs.m; i++)
	{
		//AA[i] *= rho_a;
		Fr::Mul(AA[i], AA[i], rho_a);
		//BB[i] *= rho_b;
		Fr::Mul(BB[i], BB[i], rho_b);
		//CC[i] *= rho_a * rho_b;
		Fr::Mul(CC[i], CC[i], rho_a);
		Fr::Mul(CC[i], CC[i], rho_b);

		//pk.A1.push_back(AA[i] * libff::alt_bn128_G1::one());
		EC::Mul(tmp_1, one_1, AA[i]);
		pk.A1.push_back(tmp_1);
		//pk.A2.push_back(AA[i] * alpha_a * libff::alt_bn128_G1::one());
		Fr::Mul(tmp, AA[i], alpha_a);
		EC::Mul(tmp_1, one_1, tmp);
		pk.A2.push_back(tmp_1);

		//pk.B1.push_back(BB[i] * libff::alt_bn128_G2::one());
		ECtw::Mul(tmp_2, one_2, BB[i]);
		pk.B1.push_back(tmp_2);
		//pk.B2.push_back(BB[i] * alpha_b * libff::alt_bn128_G1::one());
		Fr::Mul(tmp, BB[i], alpha_b);
		EC::Mul(tmp_1, one_1, tmp);
		pk.B2.push_back(tmp_1);

		//pk.C1.push_back(CC[i] * libff::alt_bn128_G1::one());
		EC::Mul(tmp_1, one_1, CC[i]);
		pk.C1.push_back(tmp_1);
		//pk.C2.push_back(CC[i] * alpha_c * libff::alt_bn128_G1::one());
		Fr::Mul(tmp, CC[i], alpha_c);
		EC::Mul(tmp_1, one_1, tmp);
		pk.C2.push_back(tmp_1);

		//pk.K.push_back(beta * (AA[i] + BB[i] + CC[i]) * libff::alt_bn128_G1::one());
		Fr::Add(tmp, AA[i], BB[i]);
		Fr::Add(tmp, tmp, CC[i]);
		Fr::Mul(tmp, tmp, beta);
		EC::Mul(tmp_1, one_1, tmp);
		pk.K.push_back(tmp_1);
	}

	//pk.A1.push_back(T_tau * rho_a * libff::alt_bn128_G1::one());
	//pk.A1.push_back(libff::alt_bn128_G1::zero());
	//pk.A1.push_back(libff::alt_bn128_G1::zero());
	Fr::Mul(tmp, T_tau, rho_a);
	EC::Mul(tmp_1, one_1, tmp);
	pk.A1.push_back(tmp_1);
	pk.A1.push_back(zero_1);
	pk.A1.push_back(zero_1);

	//pk.A2.push_back(T_tau * rho_a * alpha_a * libff::alt_bn128_G1::one());
	//pk.A2.push_back(libff::alt_bn128_G1::zero());
	//pk.A2.push_back(libff::alt_bn128_G1::zero());
	Fr::Mul(tmp, tmp, alpha_a);
	EC::Mul(tmp_1, one_1, tmp);
	pk.A2.push_back(tmp_1);
	pk.A2.push_back(zero_1);
	pk.A2.push_back(zero_1);

	//pk.B1.push_back(libff::alt_bn128_G2::zero());
	//pk.B1.push_back(T_tau * rho_b * libff::alt_bn128_G2::one());
	//pk.B1.push_back(libff::alt_bn128_G2::zero());
	Fr::Mul(tmp, T_tau, rho_b);
	ECtw::Mul(tmp_2, one_2, tmp);
	pk.B1.push_back(zero_2);
	pk.B1.push_back(tmp_2);
	pk.B1.push_back(zero_2);

	//pk.B2.push_back(libff::alt_bn128_G1::zero());
	//pk.B2.push_back(T_tau * rho_b * alpha_b * libff::alt_bn128_G1::one());
	//pk.B2.push_back(libff::alt_bn128_G1::zero());
	Fr::Mul(tmp, tmp, alpha_b);
	EC::Mul(tmp_1, one_1, tmp);
	pk.B2.push_back(zero_1);
	pk.B2.push_back(tmp_1);
	pk.B2.push_back(zero_1);

	//pk.C1.push_back(libff::alt_bn128_G1::zero());
	//pk.C1.push_back(libff::alt_bn128_G1::zero());
	//pk.C1.push_back(T_tau * rho_a * rho_b * libff::alt_bn128_G1::one());
	Fr::Mul(tmp, T_tau, rho_a);
	Fr::Mul(tmp, tmp, rho_b);
	EC::Mul(tmp_1, one_1, tmp);
	pk.C1.push_back(zero_1);
	pk.C1.push_back(zero_1);
	pk.C1.push_back(tmp_1);

	//pk.C2.push_back(libff::alt_bn128_G1::zero());
	//pk.C2.push_back(libff::alt_bn128_G1::zero());
	//pk.C2.push_back(T_tau * rho_a * rho_b * alpha_c * libff::alt_bn128_G1::one());
	Fr::Mul(tmp, tmp, alpha_c);
	EC::Mul(tmp_1, one_1, tmp);
	pk.C2.push_back(zero_1);
	pk.C2.push_back(zero_1);
	pk.C2.push_back(tmp_1);

	//pk.K.push_back(beta * T_tau * rho_a * libff::alt_bn128_G1::one());
	//pk.K.push_back(beta * T_tau * rho_b * libff::alt_bn128_G1::one());
	//pk.K.push_back(beta * T_tau * rho_a * rho_b * libff::alt_bn128_G1::one());
	Fr::Mul(tmp1, beta, T_tau);
	Fr::Mul(tmp, tmp1, rho_a);
	EC::Mul(tmp_1, one_1, tmp);
	pk.K.push_back(tmp_1);
	Fr::Mul(tmp, tmp1, rho_b);
	EC::Mul(tmp_1, one_1, tmp);
	pk.K.push_back(tmp_1);
	Fr::Mul(tmp, tmp, rho_a);
	EC::Mul(tmp_1, one_1, tmp);
	pk.K.push_back(tmp_1);

	//libff::alt_bn128_G1 Hi = libff::alt_bn128_G1::one();
	EC::Elem Hi;
	EC::SetOne(Hi);
	pk.H.push_back(Hi);
	for (size_t i = 0; i < r1cs.n; i++)
	{
		//Hi = tau * Hi;
		EC::Mul(Hi, Hi, tau);
		pk.H.push_back(Hi);
	}

	//for (auto it = pk.A1.begin(); it != pk.A1.end(); it++)
	//	it->to_special();
	//for (auto it = pk.A2.begin(); it != pk.A2.end(); it++)
	//	it->to_special();
	//for (auto it = pk.B1.begin(); it != pk.B1.end(); it++)
	//	it->to_special();
	//for (auto it = pk.B2.begin(); it != pk.B2.end(); it++)
	//	it->to_special();
	//for (auto it = pk.C1.begin(); it != pk.C1.end(); it++)
	//	it->to_special();
	//for (auto it = pk.C2.begin(); it != pk.C2.end(); it++)
	//	it->to_special();
	//for (auto it = pk.K.begin(); it != pk.K.end(); it++)
	//	it->to_special();
	//for (auto it = pk.H.begin(); it != pk.H.end(); it++)
	//	it->to_special();

	pk.V = r1cs.V;

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "Creating verificarion key...\n";
	cout.flush();
	start2 = clock();

	// Verifying key evaluation  [ vk = (vkA, vkB, vkC, vkGamma, vkBetaGamma1, vkBetaGamma2, vkT, vkIC) ]
	VerifyingKey vk;
	//vk.A = alpha_a * libff::alt_bn128_G2::one();
	//vk.A.to_special();
	ECtw::Mul(vk.A, one_2, alpha_a);
	//vk.B = alpha_b * libff::alt_bn128_G1::one();
	//vk.B.to_special();
	EC::Mul(vk.B, one_1, alpha_b);
	//vk.C = alpha_c * libff::alt_bn128_G2::one();
	//vk.C.to_special();
	ECtw::Mul(vk.C, one_2, alpha_c);
	//vk.Gamma = gamma * libff::alt_bn128_G2::one();
	//vk.Gamma.to_special();
	ECtw::Mul(vk.Gamma, one_2, gamma);
	//vk.BetaGamma1 = beta * gamma * libff::alt_bn128_G1::one();
	//vk.BetaGamma1.to_special();
	Fr::Mul(tmp, beta, gamma);
	EC::Mul(vk.BetaGamma1, one_1, tmp);
	//vk.BetaGamma2 = beta * gamma * libff::alt_bn128_G2::one();
	//vk.BetaGamma2.to_special();
	ECtw::Mul(vk.BetaGamma2, one_2, tmp);
	//vk.T = T_tau * rho_a * rho_b * libff::alt_bn128_G2::one();
	//vk.T.to_special();
	Fr::Mul(tmp, T_tau, rho_a);
	Fr::Mul(tmp, tmp, rho_b);
	ECtw::Mul(vk.T, one_2, tmp);
	//vk.IC = vector<libff::alt_bn128_G1>(pk.A1.begin(), pk.A1.begin() + r1cs.input_size);
	vk.IC = vector<EC::Elem>(pk.A1.begin(), pk.A1.begin() + r1cs.input_size);


	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "Exporting proving key...\n";
	cout.flush();
	start2 = clock();

	pk.Export("pk.key");

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "Exporting verification key...\n";
	cout.flush();
	start2 = clock();

	vk.Export("vk.key");

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();*/

	cout << "Importing proving key...\n";
	cout.flush();
	start2 = clock();

	pk.Import("pk.key");

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "Importing verification key...\n";
	cout.flush();
	start2 = clock();

	VerifyingKey vk;
	vk.Import("vk.key");

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "Key generation finished\n";
	cout << "Elapsed time: " << fixed << setprecision(3) << double(clock() - start1) / CLOCKS_PER_SEC << " sec\n\n";
	cout.flush();


	/*
	 *  _________
	 *   Proving
	 *  ¯¯¯¯¯¯¯¯¯
	 */

	cout << "SNARK generation started\n";
	cout.flush();
	start1 = clock();

	cout << "Creating witness...\n";
	cout.flush();
	start2 = clock();

	// Get witness
	vector<Fr::Elem> s = GetWitness(r1cs.V);

	// Add error
	//Fr::Add(s[1000], s[1000], "1");

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	/*cout << "Checking witness...\n";
	cout.flush();
	start2 = clock();

	CheckWitness(r1cs, s);

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();*/

	cout << "Calculating H(x)...\n";
	cout.flush();
	start2 = clock();

	// Random parameters generation
	Fr::Elem delta_1, delta_2, delta_3;
	Fr::GetRandomElement(delta_1);
	Fr::GetRandomElement(delta_2);
	Fr::GetRandomElement(delta_3);

	// Calculate polynomials A, B and C
	Fr::Elem coset_base("3"), coset_base_inv;
	Fr::Inv(coset_base_inv, coset_base);

	memset(buf1, 0, sizeof(Fr::Elem) * kBigDomainSize);
	for (auto it = r1cs.mA.begin(); it != r1cs.mA.end(); it++)
	{
		Fr::Mul(tmp, s[it->first.first], it->second);
		Fr::Add(buf1[it->first.second], buf1[it->first.second], tmp);
	}
	memset(buf2, 0, sizeof(Fr::Elem) * kBigDomainSize);
	SmallDomain::iFFT(buf2, buf1);
	SmallDomain::AddVanishingPolynomial(buf2, delta_1);
	BigDomain::MoveToCoset(buf2, buf2, coset_base);
	memset(buf1, 0, sizeof(Fr::Elem) * kBigDomainSize);
	BigDomain::FFT(buf1, buf2);

	memset(buf2, 0, sizeof(Fr::Elem) * kBigDomainSize);
	for (auto it = r1cs.mB.begin(); it != r1cs.mB.end(); it++)
	{
		Fr::Mul(tmp, s[it->first.first], it->second);
		Fr::Add(buf2[it->first.second], buf2[it->first.second], tmp);
	}
	memset(buf3, 0, sizeof(Fr::Elem) * kBigDomainSize);
	SmallDomain::iFFT(buf3, buf2);
	SmallDomain::AddVanishingPolynomial(buf3, delta_2);
	BigDomain::MoveToCoset(buf3, buf3, coset_base);
	memset(buf2, 0, sizeof(Fr::Elem) * kBigDomainSize);
	BigDomain::FFT(buf2, buf3);

	for (size_t i = 0; i <= BigDomain::polynomial_size; i++)
	{
		Fr::Mul(buf3[i], buf1[i], buf2[i]);
	}

	memset(buf1, 0, sizeof(Fr::Elem) * kBigDomainSize);
	for (auto it = r1cs.mC.begin(); it != r1cs.mC.end(); it++)
	{
		Fr::Mul(tmp, s[it->first.first], it->second);
		Fr::Add(buf1[it->first.second], buf1[it->first.second], tmp);
	}
	memset(buf2, 0, sizeof(Fr::Elem) * kBigDomainSize);
	SmallDomain::iFFT(buf2, buf1);
	SmallDomain::AddVanishingPolynomial(buf2, delta_3);
	BigDomain::MoveToCoset(buf2, buf2, coset_base);
	memset(buf1, 0, sizeof(Fr::Elem) * kBigDomainSize);
	BigDomain::FFT(buf1, buf2);

	for (size_t i = 0; i <= BigDomain::polynomial_size; i++)
	{
		Fr::Sub(buf3[i], buf3[i], buf1[i]);
	}

	memset(buf1, 0, sizeof(Fr::Elem) * kBigDomainSize);
	SmallDomain::AddVanishingPolynomial(buf1, "1");
	BigDomain::MoveToCoset(buf1, buf1, coset_base);
	memset(buf2, 0, sizeof(Fr::Elem) * kBigDomainSize);
	BigDomain::FFT(buf2, buf1);

	for (size_t i = 0; i <= BigDomain::polynomial_size; i++)
	{
		Fr::Div(buf3[i], buf3[i], buf2[i]);
	}

	memset(buf1, 0, sizeof(Fr::Elem) * kBigDomainSize);
	BigDomain::iFFT(buf1, buf3);
	BigDomain::MoveToCoset(buf1, buf1, coset_base_inv);




	/*memset(A_val, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(B_val, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(C_val, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(T_val, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(H_val, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(A_coef, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(B_coef, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(C_coef, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(T_coef, 0, sizeof(Fr::Elem) * kBigDomainSize);
	memset(H_coef, 0, sizeof(Fr::Elem) * kBigDomainSize);

	for (auto it = r1cs.mA.begin(); it != r1cs.mA.end(); it++)
	{
		Fr::Mul(tmp, s[it->first.first], it->second);
		Fr::Add(A_val[it->first.second], A_val[it->first.second], tmp);
	}

	for (auto it = r1cs.mB.begin(); it != r1cs.mB.end(); it++)
	{
		Fr::Mul(tmp, s[it->first.first], it->second);
		Fr::Add(B_val[it->first.second], B_val[it->first.second], tmp);
	}

	for (auto it = r1cs.mC.begin(); it != r1cs.mC.end(); it++)
	{
		Fr::Mul(tmp, s[it->first.first], it->second);
		Fr::Add(C_val[it->first.second], C_val[it->first.second], tmp);
	}

	SmallDomain::iFFT(A_coef, A_val);
	SmallDomain::iFFT(B_coef, B_val);
	SmallDomain::iFFT(C_coef, C_val);

	SmallDomain::AddVanishingPolynomial(A_coef, delta_1);
	SmallDomain::AddVanishingPolynomial(B_coef, delta_2);
	SmallDomain::AddVanishingPolynomial(C_coef, delta_3);
	SmallDomain::AddVanishingPolynomial(T_coef, "1");

	// Jump into coset in another domain
	// TODO ! IS 3 OK? OR SHOULD ANOTHER ELEMENT BE USED?

	Fr::Elem coset_base("3"), coset_base_inv;
	Fr::Inv(coset_base_inv, coset_base);
	BigDomain::MoveToCoset(A_coef, A_coef, coset_base);
	BigDomain::MoveToCoset(B_coef, B_coef, coset_base);
	BigDomain::MoveToCoset(C_coef, C_coef, coset_base);
	BigDomain::MoveToCoset(T_coef, T_coef, coset_base);
	BigDomain::FFT(A_val, A_coef);
	BigDomain::FFT(B_val, B_coef);
	BigDomain::FFT(C_val, C_coef);
	BigDomain::FFT(T_val, T_coef);

	// Evaluate H
	for (size_t i = 0; i <= BigDomain::polynomial_size; i++)
	{
		Fr::Mul(H_val[i], A_val[i], B_val[i]);
		Fr::Sub(H_val[i], H_val[i], C_val[i]);
		Fr::Div(H_val[i], H_val[i], T_val[i]);
	}
	BigDomain::iFFT(H_coef, H_val);
	BigDomain::MoveToCoset(H_coef, H_coef, coset_base_inv);*/

	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "Calculating SNARK...\n";
	cout.flush();
	start2 = clock();

	// Create SNARK
	// TODO ! PARALLELIZATION?
	s.push_back(delta_1);
	s.push_back(delta_2);
	s.push_back(delta_3);
	EC::Elem piA1;
	EC::Elem piA2;
	ECtw::Elem piB1;
	EC::Elem piB2;
	EC::Elem piC1;
	EC::Elem piC2;
	EC::Elem piK;
	EC::Elem piH;

	for (unsigned int i = 0; i < s.size(); i++)
	{
		power_s[i] = s[i];
	}/*
	for (unsigned int i = s.size(); i < kBigDomainSize; i++)
	{
		power_s[i] = "0";
	}*/
	for (unsigned int i = 0; i < pk.H.size(); i++)
	{
		power_h[i] = buf1[i];
	}/*
	for (unsigned int i = pk.H.size(); i < kBigDomainSize; i++)
	{
		power_h[i] = "0";
	}*/
	/*EC::MultiMul(piA1, &pk.A1[r1cs.input_size], &power_s[r1cs.input_size], pk.A1.size() - r1cs.input_size);
	EC::MultiMul(piA2, &pk.A2[r1cs.input_size], &power_s[r1cs.input_size], pk.A2.size() - r1cs.input_size);
	ECtw::MultiMul(piB1, &pk.B1[0], power_s, pk.B1.size());
	EC::MultiMul(piB2, &pk.B2[0], power_s, pk.B2.size());
	EC::MultiMul(piC1, &pk.C1[0], power_s, pk.C1.size());
	EC::MultiMul(piC2, &pk.C2[0], power_s, pk.C2.size());
	EC::MultiMul(piK, &pk.K[0], power_s, pk.K.size());
	EC::MultiMul(piH, &pk.H[0], power_h, pk.H.size());*/
	/*a1_work(&piA1);
	a2_work(&piA2);
	b1_work(&piB1);
	b2_work(&piB2);
	c1_work(&piC1);
	c2_work(&piC2);
	k_work(&piK);
	h_work(&piH);*/
	std::thread a1(a1_work, &piA1);
	std::thread a2(a2_work, &piA2);
	std::thread b1(b1_work, &piB1);
	std::thread b2(b2_work, &piB2);
	std::thread c1(c1_work, &piC1);
	std::thread c2(c2_work, &piC2);
	std::thread k(k_work, &piK);
	std::thread h(h_work, &piH);
	a1.join();
	a2.join();
	b1.join();
	b2.join();
	c1.join();
	c2.join();
	k.join();
	h.join();


	cout << "Done! " << fixed << setprecision(3) << double(clock() - start2) / CLOCKS_PER_SEC << " sec\n";
	cout.flush();

	cout << "SNARK generation finished\n";
	cout << "Elapsed time: " << fixed << setprecision(3) << double(clock() - start1) / CLOCKS_PER_SEC << " sec\n\n";
	cout.flush();

	/*
	 *  ___________
	 *   Verifying
	 *  ¯¯¯¯¯¯¯¯¯¯¯
	 */

	cout << "SNARK varification started\n";
	cout.flush();
	start1 = clock();

	// Get input
	//vector<Fr::Elem> input = { "1", mt->Root(), topic_hash, pass_topic_hash, comment_hash, pass_comment_hash };
	//vector<Fr::Elem> input = { "1", "2", "128" };
	//vector<Fr::Elem> input = { "1" };
	vector<Fr::Elem> input = { "1", mt->Root(), topic_hash, pass_topic_hash, comment_hash, 
		n_limbs[0], n_limbs[1], n_limbs[2], n_limbs[3], n_limbs[4], n_limbs[5], n_limbs[6], n_limbs[7], 
		n_limbs[8], n_limbs[9], n_limbs[10], n_limbs[11], n_limbs[12], n_limbs[13], n_limbs[14], n_limbs[15] };

	// Calculate verification key
	EC::Elem vkX;
	EC::SetZero(vkX);
	for (size_t i = 0; i < r1cs.input_size; i++)
	{
		EC::Mul(tmp_1, vk.IC[i], input[i]);
		EC::Add(vkX, vkX, tmp_1);
	}

	TPE tpe;

	ECex::Elem tmp_3;
	Fp12::Elem pair1, pair2, pair3;

	// Check validity of knowledge commitments
	ECT::Untwist(tmp_3, vk.A);
	tpe.TatePairing(pair1, piA1, tmp_3);
	ECT::Untwist(tmp_3, one_2);
	tpe.TatePairing(pair2, piA2, tmp_3);
	bool cond1 = Fp12::CheckEqual(pair1, pair2);

	ECT::Untwist(tmp_3, piB1);
	tpe.TatePairing(pair1, vk.B, tmp_3);

	ECT::Untwist(tmp_3, one_2);
	tpe.TatePairing(pair2, piB2, tmp_3);
	bool cond2 = Fp12::CheckEqual(pair1, pair2);

	ECT::Untwist(tmp_3, vk.C);
	tpe.TatePairing(pair1, piC1, tmp_3);
	ECT::Untwist(tmp_3, one_2);
	tpe.TatePairing(pair2, piC2, tmp_3);
	bool cond3 = Fp12::CheckEqual(pair1, pair2);

	// Check same coefficients were used
	ECT::Untwist(tmp_3, vk.Gamma);
	tpe.TatePairing(pair1, piK, tmp_3);
	EC::Add(tmp_1, vkX, piA1);
	EC::Add(tmp_1, tmp_1, piC1);
	ECT::Untwist(tmp_3, vk.BetaGamma2);
	tpe.TatePairing(pair2, tmp_1, tmp_3);
	ECT::Untwist(tmp_3, piB1);
	tpe.TatePairing(pair3, vk.BetaGamma1, tmp_3);
	Fp12::Mul(pair2, pair2, pair3);
	bool cond4 = Fp12::CheckEqual(pair1, pair2);

	// Check QAP divisibility
	EC::Add(tmp_1, vkX, piA1);
	ECT::Untwist(tmp_3, piB1);
	tpe.TatePairing(pair1, tmp_1, tmp_3);
	ECT::Untwist(tmp_3, vk.T);
	tpe.TatePairing(pair2, piH, tmp_3);
	ECT::Untwist(tmp_3, one_2);
	tpe.TatePairing(pair3, piC1, tmp_3);
	Fp12::Mul(pair2, pair2, pair3);
	bool cond5 = Fp12::CheckEqual(pair1, pair2);

	cout << "SNARK verification finished\n";
	cout << "Elapsed time: " << fixed << setprecision(3) << double(clock() - start1) / CLOCKS_PER_SEC << " sec\nResult:\n";

	// Output check result
	cout << "Check of knowledge commitments for A: " << ((cond1) ? ("PASSED") : ("FAILED")) << '\n';
	cout << "Check of knowledge commitments for B: " << ((cond2) ? ("PASSED") : ("FAILED")) << '\n';
	cout << "Check of knowledge commitments for C: " << ((cond3) ? ("PASSED") : ("FAILED")) << '\n';
	cout << "Check of the same coefficients: " << ((cond4) ? ("PASSED") : ("FAILED")) << '\n';
	cout << "Check QAP divisibility (main check): " << ((cond5) ? ("PASSED") : ("FAILED")) << '\n';

	cout.flush();
	return 0;
}