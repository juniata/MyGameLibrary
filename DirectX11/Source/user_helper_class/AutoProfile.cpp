#include	"AutoProfile.h"

#if defined(DEBUG) || defined(_DEBUG)
//-----------------------------------------------------------------------------------------
//
//	�����t���R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
AutoProfile::AutoProfile(const char* pFunctionName) :
m_pFunctionName(pFunctionName),
m_start(std::chrono::system_clock::now())
{}

//-----------------------------------------------------------------------------------------
//
//	�f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
AutoProfile::~AutoProfile()
{
	m_end = std::chrono::system_clock::now();
	long long l_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(m_end - m_start).count();
	printf_s("%s = %lld[microseconds]\n", m_pFunctionName, l_elapsed);
}
#endif