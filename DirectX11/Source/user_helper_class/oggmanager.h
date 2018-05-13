//#ifndef __OGG_MANAGER_H_
//#define __OGG_MANAGER_H_
//
//
//#include	<vorbis\vorbisfile.h>
//
//#ifndef _MAP_
//#include	<map>
//#endif
//
//#ifndef _VECTOR_
//#include	<vector>
//#endif
//enum OGG_STATE
//{
//	NONE,	//	未設定
//	PLAY,	//	再生中
//	STOP,	//	停止
//	PAUSE,	//	停止中
//};
//
////	OGG情報
//struct tagOGGFile{
//	OggVorbis_File	oggVorbisFile;
//	vorbis_info*	pVorbisInfo;
//	size_t			bufferSize;
//	char*			pBuffer;
//	const char*		pFilepath;
//	int				format;
//	unsigned int	bufferID[2];
//	unsigned int	sourceID;
//	OGG_STATE		state;
//	bool			bLoop;
//	bool			bEndThread;
//};
//
//#include	<thread>
////****************************************************************************************************
////
////	OGGManager
////
////****************************************************************************************************
//class OGGManager
//{
//private:
//	static std::map<const int, tagOGGFile> m_oggFiles;
//	static std::vector<std::thread> m_streamThread;
//
//public:
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		確保した全データを解放
//	//
//	//------------------------------------------------------------------------------
//	static void Release();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGGファイルを読み込む
//	//	@param[in]	fileNum		設定するファイル番号
//	//	@param[in]	pFilepath	ファイルパス
//	//	@return		true:成功	false:失敗
//	//
//	//------------------------------------------------------------------------------
//	static bool LoadOGG(const int fileNum, const char* pFilepath);
//	
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGGを再生する
//	//	@param[in]	fileNum		設定したファイル番号
//	//
//	//------------------------------------------------------------------------------
//	static void Play(const int fileNum);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGGの再生を停止する
//	//	@param[in]	fileNum		設定したファイル番号
//	//	@note		Stop後にPlayをすると、曲の最初からスタート
//	//
//	//------------------------------------------------------------------------------
//	static void Stop(const int fileNum);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGGの再生を止める
//	//	@param[in]	fileNum		設定したファイル番号
//	//	@note		Pause後にPlayをすると、曲の途中からスタート
//	//
//	//------------------------------------------------------------------------------
//	static void Pause(const int fileNum);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGGデータを取得する
//	//	@return		m_oggFiles[fileNum]
//	//
//	//------------------------------------------------------------------------------
//	static tagOGGFile*	GetOGGData(const int fileNum);
//
//private:
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGGバッファを作成する
//	//	@param[in]	pOGGFile	m_oggFilesに追加したデータ
//	//
//	//------------------------------------------------------------------------------
//	static bool CreateBuffer(unsigned int* pBufferID,const size_t bufferIDCount,tagOGGFile* pOGGFile);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		Stream再生をする
//	//	@param[in]	fileNum	設定したファイル番号
//	//	@note		別スレッドでストリーミング
//	//
//	//------------------------------------------------------------------------------
//	static void Stream(void* fileNum);
//	
//};
//#endif // !__OGG_MANAGER_H_
