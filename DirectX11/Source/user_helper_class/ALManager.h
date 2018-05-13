//#ifndef __AL_MANAGER_H_
//#define __AL_MANAGER_H_
//
//#include	<al.h>
//#include	<alc.h>
//
////****************************************************************************************************
////
////	ALManager
////
////****************************************************************************************************
//class ALManager
//{
//private:
//	static ALCdevice*	m_pDevice;
//	static ALCcontext*	m_pContext;
//
//public:
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		デバイスとコンテキストを取得、または作成
//	//	@note		生成失敗の場合 thorw char*される
//	//
//	//------------------------------------------------------------------------------
//	static void Initialize();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		デバイスとコンテキストを解放
//	//
//	//------------------------------------------------------------------------------
//	static void Release();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief			バッファを作成する
//	//	@param[in]		bufferCount	バッファ数
//	//	@param[inout]	pBuffersID	ALuint配列
//	//	@return			true:成功	false:失敗
//	//
//	//------------------------------------------------------------------------------
//	static bool GetBuffers(
//		const ALsizei	bufferCount,
//		ALuint*			pBuffersID
//		);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief			バッファを作成する
//	//	@param[in]		sourceCount	ソース数
//	//	@param[inout]	pSurceID	ALuint配列
//	//	@return			true:成功	false:失敗
//	//
//	//------------------------------------------------------------------------------
//	static bool GetSources(
//		const ALsizei	sourceCount,
//		ALuint*			pSurceID
//		);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		ソースのキューにバッファ配列を接続
//	//	@param[in]	sourceID	ソースID
//	//	@param[in]	bufferCount	バッファID配列数
//	//	@param[in]	pBuffersID	バッファID配列
//	//	@return		true:成功	false:失敗
//	//
//	//------------------------------------------------------------------------------
//	static bool SourceQueueBuffers(
//		const ALuint	sourceID,
//		const ALsizei	bufferCount,
//		ALuint*			pBuffersID
//		);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		16bitのモノラルデータを設定
//	//	@param[in]	bufferID	バッファID
//	//	@param[in]	pBuffer		曲データ
//	//	@param[in]	bufferSize	データサイズ
//	//	@param[in]	frequency	周波数
//	//
//	//------------------------------------------------------------------------------
//	static bool SetBufferDataMono16bit(
//		const ALuint	bufferID,
//		const ALvoid*	pBuffer,
//		const ALsizei	bufferSize,
//		const ALsizei	frequency
//		);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		16bitのステレオデータを設定
//	//	@param[in]	bufferID	バッファID
//	//	@param[in]	pBuffer		曲データ
//	//	@param[in]	bufferSize	データサイズ
//	//	@param[in]	frequency	周波数
//	//
//	//------------------------------------------------------------------------------
//	static bool SetBufferDataStereo16bit(
//		const ALuint	bufferID,
//		const ALvoid*	pBuffer,
//		const ALsizei	bufferSize,
//		const ALsizei	frequency
//		);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		ループ設定
//	//	@param[in]	sourceID	ソースID
//	//	@param[in]	bLoop		true:ループ		false:非ループ
//	//
//	//------------------------------------------------------------------------------
//	static void SetLoopFlag(const unsigned int sourceID, const bool bLoop);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		音量を設定
//	//	@param[in]	sourceID	ソースID
//	//	@param[in]	volume		音量
//	//
//	//------------------------------------------------------------------------------
//	static void SetVolume(const unsigned int sourceID, const float volume);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		音程を設定
//	//	@param[in]	sourceID	ソースID
//	//	@param[in]	volume		音程
//	//	@note		0.5f <= volume <= 2.0f
//	//
//	//------------------------------------------------------------------------------
//	static void SetPitch(const unsigned int sourceID, const float pitch);
//
//};
//#endif // !__AL_MANAGER_H_
