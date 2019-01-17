#pragma once
#include <memory>
#include <mutex>
#include <random>
#include <chrono>
#include "isaac32_engine.h"

using namespace std;

class Random
{
private:
	static const size_t c_nAlpha = 8;
	static const size_t c_nStateSize = 1 << c_nAlpha;

	static mutex m_mInitMutex;
	static Random* m_pInstance;

	mutex m_mGenMutex;
	isaac32_engine* m_isaac;

	Random();

public:
	static Random* GetInstance();
	void GenBytes(unsigned char* pBuffer, size_t nCount);

};