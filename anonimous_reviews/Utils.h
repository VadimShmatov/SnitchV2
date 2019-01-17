#pragma once
#include <Windows.h>
#include <string>

#include "picosha2.h"

using namespace std;

void FatalError(wstring msg);
int DeleteDirectory(const std::string &refcstrRootDirectory, bool bDeleteSubdirectories = true);

string BinToHex(string str);
string HexToBin(string str);

string Hash(string str);

string UrlEncode(string str);
string UrlDecode(string str);