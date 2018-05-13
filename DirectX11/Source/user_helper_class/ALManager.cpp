//#include	"ALManager.h"
//
////-----------------------------------------------------------------------------------------
////
////	static�����o�ϐ�
////
////-----------------------------------------------------------------------------------------
//ALCdevice*	ALManager::m_pDevice	=	nullptr;
//ALCcontext*	ALManager::m_pContext	=	nullptr;
//
////-----------------------------------------------------------------------------------------
////
////	�f�o�C�X�ƃR���e�L�X�g���擾�A�܂��͍쐬
////
////-----------------------------------------------------------------------------------------
//void ALManager::Initialize()
//{
//	//	�f�o�C�X���擾
//	m_pDevice = alcOpenDevice(nullptr);
//
//	if (m_pDevice == nullptr){
//		throw "alcOpenDevice() : failed";
//	}
//
//	//	�R���e�L�X�g���擾
//	m_pContext = alcGetCurrentContext();
//
//	//	�R���e�L�X�g�̎擾�Ɏ��s�����ꍇ
//	if (m_pContext == nullptr){
//		//	�R���e�L�X�g���쐬����
//		m_pContext = alcCreateContext(m_pDevice, nullptr);
//
//		if (m_pContext == nullptr){
//			throw "alcGetCurrentContext() and alcCreateContext() : failed";
//		}
//	}
//
//	//	�R���e�L�X�g�𑀍�Ώۂɂ���
//	if (alcMakeContextCurrent(m_pContext) == ALC_FALSE){
//		throw "alcMakeContextCurrent() : failed";
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	�f�o�C�X�ƃR���e�L�X�g�����
////
////-----------------------------------------------------------------------------------------
//void ALManager::Release()
//{
//	// ����Ώۂ̃R���e�L�X�g�𖳌���
//	alcMakeContextCurrent(nullptr);
//
//	// �R���e�L�X�g��j��
//	alcDestroyContext(m_pContext);
//
//	// �f�o�C�X�����
//	alcCloseDevice(m_pDevice);
//}
//
////-----------------------------------------------------------------------------------------
////
////	�o�b�t�@ID���擾����
////
////-----------------------------------------------------------------------------------------
//bool ALManager::GetBuffers(const ALsizei bufferCount, ALuint* pBuffersID)
//{
//	//	�G���[�����N���A����
//	alGetError();
//
//	//	�o�b�t�@���擾
//	alGenBuffers(bufferCount, pBuffersID);
//
//	//	�G���[�`�F�b�N
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////	�\�[�XID���擾����
////
////-----------------------------------------------------------------------------------------
//bool ALManager::GetSources(const ALsizei sourceCount, ALuint* pSourceID)
//{
//	//	�G���[�����N���A����
//	alGetError();
//
//	//	�o�b�t�@���擾
//	alGenSources(sourceCount, pSourceID);
//
//	//	�G���[�`�F�b�N
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////	�\�[�X�̃L���[�Ƀo�b�t�@�z���ڑ�
////
////-----------------------------------------------------------------------------------------
//bool ALManager::SourceQueueBuffers(
//	const ALuint	sourceID,
//	const ALsizei	bufferCount,
//	ALuint*			pBuffersID
//	)
//{
//	//	�G���[�����N���A����
//	alGetError();
//
//	//	�\�[�X�̃L���[�Ƀo�b�t�@�z���ڑ�
//	alSourceQueueBuffers(sourceID, bufferCount, pBuffersID);
//	
//	//	�G���[�`�F�b�N
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//
//}
////-----------------------------------------------------------------------------------------
////
////	16bit�̃��m�����f�[�^��ݒ�
////
////-----------------------------------------------------------------------------------------
//bool ALManager::SetBufferDataMono16bit(
//	const ALuint	bufferID,
//	const ALvoid*	pBuffer,
//	const ALsizei	bufferSize,
//	const ALsizei	frequency
//	)
//{
//	//	�G���[�����N���A����
//	alGetError();
//
//	alBufferData(bufferID, AL_FORMAT_MONO16, pBuffer, bufferSize, frequency);
//
//	//	�G���[�`�F�b�N
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
////	16bit�̃X�e���I�f�[�^��ݒ�
////
////-----------------------------------------------------------------------------------------
//bool ALManager::SetBufferDataStereo16bit(
//	const ALuint	bufferID,
//	const ALvoid*	pBuffer,
//	const ALsizei	bufferSize,
//	const ALsizei	frequency
//	)
//{	//	�G���[�����N���A����
//	alGetError();
//
//	alBufferData(bufferID, AL_FORMAT_STEREO16, pBuffer, bufferSize, frequency);
//
//	//	�G���[�`�F�b�N
//	if (alGetError() != AL_NO_ERROR){
//		return false;
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////	���[�v�ݒ�
////
////-----------------------------------------------------------------------------------------
//void ALManager::SetLoopFlag(const unsigned int sourceID, const bool bLoop)
//{
//	alSourcei(sourceID, AL_LOOPING, (bLoop) ? AL_TRUE : AL_FALSE);
//}
//
////-----------------------------------------------------------------------------------------
////
////	���ʂ�ݒ�
////
////-----------------------------------------------------------------------------------------
//void ALManager::SetVolume(const unsigned int sourceID, const float volume)
//{
//	alSourcef(sourceID, AL_GAIN, volume);
//}
//
////-----------------------------------------------------------------------------------------
////
////	������ݒ�
////
////-----------------------------------------------------------------------------------------
//void ALManager::SetPitch(const unsigned int sourceID, const float pitch)
//{
//	alSourcef(sourceID, AL_PITCH, pitch);
//}