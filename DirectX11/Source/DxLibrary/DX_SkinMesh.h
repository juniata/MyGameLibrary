//#ifndef __DX_SKIN_MESH_H_
//#define __DX_SKIN_MESH_H_
//
////----------------------------------------------------------
////	���_���ێ�����skin���
////----------------------------------------------------------
////	���_���ێ����邓���������
//struct tagVertexSkinInfo
//{
//	unsigned int	boneNum;
//	float			weight[4];
//	tagVertexSkinInfo() : boneNum(0)
//	{
//		ZeroMemory(weight, sizeof(weight));
//	}
//};
//
//
////----------------------------------------------------------
////	IEM�p�̃A�j���[�V�������
////----------------------------------------------------------
//struct tagIEMAnimation
//{
//	unsigned long	rotCount;
//	unsigned short* pRotFrame;
//	DirectX::XMFLOAT4*		pRot;
//
//	unsigned long	posCount;
//	unsigned short* pPosFrame;
//	DirectX::XMFLOAT3* pPos;
//
//};
//
////----------------------------------------------------------
////	KM_SkinMesh�N���X�Ŏg��IEM�p�{�[�����
////----------------------------------------------------------
//struct tagIEMBoneSkinMesh
//{
//	WORD	parent;			//	�e
//	DirectX::XMFLOAT4X4	offsetMatrix;	//	����s��
//	DirectX::XMFLOAT4X4	boneMatrix;		//	�v�Z���ʂ��i�[����s��
//
//	DirectX::XMFLOAT4	orgPose;	//	�W���w��
//	DirectX::XMFLOAT3	orgPos;		//	�W���ʒu
//
//	DirectX::XMFLOAT4	curPose;
//	DirectX::XMFLOAT3	curPos;
//
//	float*			pInfluence;			//	�E�F�C�g�̉e����
//	unsigned long*	pWeightIndex;		//	�E�F�C�g�̃C���f�b�N�X
//	unsigned short  weightIndexCount;	//	�E�F�C�g�C���f�b�N�X�̐�
//
//};
//
//
//
////****************************************************************************************************
////
////	SkinMesh
////
////****************************************************************************************************
//class DX_SkinMesh : public DX_Mesh
//{
//public:
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�����o�ϐ���������
//	//
//	//------------------------------------------------------------------------------
//	DX_SkinMesh();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�����o�ϐ������������A���f����ǂݍ���
//	//	@param[in]	pFilepath	���f���̃t�@�C���p�X
//	//
//	//------------------------------------------------------------------------------
//	DX_SkinMesh(const char* pFilepath);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�S�f�[�^���
//	//
//	//------------------------------------------------------------------------------
//	~DX_SkinMesh();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�I�u�W�F�N�g�𕡐�
//	//	@retrun		�|�C���^���Ԃ�
//	//
//	//------------------------------------------------------------------------------
//	DX_SkinMesh* Clone()const;
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		���f����ǂݍ���
//	//	@param[in]	pFilepath	���f���̃t�@�C���p�X
//	//
//	//------------------------------------------------------------------------------
//	void LoadModel(const char* pFilepath);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		���W�⃁�b�V�����X�V
//	//
//	//------------------------------------------------------------------------------
//	void Update();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�X�L�����b�V����`��
//	//
//	//------------------------------------------------------------------------------
//	void Render();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		���f�����A�j���[�V����������
//	//
//	//------------------------------------------------------------------------------
//	void Animation();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		���[�V������؂�Ԃ�
//	//	@param		motinoNum	���[�V�����ԍ�	
//	//
//	//------------------------------------------------------------------------------
//	void SetMotion(const int motinoNum);
//
//private:
//	//	�X�L���o�b�t�@
//	ID3D11Buffer*	m_pSkinBuffer;
//
//	tagIEMBoneSkinMesh*	m_pBoneInfo;		//	�{�[�����
//	tagIEMAnimation*	m_pAnimationInfo;	//	�A�j���[�V�������
//
//
//	DirectX::XMFLOAT4X4*	m_pBoneMatrix;	//	�p���b�g�s��
//
//	int m_boneCount;	//	�{�[����
//
//	unsigned char	m_motion;		//	���݂̃��[�V�����ԍ�
//	unsigned short	m_frameCount;	//	�t���[����
//	unsigned short* m_pFrameFlag;	//	�t���[�����
//	unsigned short	m_offset[256];		//	���[�V�����擪�t���[��
//	unsigned char	m_param[16];
//
//	int m_dwFrame;
//	int m_frame;
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�t���[���ɂ���Ďp�����X�V����
//	//	@param[in]	frame	�t���[��
//	//
//	//------------------------------------------------------------------------------
//	void UpdateSkinMeshFrame(const float frame);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�t���[���ɂ����m_pBoneInfo->curPose���X�V����
//	//	@param[in]	frame	�t���[��
//	//
//	//------------------------------------------------------------------------------
//	void UpdateBonePoseByFrame(const float frame, tagIEMBoneSkinMesh* pIEMBoneSkinMesh, tagIEMAnimation* pIEMAnimation);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�t���[���ɂ����m_pBoneInfo->curPos���X�V����
//	//	@param[in]	frame	�t���[��
//	//
//	//------------------------------------------------------------------------------
//	void UpdateBonePosByFrame(const float frame, tagIEMBoneSkinMesh* pIEMBoneSkinMesh, tagIEMAnimation* pIEMAnimation);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		���݂̃t���[���ƊJ�n�A�I�����t���[������o�߃t���[�����Z�o����
//	//	@param[in]	frame		m_dwFrame
//	//	@param[in]	curFrame	m_pAnimationInfo->pRotFrame[i]
//	//	@param[in]	nextFrame	m_pAnimationInfo->pRotFrame[i + 1]
//	//	@retrun		��Ԃ��ꂽ�t���[��
//	//
//	//------------------------------------------------------------------------------
//	float GetElapsedFrame(const float frame, const WORD curFrame, const WORD argNextFrame);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		���݂̃t���[�����A���݂̃��[�V�����Ǝ��̃��[�V�����̊Ԃɂ��邩�ǂ����B
//	//	@param[in]	frame		m_dwFrame
//	//	@param[in]	curFrame	m_pAnimationInfo->pRotFrame[i]
//	//	@param[in]	nextFrame	m_pAnimationInfo->pRotFrame[i + 1]
//	//	@retrun		ture:����	false�F�Ȃ�
//	//
//	//------------------------------------------------------------------------------
//	bool IsCurrentFrameIsInTheMiddle(const float frame, const WORD argCurFrame, const WORD nextFrame);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		�{�[���̍s����X�V����
//	//
//	//------------------------------------------------------------------------------
//	void UpdateBoneMatrix();
//
//};
//#endif // !__DX_SKIN_MESH_H_
