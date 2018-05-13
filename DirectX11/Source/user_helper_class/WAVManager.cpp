//#include	"ALManager.h"
//#include	"WAVManager.h"
//
////-----------------------------------------------------------------------------------------
////
////	static�����o�ϐ�
////
////-----------------------------------------------------------------------------------------
//std::map<const int, tagWAVFile> WAVManager::m_wavFiles;
//
//
//
////-----------------------------------------------------------------------------------------
////
////	WAV�t�@�C����ǂݍ���
////
////-----------------------------------------------------------------------------------------
//bool WAVManager::LoadWAV(const int fileNum, const char* pFilepath)
//{
//	//	�V�����t�@�C����ǉ�
//	m_wavFiles[fileNum] = tagWAVFile();
//
//	//	�A�h���X���擾
//	tagWAVFile*	l_pWAVFile = &m_wavFiles[fileNum];
//
//	//	�t�@�C���p�X��ۑ�
//	l_pWAVFile->pFilepath = pFilepath;
//
//	//	wav�t�@�C�����J��
//	FILE* l_pFp = nullptr;
//	fopen_s(&l_pFp, pFilepath, "rb");
//
//	//	�t�@�C���ǂݍ��݃`�F�b�N
//	if (l_pFp == nullptr){
//		return false;
//	}
//
//	//	riff�`�F�b�N
//	fread_s(&l_pWAVFile->riff, sizeof(l_pWAVFile->riff), sizeof(l_pWAVFile->riff), 1, l_pFp);
//
//	//	WAV�t�@�C�������擾
//	GetWAVInfo(l_pWAVFile,l_pFp);
//
//	//	�`�����l�����ɂ���ăt�H�[�}�b�g��ݒ�
//	l_pWAVFile->format = (l_pWAVFile->fileFormat.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
//
//	//	�o�b�t�@�T�C�Y���擾
//	l_pWAVFile->bufferSize = l_pWAVFile->fileFormat.bytesPerSecond;
//
//	//	�o�b�t�@�𐶐�
//	l_pWAVFile->pBuffer = new char[l_pWAVFile->bufferSize];
//
//	//	�o�b�t�@ID���擾
//	if (ALManager::GetBuffers(2, l_pWAVFile->bufferID) == false){
//		return false;
//	}
//
//	//	�\�[�XID���擾
//	if (ALManager::GetSources(1, &l_pWAVFile->sourceID) == false){
//		return false;
//	}
//
//	//	WAVBuffer���쐬����
//	CreateBuffer(l_pWAVFile, l_pFp);
//	return true;
//}
//
//void WAVManager::GetWAVInfo(tagWAVFile* pWAVFile,FILE* pFile)
//{
//	//	�`�����N
//	load_wav_struct::tagChunk l_chunk;
//
//	while (true)
//	{
//		//	chunk������
//		l_chunk.size = 0;
//		l_chunk.id[3] = l_chunk.id[2] =
//		l_chunk.id[1] = l_chunk.id[0] = 0;
//
//		//	�t�@�C���ǂݍ���
//		fread_s(&l_chunk, sizeof(l_chunk), sizeof(l_chunk), 1, pFile);
//
//		if (l_chunk.size <= 0){ break; }
//
//		//	�t�H�[�}�b�g
//		if (strncmp(l_chunk.id, "fmt ", 4) == 0){
//			size_t minSize = (l_chunk.size < sizeof(load_wav_struct::tagWaveFileFormat)) ? l_chunk.size : sizeof(load_wav_struct::tagWaveFileFormat);
//
//			fread_s(&pWAVFile->fileFormat, minSize, minSize, 1, pFile);
//
//			fseek(pFile, l_chunk.size - sizeof(load_wav_struct::tagWaveFileFormat), SEEK_CUR);
//		
//		}
//		else if (strncmp(l_chunk.id, "data", 4) == 0){
//			//	�f�[�^��������������break
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
