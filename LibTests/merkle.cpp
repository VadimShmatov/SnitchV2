#include "merkle.h"

MerkleTree::MerkleTree(string sPathToTree, string sPathToIndex)
{
    m_fTree.open(sPathToTree, ios::in | ios::out | ios::ate | ios::binary);
    if (!m_fTree.good())
        m_fTree.open(sPathToTree, ios::in | ios::out | ios::trunc | ios::binary);
    m_fIndex.open(sPathToIndex, ios::in | ios::out | ios::ate | ios::binary);
    if (!m_fIndex.good())
        m_fIndex.open(sPathToIndex, ios::in | ios::out | ios::trunc | ios::binary);
    uint32_t uiIndexSize = m_fIndex.tellg();
    if (uiIndexSize == 0)
    {
        m_uiSize = 0;
        m_fIndex.write((char*)&m_uiSize, sizeof(uint32_t));
    }
    else
    {
        m_fIndex.clear();
        m_fIndex.seekp(0);
        m_fIndex.read((char*)&m_uiSize, sizeof(uint32_t));
    }
    mimc5 = MiMC5::GetInstance();
    m_pDefaultValues = new Fr::Elem[m_cHeight];
    m_pDefaultValues[0] = "0";
    for (uint32_t i = 1; i < m_cHeight; i++)
        m_pDefaultValues[i] = mimc5->Hash({ m_pDefaultValues[i - 1], m_pDefaultValues[i - 1] });
}

MerkleTree::~MerkleTree()
{
    m_fTree.close();
    m_fIndex.close();
    delete[] m_pDefaultValues;
}

void MerkleTree::Add(Fr::Elem val)
{
    uint32_t ord;
    if (SearchValue(val, ord))
        return;
    Coord node = { m_uiSize, 0 };
    SetValue(node, val);
    for (uint32_t h = 1; h <= m_cHeight; h++)
    {
        if (node.x & 1)
            val = mimc5->Hash({ GetValue(GetNeighbour(node)), val });
        else
            val = mimc5->Hash({ val, GetValue(GetNeighbour(node)) });
        node = GetParent(node);
        SetValue(node, val);
    }
    AddIndex(ord, m_uiSize);
    m_fIndex.flush();
    m_fTree.flush();
}

bool MerkleTree::Check(Fr::Elem val)
{
    uint32_t ord;
    return SearchValue(val, ord);
}

Fr::Elem MerkleTree::Root()
{
    return GetValue({ 0, m_cHeight });
}

pair<vector<Fr::Elem>, uint32_t> MerkleTree::Path(Fr::Elem val)
{
    pair<vector<Fr::Elem>, uint32_t> result;
    uint32_t ord;
    if (!SearchValue(val, ord))
        return result;

    Coord node = { GetIndex(ord), 0 };
    result.second = 0;
    for (uint32_t h = 0; h < m_cHeight; h++)
    {
        result.first.push_back(GetValue(GetNeighbour(node)));
        if (node.x & 1)
            result.second |= (1 << h);
        node = GetParent(node);
    }
    return result;
}

void MerkleTree::SetValue(Coord coord, Fr::Elem val)
{
    uint32_t pos = GetPosition(coord);
    m_fTree.clear();
    m_fTree.seekp(pos * m_cElementSize);
    //m_fTree.write((char*) val.limbs, m_cElementSize);
	m_fTree << val;
}

Fr::Elem MerkleTree::GetValue(Coord coord)
{
    uint32_t pos = GetPosition(coord);
    uint32_t max = GetPosition({ m_uiSize - 1, 0 });
    if (pos <= max)
    {
        m_fTree.clear();
        m_fTree.seekg(pos * m_cElementSize);
		Fr::Elem tmp;
        //m_fTree.read((char*)tmp.limbs, m_cElementSize);
		m_fTree >> tmp;
        return tmp;
    }
    else
    {
        return m_pDefaultValues[coord.h];
    }
}

Fr::Elem MerkleTree::GetValue(uint32_t index)
{
    return MerkleTree::GetValue({ index, 0 });
}

uint32_t MerkleTree::GetPosition(Coord coord)
{
    uint32_t result = 0;
    for (uint32_t i = coord.h; i < m_cHeight; i++)
    {
        if (coord.x & 0x1)
            result += (1 << (i + 1));
        else
            result += 1;
        coord.x >>= 1;
    }
    return result;
}

MerkleTree::Coord MerkleTree::GetParent(Coord coord)
{
    coord.h += 1;
    coord.x >>= 1;
    return coord;
}

MerkleTree::Coord MerkleTree::GetNeighbour(Coord coord)
{
    coord.x ^= 1;
    return coord;
}

uint32_t MerkleTree::GetIndex(uint32_t ord)
{
    uint32_t tmp;
    m_fIndex.clear();
    m_fIndex.seekg((1 + ord) * sizeof(uint32_t));
    m_fIndex.read((char*)&tmp, sizeof(uint32_t));
    return tmp;
}

const uint32_t buf_size = 10 * 1024 * 1024; // 10 Mb
char buf[buf_size];

void MerkleTree::AddIndex(uint32_t ord, uint32_t index)
{
    uint32_t start_pos = (1 + ord) * sizeof(uint32_t);
    uint32_t total_size = (1 + m_uiSize) * sizeof(uint32_t);
    uint32_t pos = start_pos + ((total_size - start_pos) / buf_size) * buf_size;
    if (pos != total_size)
        while (true)
        {
            m_fIndex.clear();
            m_fIndex.seekg(pos);
            m_fIndex.read(buf, buf_size);
            m_fIndex.clear();
            m_fIndex.seekp(pos + sizeof(uint32_t));
            m_fIndex.write(buf, min(buf_size, total_size - pos));
            if (pos < start_pos + buf_size)
                break;
            pos -= buf_size;
        }
    m_fIndex.clear();
    m_fIndex.seekp(start_pos);
    m_fIndex.write((char*)&index, sizeof(uint32_t));
    m_uiSize++;
    m_fIndex.clear();
    m_fIndex.seekp(0);
    m_fIndex.write((char*)&m_uiSize, sizeof(uint32_t));
}

bool MerkleTree::SearchValue(Fr::Elem val, uint32_t& ord)
{
    uint32_t first = 0;
    uint32_t last = m_uiSize;

    if (last == 0)
    {
        ord = 0;
        return false;
    }
    if (cmp(GetValue(GetIndex(0)), val) > 0)
    {
        ord = 0;
        return false;
    }
    if (cmp(GetValue(GetIndex(last - 1)), val) < 0)
    {
        ord = last;
        return false;
    }

    while (first < last) {
        uint32_t mid = first + (last - first) / 2;

        if (cmp(val, GetValue(GetIndex(mid))) <= 0)
            last = mid;
        else
            first = mid + 1;
    }

    ord = last;
    return (cmp(val, GetValue(GetIndex(ord))) == 0);
}


// TODO! Memory leak? Is it needed to destroy numbers?
int MerkleTree::cmp(const Fr::Elem& left, const Fr::Elem& right)
{
	return mpn_cmp(left.limbs, right.limbs, m_cElementSize / sizeof(mp_limb_t));
    /*mpz_t l,r;
    mpz_init(l);
    mpz_init(r);
    left.as_bigint().to_mpz(l);
    right.as_bigint().to_mpz(r);
    return mpz_cmp(l, r);*/
}