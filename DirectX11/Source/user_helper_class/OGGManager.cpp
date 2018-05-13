//#include	"ALManager.h"
//#include	"OGGManager.h"
//#include	<mutex>
//#include	<process.h>
////-----------------------------------------------------------------------------------------
////
////	static�����o�ϐ�
////
////-----------------------------------------------------------------------------------------
//std::map<const int, tagOGGFile> OGGManager::m_oggFiles;
//std::vector<std::thread> OGGManager::m_streamThread;
//
////-----------------------------------------------------------------------------------------
////
////	��������S�f�[�^�����
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Release()
//{
//	int l_threadNum = 0;
//	for (auto itr = m_oggFiles.begin(); itr != m_oggFiles.end(); ++itr, ++l_threadNum){
//
//		//	�X���b�h���I������t���O�𗧂Ă�
//		itr->second.bEndThread = true;
//
//		//	�X���b�h���I������̂�҂�
//		m_streamThread[l_threadNum].join();
//
//		//	�o�b�t�@���
//		if (itr->second.pBuffer){ delete[] itr->second.pBuffer; itr->second.pBuffer = nullptr; }
//
//		//	ogg�t�@�C�������
//		ov_clear(&itr->second.oggVorbisFile);
//
//		// �\�[�X�̉��
//		alDeleteSources(1, &itr->second.sourceID);
//
//		// �o�b�t�@�̉��
//		alDeleteBuffers(2, itr->second.bufferID);
//	}
//	m_oggFiles.clear();
//}
//
////-----------------------------------------------------------------------------------------
////
////	�t�@�C�����J��
////
////-----------------------------------------------------------------------------------------
//bool  OGGManager::LoadOGG(const int fileNum, const char* pFilepath)
//{
//	//	�V�����t�@�C����ǉ�
//	m_oggFiles[fileNum] = tagOGGFile();
//
//	//	�A�h���X���擾
//	tagOGGFile*	l_pOGGFile	= &m_oggFiles[fileNum];
//	
//	//	�t�@�C���p�X��ۑ�
//	l_pOGGFile->pFilepath	= pFilepath;
//
//	//	��Ԃ�NONE�ɐݒ�
//	l_pOGGFile->state = OGG_STATE::NONE;
//
//	//	�񃋁[�v�Đ�
//	l_pOGGFile->bLoop = true;
//
//	//	�X���b�h�I���t���O
//	l_pOGGFile->bEndThread = false;
//
//	//	ogg�t�@�C�����J���B�߂�l��0�Ȃ琬��
//	if (ov_fopen(l_pOGGFile->pFilepath, &l_pOGGFile->oggVorbisFile)){
//		return false;
//	}
//
//	//	OGG�t�@�C�������擾
//	l_pOGGFile->pVorbisInfo = ov_info(&l_pOGGFile->oggVorbisFile, -1);
//
//	//	�`�����l�����ɂ���ăt�H�[�}�b�g��ݒ�
//	l_pOGGFile->format = (l_pOGGFile->pVorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
//
//	//	�o�b�t�@�T�C�Y���擾
//	l_pOGGFile->bufferSize = (l_pOGGFile->pVorbisInfo->channels * sizeof(unsigned short)* l_pOGGFile->pVorbisInfo->rate);
//
//	//	�o�b�t�@�𐶐�
//	l_pOGGFile->pBuffer = new char[l_pOGGFile->bufferSize];
//
//	//	�o�b�t�@ID���擾
//	if (ALManager::GetBuffers(2, l_pOGGFile->bufferID) == false){
//		return false;
//	}
//
//	//	�\�[�XID���擾
//	if (ALManager::GetSources(1, &l_pOGGFile->sourceID) == false){
//		return false;
//	}
//
//
//	//	����
//	return true;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	OGG���Đ�����
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Play(const int fileNum)
//{
//	//	�A�h���X���擾
//	tagOGGFile*	l_pOGGFile = &m_oggFiles[fileNum];
//
//	switch (l_pOGGFile->state){
//	case OGG_STATE::NONE:
//		
//			//	Play��Ԃɂ���
//			l_pOGGFile->state = OGG_STATE::PLAY;
//
//			//	thread��ǉ�����
//			m_streamThread.push_back(std::thread(OGGManager::Stream, (void*)fileNum));
//			
//			break;
//	
//	case OGG_STATE::PLAY:break;
//	default:
//		//	Play��Ԃɂ���
//		l_pOGGFile->state = OGG_STATE::PLAY;
//	
//		//	�Đ�
//		alSourcePlay(l_pOGGFile->sourceID);
//		
//		break;
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	OGG���~����
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Stop(const int fileNum)
//{
//	//	�A�h���X���擾
//	tagOGGFile*	l_pOGGFile = &m_oggFiles[fileNum];
//
//	if (l_pOGGFile->state == OGG_STATE::PLAY){
//
//		//	��~���
//		l_pOGGFile->state = OGG_STATE::STOP;
//
//		//	��~
//		alSourceStop(l_pOGGFile->sourceID);
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	OGG���~����
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Pause(const int fileNum)
//{
//	//	�A�h���X���擾
//	tagOGGFile*	l_pOGGFile = &m_oggFiles[fileNum];
//
//	//	�Đ����Ȃ�~�߂�
//	if (l_pOGGFile->state == OGG_STATE::PLAY){
//
//		//	��~���
//		l_pOGGFile->state = OGG_STATE::PAUSE;
//
//		//	��~
//		alSourcePause(l_pOGGFile->sourceID);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	OGGData���擾����
////
////-----------------------------------------------------------------------------------------
//tagOGGFile*	OGGManager::GetOGGData(const int fileNum)
//{
//	return &m_oggFiles[fileNum];
//}
//
////-----------------------------------------------------------------------------------------
////
////	OGGBufffer���쐬����
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
//				&pOGGFile->oggVorbisFile,			//	�I�[�v������ogg�t�@�C��
//				&pOGGFile->pBuffer[l_read],			//	buffer�ɉ��f�[�^���i�[����	
//				pOGGFile->bufferSize - l_read,	//	buffer�̃T�C�Y
//				0,			//	���g���G���f�B�A��
//				2,			//	WORD��16bit
//				1,			//	��������
//				&l_current	//	�X�g���[�����̍Đ����̈ʒu
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
////	�X�g���[�~���O�Đ�����
////
////-----------------------------------------------------------------------------------------
//void OGGManager::Stream(void* fileNum)
//{
//	tagOGGFile*	l_pOGGFile = &m_oggFiles[(int)fileNum];
//
//	//	��ɐ擪�f�[�^��OGGBuffer���쐬����
//	CreateBuffer(l_pOGGFile->bufferID, 2, l_pOGGFile);
//
//	//	�\�[�X�̃L���[�Ƀo�b�t�@�z���ڑ�
//	ALManager::SourceQueueBuffers(l_pOGGFile->sourceID, 2, l_pOGGFile->bufferID);
//
//	//	�Đ�
//	alSourcePlay(l_pOGGFile->sourceID);
//
//	bool l_pLoop = true;
//	while (l_pLoop)
//	{
//		//	endThread��true�Ȃ烋�[�v�𔲂���
//		if (l_pOGGFile->bEndThread){
//			l_pLoop = false;
//			break;
//		}
//
//		// ���݂̃L���[���̃v���Z�X���擾
//		ALint l_processed;
//		alGetSourcei(l_pOGGFile->sourceID, AL_BUFFERS_PROCESSED, &l_processed);
//		while (l_processed--)
//		{
//			ALuint l_bid;
//			//�@�v���Z�X�j��
//			alSourceUnqueueBuffers(l_pOGGFile->sourceID, 1, &l_bid);
//
//			//	�o�b�t�@���쐬���Aalbuffer�Ƀo�b�t�@�f�[�^��ڑ�����
//			if (CreateBuffer(&l_bid, 1, l_pOGGFile) == false){
//				
//				//	�������[�v�Đ��t���O��true�Ȃ�A���[�v�Đ�����
//				if (l_pOGGFile->bLoop){
//					//  �擪�֖߂���
//					ov_pcm_seek(&l_pOGGFile->oggVorbisFile, 0); 
//				}
//				else{
//					//	���[�v�t���O��false�ɂ���A���[�v���𔲂���
//					l_pLoop = false;
//					break;
//				}
//			}
//
//			// �\�[�X�̃L���[�Ƀo�b�t�@�z���ڑ�
//			alSourceQueueBuffers(l_pOGGFile->sourceID, 1, &l_bid);
//		}
//	}
//}