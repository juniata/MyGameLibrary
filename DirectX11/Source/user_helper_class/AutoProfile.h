#ifndef __AUTO_PROFILE_H_
#define __AUTO_PROFILE_H_

#if defined(DEBUG) || defined(_DEBUG)

#ifndef _CHRONO_
	#include	<chrono>
#endif // !_CHRONO_


//****************************************************************************************************
//
//	AutoProfile(�f�o�b�O���̂ݒ�`�A�錾�����)
//
//****************************************************************************************************
class AutoProfile
{
private:
	const char* m_pFunctionName;
	std::chrono::system_clock::time_point m_start; 
	std::chrono::system_clock::time_point m_end;

	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	AutoProfile(){};
public:
	//------------------------------------------------------------------------------
	//	
	//  @brief		�֐��̏������x���v���J�n����
	//	@param[in]	�֐���
	//
	//------------------------------------------------------------------------------
	AutoProfile(const char* pFunctionName);

	//------------------------------------------------------------------------------
	//
	//  @brief		�֐��̏������x���v���I������
	//
	//------------------------------------------------------------------------------
	~AutoProfile();
};

#define PROFILE(name)	AutoProfile l_autoProfile(name);

#else 
#define PROFILE(name)

#endif // !defined(DEBUG) || defined(_DEBUG)

#endif // !__AUTO_PROFILE_H_
