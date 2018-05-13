//#include	"ALManager.h"
//#include	"WAVManager.h"
//
////-----------------------------------------------------------------------------------------
////
////	staticメンバ変数
////
////-----------------------------------------------------------------------------------------
//std::map<const int, tagWAVFile> WAVManager::m_wavFiles;
//
//
//
////-----------------------------------------------------------------------------------------
////
////	WAVファイルを読み込む
////
////-----------------------------------------------------------------------------------------
//bool WAVManager::LoadWAV(const int fileNum, const char* pFilepath)
//{
//	//	新しくファイルを追加
//	m_wavFiles[fileNum] = tagWAVFile();
//
//	//	アドレスを取得
//	tagWAVFile*	l_pWAVFile = &m_wavFiles[fileNum];
//
//	//	ファイルパスを保存
//	l_pWAVFile->pFilepath = pFilepath;
//
//	//	wavファイルを開く
//	FILE* l_pFp = nullptr;
//	fopen_s(&l_pFp, pFilepath, "rb");
//
//	//	ファイル読み込みチェック
//	if (l_pFp == nullptr){
//		return false;
//	}
//
//	//	riffチェック
//	fread_s(&l_pWAVFile->riff, sizeof(l_pWAVFile->riff), sizeof(l_pWAVFile->riff), 1, l_pFp);
//
//	//	WAVファイル情報を取得
//	GetWAVInfo(l_pWAVFile,l_pFp);
//
//	//	チャンネル数によってフォーマットを設定
//	l_pWAVFile->format = (l_pWAVFile->fileFormat.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
//
//	//	バッファサイズを取得
//	l_pWAVFile->bufferSize = l_pWAVFile->fileFormat.bytesPerSecond;
//
//	//	バッファを生成
//	l_pWAVFile->pBuffer = new char[l_pWAVFile->bufferSize];
//
//	//	バッファIDを取得
//	if (ALManager::GetBuffers(2, l_pWAVFile->bufferID) == false){
//		return false;
//	}
//
//	//	ソースIDを取得
//	if (ALManager::GetSources(1, &l_pWAVFile->sourceID) == false){
//		return false;
//	}
//
//	//	WAVBufferを作成する
//	CreateBuffer(l_pWAVFile, l_pFp);
//	return true;
//}
//
//void WAVManager::GetWAVInfo(tagWAVFile* pWAVFile,FILE* pFile)
//{
//	//	チャンク
//	load_wav_struct::tagChunk l_chunk;
//
//	while (true)
//	{
//		//	chunk初期化
//		l_chunk.size = 0;
//		l_chunk.id[3] = l_chunk.id[2] =
//		l_chunk.id[1] = l_chunk.id[0] = 0;
//
//		//	ファイル読み込み
//		fread_s(&l_chunk, sizeof(l_chunk), sizeof(l_chunk), 1, pFile);
//
//		if (l_chunk.size <= 0){ break; }
//
//		//	フォーマット
//		if (strncmp(l_chunk.id, "fmt ", 4) == 0){
//			size_t minSize = (l_chunk.size < sizeof(load_wav_struct::tagWaveFileFormat)) ? l_chunk.size : sizeof(load_wav_struct::tagWaveFileFormat);
//
//			fread_s(&pWAVFile->fileFormat, minSize, minSize, 1, pFile);
//
//			fseek(pFile, l_chunk.size - sizeof(load_wav_struct::tagWaveFileFormat), SEEK_CUR);
//		
//		}
//		else if (strncmp(l_chunk.id, "data", 4) == 0){
//			//	データ部が見つかったらbreak
//			break;
//		}
//		else{
//			fseek(pFile, l_chunk.size, SEEK_CUR);
//		}
//	}
//}
//
//void WAVManager::CreateBuffer(tagWAVFile* pWAVFile, FILE* pFile)
//{
//	for (int i = 0; i < 2; ++i)
//	{
//		size_t l_read = 0;
//
//		while (l_read < pWAVFile->bufferSize)
//		{
//			size_t l_size = fread_s(
//				&pWAVFile->pBuffer[l_read],
//				pWAVFile->bufferSize - l_read,
//				pWAVFile->bufferSize - l_read,
//				1,
//				pFile
//				);
//
//			if (l_size <= 0){
//				break;
//			}
//			l_read += l_size;
//		}
//		alBufferData(
//			pWAVFile->bufferID[i],
//			pWAVFile->format,
//			pWAVFile->pBuffer,
//			pWAVFile->bufferSize,
//			pWAVFile->fileFormat.samplePerSecond
//			);
//	}
//	
//}
//unsigned int WAVManager::BufferRead(
//	FILE* pFile,
//	char* pBuffer,
//	const size_t bufferSize
//	)
//{
//
//
//	size_t l_size = fread_s(pBuffer, bufferSize, bufferSize, 1, pFile);
//	fseek(pFile, bufferSize, SEEK_CUR);
//
//	return l_size;
//
//}
