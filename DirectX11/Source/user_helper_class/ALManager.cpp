//#include	"ALManager.h"
//
////-----------------------------------------------------------------------------------------
////
////	staticメンバ変数
////
////-----------------------------------------------------------------------------------------
//ALCdevice*	ALManager::m_pDevice	=	nullptr;
//ALCcontext*	ALManager::m_pContext	=	nullptr;
//
////-----------------------------------------------------------------------------------------
////
////	デバイスとコンテキストを取得、または作成
////
////-----------------------------------------------------------------------------------------
//void ALManager::Initialize()
//{
//	//	デバイスを取得
//	m_pDevice = alcOpenDevice(nullptr);
//
//	if (m_pDevice == nullptr){
//		throw "alcOpenDevice() : failed";
//	}
//
//	//	コンテキストを取得
//	m_pContext = alcGetCurrentContext();
//
//	//	コンテキストの取得に失敗した場合
//	if (m_pContext == nullptr){
//		//	コンテキストを作成する
//		m_pContext = alcCreateContext(m_pDevice, nullptr);
//
//		if (m_pContext == nullptr){
//			throw "alcGetCurrentContext() and alcCreateContext() : failed";
//		}
//	}
//
//	//	コンテキストを操作対象にする
//	if (alcMakeContextCurrent(m_pContext) == ALC_FALSE){
//		throw "alcMakeContextCurrent() : failed";
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	デバイスとコンテキストを解放
////
////-----------------------------------------------------------------------------------------
//void ALManager::Release()
//{
//	// 操作対象のコンテキストを無効化
//	alcMakeContextCurrent(nullptr);
//
//	// コンテキストを破壊
//	alcDestroyContext(m_pContext);
//
//	// デバイスを閉じる
//	alcCloseDevice(m_pDevice);
//}
//
////-----------------------------------------------------------------------------------------
////
////	バッファIDを取得する
////
////-----------------------------------------------------------------------------------------
//bool ALManager::GetBuffers(const ALsizei bufferCount, ALuint* pBuffersID)
//{
//	//	エラー情報をクリアする
//	alGetError();
//
//	//	バッファを取得
//	alGenBuffers(bufferCount, pBuffersID);
//
//	//	エラーチェック
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////	ソースIDを取得する
////
////-----------------------------------------------------------------------------------------
//bool ALManager::GetSources(const ALsizei sourceCount, ALuint* pSourceID)
//{
//	//	エラー情報をクリアする
//	alGetError();
//
//	//	バッファを取得
//	alGenSources(sourceCount, pSourceID);
//
//	//	エラーチェック
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////	ソースのキューにバッファ配列を接続
////
////-----------------------------------------------------------------------------------------
//bool ALManager::SourceQueueBuffers(
//	const ALuint	sourceID,
//	const ALsizei	bufferCount,
//	ALuint*			pBuffersID
//	)
//{
//	//	エラー情報をクリアする
//	alGetError();
//
//	//	ソースのキューにバッファ配列を接続
//	alSourceQueueBuffers(sourceID, bufferCount, pBuffersID);
//	
//	//	エラーチェック
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//
//}
////-----------------------------------------------------------------------------------------
////
////	16bitのモノラルデータを設定
////
////-----------------------------------------------------------------------------------------
//bool ALManager::SetBufferDataMono16bit(
//	const ALuint	bufferID,
//	const ALvoid*	pBuffer,
//	const ALsizei	bufferSize,
//	const ALsizei	frequency
//	)
//{
//	//	エラー情報をクリアする
//	alGetError();
//
//	alBufferData(bufferID, AL_FORMAT_MONO16, pBuffer, bufferSize, frequency);
//
//	//	エラーチェック
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	16bitのステレオデータを設定
////
////-----------------------------------------------------------------------------------------
//bool ALManager::SetBufferDataStereo16bit(
//	const ALuint	bufferID,
//	const ALvoid*	pBuffer,
//	const ALsizei	bufferSize,
//	const ALsizei	frequency
//	)
//{	//	エラー情報をクリアする
//	alGetError();
//
//	alBufferData(bufferID, AL_FORMAT_STEREO16, pBuffer, bufferSize, frequency);
//
//	//	エラーチェック
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////	ループ設定
////
////-----------------------------------------------------------------------------------------
//void ALManager::SetLoopFlag(const unsigned int sourceID, const bool bLoop)
//{
//	alSourcei(sourceID, AL_LOOPING, (bLoop) ? AL_TRUE : AL_FALSE);
//}
//
////-----------------------------------------------------------------------------------------
////
////	音量を設定
////
////-----------------------------------------------------------------------------------------
//void ALManager::SetVolume(const unsigned int sourceID, const float volume)
//{
//	alSourcef(sourceID, AL_GAIN, volume);
//}
//
////-----------------------------------------------------------------------------------------
////
////	音程を設定
////
////-----------------------------------------------------------------------------------------
//void ALManager::SetPitch(const unsigned int sourceID, const float pitch)
//{
//	alSourcef(sourceID, AL_PITCH, pitch);
//}