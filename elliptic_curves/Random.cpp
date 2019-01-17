#include "Random.h"

mutex Random::m_mInitMutex;
Random* Random::m_pInstance = nullptr;

Random* Random::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		//lock_guard<mutex> lock(m_mInitMutex);
		if (m_pInstance == nullptr)
			m_pInstance = new Random();
	}
	return m_pInstance;
}

Random::Random()
{
	isaac32_engine::result_type pSeed[c_nStateSize];

	const size_t nBufLen = (sizeof(pSeed) + sizeof(random_device::result_type) - 1) / sizeof(random_device::result_type);
	unsigned char pMainEntropy[nBufLen * sizeof(random_device::result_type)];
	random_device rd;
	for (size_t i = 0; i < nBufLen; i++)
		((random_device::result_type*)pMainEntropy)[i] = rd();

	unsigned char pAdditionalEntropy[sizeof(long long) + 2 * sizeof(char*)];
	*(long long*)pAdditionalEntropy = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
	char cLocalVariable;
	*(char**)(pAdditionalEntropy + sizeof(long long)) = &cLocalVariable;
	char* pHeapVariable = (char*)malloc(1);
	*(char**)(pAdditionalEntropy + sizeof(long long) + sizeof(char*)) = pHeapVariable;
	free(pHeapVariable);

	for (size_t i = 0; i < sizeof(pSeed); i++)
		((char*)pSeed)[i] = pMainEntropy[i % sizeof(pMainEntropy)] ^ pAdditionalEntropy[i % sizeof(pAdditionalEntropy)];
	m_isaac = new isaac32_engine(pSeed);
}

void Random::GenBytes(unsigned char * pBuffer, size_t nCount)
{
	uniform_int_distribution<unsigned short> dist(0, 255);
	for (size_t i = 0; i < nCount; i++)
		pBuffer[i] = dist(*m_isaac);
}
