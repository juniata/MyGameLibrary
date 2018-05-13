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
//	NONE,	//	���ݒ�
//	PLAY,	//	�Đ���
//	STOP,	//	��~
//	PAUSE,	//	��~��
//};
//
////	OGG���
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
//	//  @brief		�m�ۂ����S�f�[�^�����
//	//
//	//------------------------------------------------------------------------------
//	static void Release();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGG�t�@�C����ǂݍ���
//	//	@param[in]	fileNum		�ݒ肷��t�@�C���ԍ�
//	//	@param[in]	pFilepath	�t�@�C���p�X
//	//	@return		true:����	false:���s
//	//
//	//------------------------------------------------------------------------------
//	static bool LoadOGG(const int fileNum, const char* pFilepath);
//	
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGG���Đ�����
//	//	@param[in]	fileNum		�ݒ肵���t�@�C���ԍ�
//	//
//	//------------------------------------------------------------------------------
//	static void Play(const int fileNum);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGG�̍Đ����~����
//	//	@param[in]	fileNum		�ݒ肵���t�@�C���ԍ�
//	//	@note		Stop���Play������ƁA�Ȃ̍ŏ�����X�^�[�g
//	//
//	//------------------------------------------------------------------------------
//	static void Stop(const int fileNum);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGG�̍Đ����~�߂�
//	//	@param[in]	fileNum		�ݒ肵���t�@�C���ԍ�
//	//	@note		Pause���Play������ƁA�Ȃ̓r������X�^�[�g
//	//
//	//------------------------------------------------------------------------------
//	static void Pause(const int fileNum);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGG�f�[�^���擾����
//	//	@return		m_oggFiles[fileNum]
//	//
//	//------------------------------------------------------------------------------
//	static tagOGGFile*	GetOGGData(const int fileNum);
//
//private:
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		OGG�o�b�t�@���쐬����
//	//	@param[in]	pOGGFile	m_oggFiles�ɒǉ������f�[�^
//	//
//	//------------------------------------------------------------------------------
//	static bool CreateBuffer(unsigned int* pBufferID,const size_t bufferIDCount,tagOGGFile* pOGGFile);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		Stream�Đ�������
//	//	@param[in]	fileNum	�ݒ肵���t�@�C���ԍ�
//	//	@note		�ʃX���b�h�ŃX�g���[�~���O
//	//
//	//------------------------------------------------------------------------------
//	static void Stream(void* fileNum);
//	
//};
//#endif // !__OGG_MANAGER_H_
