#pragma once
#include <string.h>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "mimc5.h"

using namespace std;
using namespace ec_lib;

class MerkleTree
{
public:
    const static uint32_t m_cHeight = 31;
    
    MerkleTree(string sPathToTree, string sPathToIndex);
    ~MerkleTree();
    void Add(Fr::Elem val);
    bool Check(Fr::Elem val);
    Fr::Elem Root();
    pair<vector<Fr::Elem>, uint32_t> Path(Fr::Elem val);

private:

    struct Coord
    {
        uint32_t x;
        uint32_t h;
    };

    const uint32_t m_cElementSize = 32;
    uint32_t m_uiSize;
    Fr::Elem *m_pDefaultValues;
    fstream m_fIndex, m_fTree;
    shared_ptr<MiMC5> mimc5;

    void SetValue(Coord coord, Fr::Elem val);
    Fr::Elem GetValue(Coord coord);
    Fr::Elem GetValue(uint32_t index);
    uint32_t GetPosition(Coord coord);
    Coord GetParent(Coord coord);
    Coord GetNeighbour(Coord coord);

    uint32_t GetIndex(uint32_t ord);
    void AddIndex(uint32_t ord, uint32_t index);
    bool SearchValue(Fr::Elem val, uint32_t& ord);
    
    int cmp(const Fr::Elem& left, const Fr::Elem& right);

};