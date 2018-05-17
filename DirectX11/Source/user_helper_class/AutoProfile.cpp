#include	"AutoProfile.h"

#if defined(DEBUG) || defined(_DEBUG)
//-----------------------------------------------------------------------------------------
//
//	引数付きコンストラクタ
//
//-----------------------------------------------------------------------------------------
AutoProfile::AutoProfile(const char* pFunctionName) :
m_pFunctionName(pFunctionName),
m_start(std::chrono::system_clock::now())
{}

//-----------------------------------------------------------------------------------------
//
//	デストラクタ
//
//-----------------------------------------------------------------------------------------
AutoProfile::~AutoProfile()
{
	m_end = std::chrono::system_clock::now();
	long long l_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(m_end - m_start).count();
	printf_s("%s = %lld[microseconds]\n", m_pFunctionName, l_elapsed);
}
#endif