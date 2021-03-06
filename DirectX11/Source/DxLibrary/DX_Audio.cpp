//#include	"DX_Library.h"
//
////-----------------------------------------------------------------------------------------
////
////  staticメンバ変数
////
////-----------------------------------------------------------------------------------------
//IXAudio2*				DX_Audio::m_pIXAudio				= nullptr;
//IXAudio2MasteringVoice*	DX_Audio::m_pIXAudioMasteringVoice	= nullptr;
//
////-----------------------------------------------------------------------------------------
////
////  メンバ変数を初期化
////
////-----------------------------------------------------------------------------------------
//void DX_Audio::Initialize()
//{
//	HRESULT l_hr;
//
//	//	XAudio2を初期化
//	l_hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//
//	if (!DX_Debug::GetInstance()->CheckHresult(l_hr)){
//		throw "CoInitializeEx() : failed";
//	}
//
//#if defined(DEBUG) || defined(_DEBUG)
//	UINT32 l_flags = XAUDIO2_DEBUG_ENGINE;
//#else
//	UINT32 l_flags = 0;
//#endif
//
//	//	XAudio2を生成
//	l_hr = XAudio2Create(&m_pIXAudio, l_flags);
//
//	if (!DX_Debug::GetInstance()->CheckHresult(l_hr)){
//		throw "XAudio2Create() : failed";
//	}
//
//	//	mastering voiceを作成
//	l_hr = m_pIXAudio->CreateMasteringVoice(&m_pIXAudioMasteringVoice);
//	
//	if (!DX_Debug::GetInstance()->CheckHresult(l_hr)){
//		throw "IXAudio2::CreateMasteringVoice() : failed";
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  メンバ変数を解放
////
////-----------------------------------------------------------------------------------------
//void DX_Audio::Release()
//{
//	if (m_pIXAudioMasteringVoice){
//		m_pIXAudioMasteringVoice->DestroyVoice();
//		m_pIXAudioMasteringVoice = nullptr;
//	}
//
//	if (m_pIXAudio){
//		m_pIXAudio->Release();
//		m_pIXAudio = nullptr;
//	}
//
//	CoUninitialize();
//}