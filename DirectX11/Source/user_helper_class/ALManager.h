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
//	//  @brief		�f�o�C�X�ƃR���e�L�X�g���擾�A�܂��͍쐬
//	//	@note		�������s�̏ꍇ thorw char*�����
//	//
//	//------------------------------------------------------------------------------
//	static void Initialize();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�f�o�C�X�ƃR���e�L�X�g�����
//	//
//	//------------------------------------------------------------------------------
//	static void Release();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief			�o�b�t�@���쐬����
//	//	@param[in]		bufferCount	�o�b�t�@��
//	//	@param[inout]	pBuffersID	ALuint�z��
//	//	@return			true:����	false:���s
//	//
//	//------------------------------------------------------------------------------
//	static bool GetBuffers(
//		const ALsizei	bufferCount,
//		ALuint*			pBuffersID
//		);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief			�o�b�t�@���쐬����
//	//	@param[in]		sourceCount	�\�[�X��
//	//	@param[inout]	pSurceID	ALuint�z��
//	//	@return			true:����	false:���s
//	//
//	//------------------------------------------------------------------------------
//	static bool GetSources(
//		const ALsizei	sourceCount,
//		ALuint*			pSurceID
//		);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�\�[�X�̃L���[�Ƀo�b�t�@�z���ڑ�
//	//	@param[in]	sourceID	�\�[�XID
//	//	@param[in]	bufferCount	�o�b�t�@ID�z��
//	//	@param[in]	pBuffersID	�o�b�t�@ID�z��
//	//	@return		true:����	false:���s
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
//	//  @brief		16bit�̃��m�����f�[�^��ݒ�
//	//	@param[in]	bufferID	�o�b�t�@ID
//	//	@param[in]	pBuffer		�ȃf�[�^
//	//	@param[in]	bufferSize	�f�[�^�T�C�Y
//	//	@param[in]	frequency	���g��
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
//	//  @brief		16bit�̃X�e���I�f�[�^��ݒ�
//	//	@param[in]	bufferID	�o�b�t�@ID
//	//	@param[in]	pBuffer		�ȃf�[�^
//	//	@param[in]	bufferSize	�f�[�^�T�C�Y
//	//	@param[in]	frequency	���g��
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
//	//  @brief		���[�v�ݒ�
//	//	@param[in]	sourceID	�\�[�XID
//	//	@param[in]	bLoop		true:���[�v		false:�񃋁[�v
//	//
//	//------------------------------------------------------------------------------
//	static void SetLoopFlag(const unsigned int sourceID, const bool bLoop);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		���ʂ�ݒ�
//	//	@param[in]	sourceID	�\�[�XID
//	//	@param[in]	volume		����
//	//
//	//------------------------------------------------------------------------------
//	static void SetVolume(const unsigned int sourceID, const float volume);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		������ݒ�
//	//	@param[in]	sourceID	�\�[�XID
//	//	@param[in]	volume		����
//	//	@note		0.5f <= volume <= 2.0f
//	//
//	//------------------------------------------------------------------------------
//	static void SetPitch(const unsigned int sourceID, const float pitch);
//
//};
//#endif // !__AL_MANAGER_H_
