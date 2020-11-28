#ifndef __DX_AUDIO_H_
#define __DX_AUDIO_H_

//****************************************************************************************************
//
//	Audio
//
//****************************************************************************************************
class DX_Audio
{
private:
	static IXAudio2*	m_pIXAudio;
	static IXAudio2MasteringVoice*	m_pIXAudioMasteringVoice;

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	DX_Audio() = delete;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	~DX_Audio(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数を初期化
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数を解放
	//
	//------------------------------------------------------------------------------
	static void Release();
};
#endif // !__DX_AUDIO_H_
