#pragma once
#include <string>
#include <fstream>

#include "Utils.h"

using namespace std;

class IpfsAdapter
{
public:
	static bool Save(string hash, const string& data);
	static bool Load(string hash, string& data);
};

