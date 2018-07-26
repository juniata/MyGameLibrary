#ifndef __DX_AUDIO_H_
#define __DX_AUDIO_H_

//****************************************************************************************************
//
//	Audio
//
//****************************************************************************************************
class Audio
{
private:
	static IXAudio2*	m_pIXAudio;
	static IXAudio2MasteringVoice*	m_pIXAudioMasteringVoice;

	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	Audio() = delete;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	~Audio(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�[�ϐ���������
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�[�ϐ������
	//
	//------------------------------------------------------------------------------
	static void Release();
};
#endif // !__DX_AUDIO_H_