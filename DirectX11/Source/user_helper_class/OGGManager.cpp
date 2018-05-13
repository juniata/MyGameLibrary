//#include	"ALManager.h"
//#include	"OGGManager.h"
//#include	<mutex>
//#include	<process.h>
////-----------------------------------------------------------------------------------------
////
////	staticメンバ変数
////
////-----------------------------------------------------------------------------------------
//std::map<const int, tagOGGFile> OGGManager::m_oggFiles;
//std::vector<std::thread> OGGManager::m_streamThread;
//
////-----------------------------------------------------------------------------------------
////
////	解放した全データを解放
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Release()
//{
//	int l_threadNum = 0;
//	for (auto itr = m_oggFiles.begin(); itr != m_oggFiles.end(); ++itr, ++l_threadNum){
//
//		//	スレッドを終了するフラグを立てる
//		itr->second.bEndThread = true;
//
//		//	スレッドが終了するのを待つ
//		m_streamThread[l_threadNum].join();
//
//		//	バッファ解放
//		if (itr->second.pBuffer){ delete[] itr->second.pBuffer; itr->second.pBuffer = nullptr; }
//
//		//	oggファイルを解放
//		ov_clear(&itr->second.oggVorbisFile);
//
//		// ソースの解放
//		alDeleteSources(1, &itr->second.sourceID);
//
//		// バッファの解放
//		alDeleteBuffers(2, itr->second.bufferID);
//	}
//	m_oggFiles.clear();
//}
//
////-----------------------------------------------------------------------------------------
////
////	ファイルを開く
////
////-----------------------------------------------------------------------------------------
//bool  OGGManager::LoadOGG(const int fileNum, const char* pFilepath)
//{
//	//	新しくファイルを追加
//	m_oggFiles[fileNum] = tagOGGFile();
//
//	//	アドレスを取得
//	tagOGGFile*	l_pOGGFile	= &m_oggFiles[fileNum];
//	
//	//	ファイルパスを保存
//	l_pOGGFile->pFilepath	= pFilepath;
//
//	//	状態をNONEに設定
//	l_pOGGFile->state = OGG_STATE::NONE;
//
//	//	非ループ再生
//	l_pOGGFile->bLoop = true;
//
//	//	スレッド終了フラグ
//	l_pOGGFile->bEndThread = false;
//
//	//	oggファイルを開く。戻り値が0なら成功
//	if (ov_fopen(l_pOGGFile->pFilepath, &l_pOGGFile->oggVorbisFile)){
//		return false;
//	}
//
//	//	OGGファイル情報を取得
//	l_pOGGFile->pVorbisInfo = ov_info(&l_pOGGFile->oggVorbisFile, -1);
//
//	//	チャンネル数によってフォーマットを設定
//	l_pOGGFile->format = (l_pOGGFile->pVorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
//
//	//	バッファサイズを取得
//	l_pOGGFile->bufferSize = (l_pOGGFile->pVorbisInfo->channels * sizeof(unsigned short)* l_pOGGFile->pVorbisInfo->rate);
//
//	//	バッファを生成
//	l_pOGGFile->pBuffer = new char[l_pOGGFile->bufferSize];
//
//	//	バッファIDを取得
//	if (ALManager::GetBuffers(2, l_pOGGFile->bufferID) == false){
//		return false;
//	}
//
//	//	ソースIDを取得
//	if (ALManager::GetSources(1, &l_pOGGFile->sourceID) == false){
//		return false;
//	}
//
//
//	//	成功
//	return true;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	OGGを再生する
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Play(const int fileNum)
//{
//	//	アドレスを取得
//	tagOGGFile*	l_pOGGFile = &m_oggFiles[fileNum];
//
//	switch (l_pOGGFile->state){
//	case OGG_STATE::NONE:
//		
//			//	Play状態にする
//			l_pOGGFile->state = OGG_STATE::PLAY;
//
//			//	threadを追加する
//			m_streamThread.push_back(std::thread(OGGManager::Stream, (void*)fileNum));
//			
//			break;
//	
//	case OGG_STATE::PLAY:break;
//	default:
//		//	Play状態にする
//		l_pOGGFile->state = OGG_STATE::PLAY;
//	
//		//	再生
//		alSourcePlay(l_pOGGFile->sourceID);
//		
//		break;
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	OGGを停止する
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Stop(const int fileNum)
//{
//	//	アドレスを取得
//	tagOGGFile*	l_pOGGFile = &m_oggFiles[fileNum];
//
//	if (l_pOGGFile->state == OGG_STATE::PLAY){
//
//		//	停止状態
//		l_pOGGFile->state = OGG_STATE::STOP;
//
//		//	停止
//		alSourceStop(l_pOGGFile->sourceID);
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	OGGを停止する
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Pause(const int fileNum)
//{
//	//	アドレスを取得
//	tagOGGFile*	l_pOGGFile = &m_oggFiles[fileNum];
//
//	//	再生中なら止める
//	if (l_pOGGFile->state == OGG_STATE::PLAY){
//
//		//	停止状態
//		l_pOGGFile->state = OGG_STATE::PAUSE;
//
//		//	停止
//		alSourcePause(l_pOGGFile->sourceID);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	OGGDataを取得する
////
////-----------------------------------------------------------------------------------------
//tagOGGFile*	OGGManager::GetOGGData(const int fileNum)
//{
//	return &m_oggFiles[fileNum];
//}
//
////-----------------------------------------------------------------------------------------
////
////	OGGBuffferを作成する
////
////-----------------------------------------------------------------------------------------
//bool OGGManager::CreateBuffer(unsigned int* pBufferID, const size_t bufferIDCount, tagOGGFile* pOGGFile)
//{
//	for (unsigned int i = 0; i < bufferIDCount; ++i)
//	{
//
//		size_t l_read = 0;
//
//		while (l_read < pOGGFile->bufferSize)
//		{
//			int l_current = 0;
//			long l_size = ov_read(
//				&pOGGFile->oggVorbisFile,			//	オープンしたoggファイル
//				&pOGGFile->pBuffer[l_read],			//	bufferに音データを格納する	
//				pOGGFile->bufferSize - l_read,	//	bufferのサイズ
//				0,			//	リトルエンディアン
//				2,			//	WORDは16bit
//				1,			//	符号あり
//				&l_current	//	ストリーム中の再生中の位置
//				);
//
//			if (l_size <= 0){
//				return false;
//			}
//			l_read += l_size;
//		}
//
//		alBufferData(
//			pBufferID[i],
//			pOGGFile->format,
//			pOGGFile->pBuffer,
//			pOGGFile->bufferSize,
//			pOGGFile->pVorbisInfo->rate);
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////	ストリーミング再生する
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Stream(void* fileNum)
//{
//	tagOGGFile*	l_pOGGFile = &m_oggFiles[(int)fileNum];
//
//	//	先に先頭データのOGGBufferを作成する
//	CreateBuffer(l_pOGGFile->bufferID, 2, l_pOGGFile);
//
//	//	ソースのキューにバッファ配列を接続
//	ALManager::SourceQueueBuffers(l_pOGGFile->sourceID, 2, l_pOGGFile->bufferID);
//
//	//	再生
//	alSourcePlay(l_pOGGFile->sourceID);
//
//	bool l_pLoop = true;
//	while (l_pLoop)
//	{
//		//	endThreadがtrueならループを抜ける
//		if (l_pOGGFile->bEndThread){
//			l_pLoop = false;
//			break;
//		}
//
//		// 現在のキュー内のプロセス情報取得
//		ALint l_processed;
//		alGetSourcei(l_pOGGFile->sourceID, AL_BUFFERS_PROCESSED, &l_processed);
//		while (l_processed--)
//		{
//			ALuint l_bid;
//			//　プロセス破棄
//			alSourceUnqueueBuffers(l_pOGGFile->sourceID, 1, &l_bid);
//
//			//	バッファを作成し、albufferにバッファデータを接続する
//			if (CreateBuffer(&l_bid, 1, l_pOGGFile) == false){
//				
//				//	もしループ再生フラグがtrueなら、ループ再生する
//				if (l_pOGGFile->bLoop){
//					//  先頭へ戻して
//					ov_pcm_seek(&l_pOGGFile->oggVorbisFile, 0); 
//				}
//				else{
//					//	ループフラグをfalseにする、ループ文を抜ける
//					l_pLoop = false;
//					break;
//				}
//			}
//
//			// ソースのキューにバッファ配列を接続
//			alSourceQueueBuffers(l_pOGGFile->sourceID, 1, &l_bid);
//		}
//	}
//}