#pragma once
#include <memory>
#include <vector>
#include <map>
#include "Structures.h"

using namespace std;
using namespace ec_lib;

class MiMC5
{
public:
    static const size_t m_uiNumberOfRounds;
	static Fr::Elem m_initialValue;
	Fr::Elem *m_pRoundConstants;

    static shared_ptr<MiMC5> GetInstance();
    Fr::Elem HashStep(Fr::Elem v1, Fr::Elem v2);
    Fr::Elem Hash(vector<Fr::Elem> vBlocks);

    Fr::Elem HashStep_witness(Fr::Elem v1, Fr::Elem v2, map<string, Fr::Elem>& wit_map, size_t& step);
    Fr::Elem Hash_witness(vector<Fr::Elem> vBlocks, map<string, Fr::Elem>& wit_map, size_t& step);

    ~MiMC5();
    
private:
    static shared_ptr<MiMC5> m_pInstance;

    MiMC5();
};