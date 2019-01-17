#include "IpfsAdapter.h"

bool IpfsAdapter::Save(string hash, const string& data)
{
	ofstream fout("E:\\Ipfs\\" + BinToHex(hash), ios::binary);
	fout.write(data.data(), data.length());
	fout.close();
	return true;
}

bool IpfsAdapter::Load(string hash, string & data)
{
	ifstream fin("E:\\Ipfs\\" + BinToHex(hash), ios::binary);
	if (!fin.is_open())
	{
		return false;
	}
	data = string((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	fin.close();
	return true;
}
