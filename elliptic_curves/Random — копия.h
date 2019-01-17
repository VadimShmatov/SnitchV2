#pragma once
#include <memory>
#include <mutex>
#include <random>
#include <chrono>
#include "isaac.h"

using namespace std;
using namespace utils;

class Random
{
private:
	static const size_t c_nAlpha = 8;
	static const size_t c_nStateSize = 1 << c_nAlpha;

	static mutex m_mInitMutex;
	static shared_ptr<Random> m_pInstance;

	mutex m_mGenMutex;
	isaac<c_nAlpha> m_isaac;

	Random();

public:
	static shared_ptr<Random> GetInstance();
	void GenBytes(unsigned char* pBuffer, size_t nCount);

};