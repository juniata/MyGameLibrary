#ifndef __AUTO_PROFILE_H_
#define __AUTO_PROFILE_H_

#if defined(DEBUG) || defined(_DEBUG)

#ifndef _CHRONO_
	#include	<chrono>
#endif // !_CHRONO_


//****************************************************************************************************
//
//	AutoProfile(デバッグ時のみ定義、宣言される)
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
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	AutoProfile(){};
public:
	//------------------------------------------------------------------------------
	//	
	//  @brief		関数の処理速度を計測開始する
	//	@param[in]	関数名
	//
	//------------------------------------------------------------------------------
	AutoProfile(const char* pFunctionName);

	//------------------------------------------------------------------------------
	//
	//  @brief		関数の処理速度を計測終了する
	//
	//------------------------------------------------------------------------------
	~AutoProfile();
};

#define PROFILE(name)	AutoProfile l_autoProfile(name);

#else 
#define PROFILE(name)

#endif // !defined(DEBUG) || defined(_DEBUG)

#endif // !__AUTO_PROFILE_H_
