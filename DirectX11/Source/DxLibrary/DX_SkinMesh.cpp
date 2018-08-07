//#include	"DX_Library.h"
//#include	"../ModelLoader/IEMLoader.h"
//using namespace DirectX;
//
////-----------------------------------------------------------------------------------------
////
////	�����o�ϐ���������
////
////-----------------------------------------------------------------------------------------
//DX_SkinMesh::DX_SkinMesh() :
//
////	�{�[������������
//m_pBoneInfo(nullptr),
//m_pAnimationInfo(nullptr),
//m_boneCount(0),
//
////	�p���b�g�s���������
//m_pBoneMatrix(nullptr),
//
//m_motion(0),
//m_frameCount(0),
//m_pFrameFlag(nullptr),
//m_dwFrame(0),
//m_frame(0),
//
//m_pSkinBuffer(nullptr)
//{
//	ZeroMemory(m_param, sizeof(m_param));
//	ZeroMemory(m_offset, sizeof(m_offset));
//
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	�����o�ϐ������������A���f����ǂݍ���
////
////-----------------------------------------------------------------------------------------
//DX_SkinMesh::DX_SkinMesh(const char* pFilepath) : DX_SkinMesh()
//{
//	//	���f����ǂݍ���
//	LoadModel(pFilepath);
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	�S�f�[�^���
////
////-----------------------------------------------------------------------------------------
//DX_SkinMesh::~DX_SkinMesh()
//{
//	//	���g���I���W�i���Ȃ�A�f�[�^���������
//	if (IsOriginal()){
//
//		for (int i = 0; i < m_boneCount; ++i){
//
//			//	�{�[���f�[�^�����
//			DELETE_OBJ_ARRAY(m_pBoneInfo[i].pInfluence);
//			DELETE_OBJ_ARRAY(m_pBoneInfo[i].pWeightIndex);
//
//			//	�A�j���[�V�����f�[�^�����
//			DELETE_OBJ_ARRAY(m_pAnimationInfo[i].pRotFrame);
//			DELETE_OBJ_ARRAY(m_pAnimationInfo[i].pRot);
//			DELETE_OBJ_ARRAY(m_pAnimationInfo[i].pPosFrame);
//			DELETE_OBJ_ARRAY(m_pAnimationInfo[i].pPos);
//		}
//
//		//	�{�[���f�[�^�����
//		DELETE_OBJ_ARRAY(m_pBoneInfo);
//
//		//	�A�j���[�V�����f�[�^�����
//		DELETE_OBJ_ARRAY(m_pAnimationInfo);
//
//		SAFE_RELEASE(m_pSkinBuffer);
//	}
//	else{
//		//	�p���b�g�s�񂾂��͕ʂȂ̂ŁA�폜���Ă���
//		DELETE_OBJ_ARRAY(m_pBoneMatrix);
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	�I�u�W�F�N�g�𕡐�����
////
////-----------------------------------------------------------------------------------------
//DX_SkinMesh* DX_SkinMesh::Clone()const
//{
//	DX_SkinMesh* l_pSkinMesh = new DX_SkinMesh(*this);
//	l_pSkinMesh->m_bClone = true;
//	
//	l_pSkinMesh->m_pBoneMatrix = new DirectX::XMFLOAT4X4[m_boneCount];
//	CopyMemory(l_pSkinMesh->m_pBoneMatrix, m_pBoneMatrix,sizeof(DirectX::XMFLOAT4X4)* m_boneCount);
//	return l_pSkinMesh;
//}
//
////-----------------------------------------------------------------------------------------
////
////	���f����ǂݍ���
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::LoadModel(const char* pFilepath)
//{
//	//	�f�o�C�X���擾����
//	auto l_pDevice = DX_System::GetInstance()->GetDevice();
//
//	//	IEMLoader�𐶐�
//	IEMLoader l_iemLoader(pFilepath);
//
//	//	IEM��񂩂�e�ϐ��̍ő吔���擾
//	m_indexCount	= l_iemLoader.GetIndexCount();
//	m_materialCount = l_iemLoader.GetMaterialCount();
//	m_vertexCount	= l_iemLoader.GetVertexCount();
//	m_faceCount		= l_iemLoader.GetFaceCount();
//	m_frameCount = l_iemLoader.GetFrameCount();
//	m_boneCount = l_iemLoader.GetBoneCount();
//
//	//	�T�C�Y����C�Ɋm�ۂ���
//	std::vector<tagMeshVertex>	l_vertexList(m_vertexCount);
//	std::vector<WORD>			l_indexList(m_indexCount);
//
//	//	�O���[�v�`��p�\���̂𐶐�
//	m_pGroupMesh = new tagGroupMesh[m_materialCount];
//
//	//	���_�f�[�^���R�s�[����
//	CopyMemory(&l_vertexList[0], l_iemLoader.GetVertices(), sizeof(tagIEMMeshVertex)* m_vertexCount);
//
//	//	�C���f�b�N�X���}�e���A�����ɒ���
//	CreateIndexMaterialOrder(&l_indexList, l_iemLoader.GetIndices(), l_iemLoader.GetAttributes());
//
//	//	���_�o�b�t�@���쐬
//	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(l_pDevice, sizeof(tagMeshVertex)* m_vertexCount, (tagMeshVertex*)&l_vertexList[0]);
//
//	//	�C���f�b�N�X�o�b�t�@
//	m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(l_pDevice, sizeof(WORD)* l_indexList.size(), (LPWORD)&l_indexList[0]);
//
//	//	�}�e���A�������쐬
//	CreateTextureInfo(l_iemLoader.GetTextures());
//
//	//	���
//	l_vertexList.clear();
//	l_indexList.clear();
//
//	//	�{�[�����A�A�j���[�V�������A���_���W���e�X������
//	m_pBoneInfo = new tagIEMBoneSkinMesh[m_boneCount];
//	m_pAnimationInfo = new tagIEMAnimation[m_boneCount];
//	ZeroMemory(m_pAnimationInfo, sizeof(tagIEMAnimation)* m_boneCount);
//
//	//	�A�j���[�V�������
//	m_pFrameFlag = new WORD[m_frameCount];
//	CopyMemory(m_offset, l_iemLoader.GetOffSet(), sizeof(m_offset));
//	CopyMemory(m_pFrameFlag, l_iemLoader.GetFrameFlag(), sizeof(WORD)* m_frameCount);
//
//	tagIEMBoneSkinMesh* l_pIEMBoneSkineMeshInfo = nullptr;
//	tagIEMAnimation*	l_pIEMAnimationInfo = nullptr;
//	tagIEMBone*			l_pTempIEMBoneInfoData = nullptr;
//	tagIEMMotion*		l_pTempIEMMotionInfo = nullptr;
//
//	for (int i = 0; i < m_boneCount; ++i){
//
//		//	�A�h���X���擾
//		l_pIEMBoneSkineMeshInfo = &m_pBoneInfo[i];
//		l_pIEMAnimationInfo = &m_pAnimationInfo[i];
//		l_pTempIEMBoneInfoData = &l_iemLoader.GetBoneInfo()[i];
//		l_pTempIEMMotionInfo = &l_iemLoader.GetMotionInfo()[i];
//
//		//	�f�[�^���R�s�[�A�y�ѐ���
//		l_pIEMBoneSkineMeshInfo->parent = l_pTempIEMBoneInfoData->parent;			//	�e
//		l_pIEMBoneSkineMeshInfo->offsetMatrix = l_pTempIEMBoneInfoData->boneMatrix;		//	����s��
//		l_pIEMBoneSkineMeshInfo->orgPos = l_pTempIEMBoneInfoData->orgPos;			//	�W���ʒu
//		l_pIEMBoneSkineMeshInfo->orgPose = l_pTempIEMBoneInfoData->orgPose;			//	�W���w��
//		l_pIEMBoneSkineMeshInfo->weightIndexCount = l_pTempIEMBoneInfoData->indexCount;		//	�E�F�C�g�C���f�b�N�X�̐�
//		l_pIEMBoneSkineMeshInfo->pWeightIndex = new DWORD[l_pIEMBoneSkineMeshInfo->weightIndexCount];	// �E�F�C�g�̃C���f�b�N�X
//		l_pIEMBoneSkineMeshInfo->pInfluence = new float[l_pIEMBoneSkineMeshInfo->weightIndexCount];	//	�E�F�C�g�̉e����
//
//		//	�f�[�^���R�s�[
//		CopyMemory(l_pIEMBoneSkineMeshInfo->pWeightIndex, l_pTempIEMBoneInfoData->pIndices, sizeof(DWORD)*	l_pIEMBoneSkineMeshInfo->weightIndexCount);
//		CopyMemory(l_pIEMBoneSkineMeshInfo->pInfluence, l_pTempIEMBoneInfoData->pInfluence, sizeof(float)*	l_pIEMBoneSkineMeshInfo->weightIndexCount);
//
//		//	�A�j���[�V�������
//		l_pIEMAnimationInfo->rotCount = l_pTempIEMMotionInfo->rotateCount;
//		l_pIEMAnimationInfo->pRotFrame = new WORD[l_pIEMAnimationInfo->rotCount];
//		l_pIEMAnimationInfo->pRot = new DirectX::XMFLOAT4[l_pIEMAnimationInfo->rotCount];
//
//		//	�N�H�[�^�j�I���R�s�[
//		for (DWORD j = 0; j < l_pIEMAnimationInfo->rotCount; ++j){
//			l_pIEMAnimationInfo->pRotFrame[j] = l_pTempIEMMotionInfo->pRotateFrame[j];
//			l_pIEMAnimationInfo->pRot[j] = l_pTempIEMMotionInfo->pRotate[j];
//		}
//
//		//	�|�W�V�����R�s�[
//		l_pIEMAnimationInfo->posCount = l_pTempIEMMotionInfo->posCount;
//
//		if (l_pIEMAnimationInfo->posCount > 0){
//			l_pIEMAnimationInfo->pPosFrame = new WORD[l_pIEMAnimationInfo->posCount];
//			l_pIEMAnimationInfo->pPos = new DirectX::XMFLOAT3[l_pIEMAnimationInfo->posCount];
//			CopyMemory(l_pIEMAnimationInfo->pPosFrame, l_pTempIEMMotionInfo->pPosFrame, sizeof(WORD)*		l_pIEMAnimationInfo->posCount);
//			CopyMemory(l_pIEMAnimationInfo->pPos, l_pTempIEMMotionInfo->pPos, sizeof(DirectX::XMFLOAT3)*	l_pIEMAnimationInfo->posCount);
//		}
//	}
//
//	//	�{�[���̃p���b�g�s��
//	m_pBoneMatrix = new DirectX::XMFLOAT4X4[m_boneCount];
//	ZeroMemory(m_pBoneMatrix, sizeof(DirectX::XMFLOAT4X4)* m_boneCount);
//
//	//	���_���m��
//	std::vector<tagVertexSkinInfo> l_vertexSkinList(m_vertexCount);
//
//	//	��̃|���S���ɉe������{�[���͂S��
//	//	index == uint4   weight == float4
//	//	1�̒��_�ɃC���f�b�N�X�ƃE�F�C�g���S����
//	tagIEMBoneSkinMesh* l_pTempSkinMesh = nullptr;
//	tagVertexSkinInfo*	l_pVertexSkinInfo = nullptr;
//	std::vector<int>	l_weightIndexCount(m_vertexCount, 0);
//
//	DWORD l_weinghtIndex = 0;
//
//	//	bone��������
//	for (int bone = 0; bone < m_boneCount; ++bone){
//
//		//	�A�h���X���擾
//		l_pTempSkinMesh = &m_pBoneInfo[bone];
//
//		//	�E�F�C�g�J�E���g����
//		for (int i = 0; i < l_pTempSkinMesh->weightIndexCount; ++i){
//
//			//	�e����^���钸�_�̃C���f�b�N�X���擾
//			l_weinghtIndex = l_pTempSkinMesh->pWeightIndex[i];
//
//			//	�E�F�C�g�C���f�b�N�X�ŃA�N�Z�X
//			l_pVertexSkinInfo = &l_vertexSkinList[l_weinghtIndex];
//
//			//	���̒��_���e�����󂯂�{�[���̔ԍ���ێ�
//			//	�r�b�g���Z��uint4 ~ uint1�Ƀp�b�N
//			l_pVertexSkinInfo->boneNum |= (bone << (l_weightIndexCount[l_weinghtIndex] * 8));
//
//			//	���̒��_��bone����󂯂�e���͂�ݒ�
//			l_pVertexSkinInfo->weight[l_weightIndexCount[l_weinghtIndex]] = l_pTempSkinMesh->pInfluence[i];
//
//			//	�C���f�b�N�X���J�E���g����
//			l_weightIndexCount[l_weinghtIndex] += 1;
//		}
//	}
//
//	//	�X�L���o�b�t�@���쐬����
//	m_pSkinBuffer = DX_Buffer::CreateSkinBuffer(l_pDevice, sizeof(tagVertexSkinInfo)* m_vertexCount, (tagVertexSkinInfo*)&l_vertexSkinList[0]);
//
//	//	�X�L�������폜
//	l_vertexSkinList.clear();
//	l_weightIndexCount.clear();
//}
////-----------------------------------------------------------------------------------------
////
////	���W�⃁�b�V�����X�V
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::Update()
//{
//	//	�X�L�����b�V���X�V
//	if (m_frame != m_dwFrame){
//	
//		//	�t���[���ɂ���Ďp�����X�V����
//		UpdateSkinMeshFrame(CAST_F(m_dwFrame));
//
//		//	�p���b�g�s����쐬
//		UpdateBoneMatrix();
//	}
//
//	DX_Mesh::Update();
//
//	m_frame = m_dwFrame;
//
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	�X�L�����b�V����`��
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::Render()
//{
//	//	�f�o�C�X�R���e�L�X�g���擾
//	auto l_pDeviceContext = DX_System::GetInstance()->GetDeviceContext();
//
//	//	stride
//	unsigned int l_stride[] = {
//		sizeof(tagMeshVertex),
//		sizeof(tagVertexSkinInfo)
//	};
//
//	//	offset
//	unsigned int l_offset = 0;
//
//	//	IA��verteBuffer��ݒ�
//	l_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &l_stride[0], &l_offset);
//
//	//	IA��SkinBuffer��ݒ�
//	l_pDeviceContext->IASetVertexBuffers(1, 1, &m_pSkinBuffer, &l_stride[1], &l_offset);
//
//	//	�s�D�悩���D��ɕύX(float4x4 �� flaot3x4)
//	XMMATRIX boneMat;
//	for (int i = 0; i < m_boneCount; ++i){
//		boneMat = XMLoadFloat4x4(&m_pBoneMatrix[i]);
//		XMStoreFloat4x4(&m_pBoneMatrix[i], XMMatrixTranspose(boneMat));
//	}
//
//	//	Bone�s��𒸓_�V�F�[�_�[��register(t1)�ɑ���
//	DX_ShaderManager::GetInstance()->SetMatrixResoruce(1, m_pBoneMatrix, m_boneCount, l_pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);
//
//	//	���C�s�b�N�t���O�������Ă���΁A���C�s�b�N�����̓������W�I���g���V�F�[�_�[���擾
//	DX_Shader* l_pGeometryShader = ((m_bRayPick) ? DX_ShaderManager::GetInstance()->GetDefaultGeometryShaderRayPick() : DX_ShaderManager::GetInstance()->GetDefaultGeometryShader3D());
//
//	//	�`��
//	DX_Mesh::Render(
//		l_pDeviceContext,
//		DX_ShaderManager::GetInstance()->GetDefaultVertexShaderSkinMesh(),
//		l_pGeometryShader,
//		DX_ShaderManager::GetInstance()->GetDefaultPixelShader3D(),
//		DX_ShaderManager::GetInstance()->GetDefaultInputLayoutSkinMesh()
//		);
//	
//	//	���C�s�b�N�����t���O��false�ɂ���
//	m_bRayPick = false;
//}
////-----------------------------------------------------------------------------------------
////
////	�A�j���[�V�����t���[�����X�V
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::Animation()
//{
//	int l_param = m_pFrameFlag[m_dwFrame];
//	int l_work = m_dwFrame;
//
//	if (l_param & 0x4000) { l_param = 0xFFFF; }
//	if (l_param != 0xFFFF){
//
//		//	�A�j���[�V�����W�����v
//		if (l_param & 0x8000){
//			SetMotion(l_param & 0xFF);
//		}
//		else {
//			m_dwFrame = l_param;
//		}
//	}
//	else{
//		m_dwFrame++;
//		if (m_dwFrame >= m_frameCount){ m_dwFrame = 0; }
//	}
//
//	if (m_dwFrame != l_work){ m_bChanged = true; }
//
//	l_param = m_pFrameFlag[m_dwFrame];
//	if ((l_param != 0xFFFF) && (l_param & 0x4000)){
//		m_param[(l_param & 0x0F00) >> 8] = (unsigned char)(l_param & 0x00FF);
//	}
//
//}
//
////-----------------------------------------------------------------------------------------
////
////	���[�V������ݒ肷��
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::SetMotion(const int motinoNum)
//{
//	int		l_param;
//
//	if (m_offset[motinoNum] == 65535){ return; }
//	m_motion 	= motinoNum;
//	m_dwFrame 	= m_offset[motinoNum];
//	m_bChanged 	= true;
//
//	l_param = m_pFrameFlag[m_dwFrame];
//	if ((l_param != 0xFFFF) && (l_param & 0x4000)) {
//		m_param[(l_param & 0x0F00) >> 8] = (unsigned char)(l_param & 0x00FF);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	�t���[���ɂ���Ďp�����X�V����
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::UpdateSkinMeshFrame(const float frame)
//{
//	tagIEMAnimation*	l_pAnimation 	= nullptr;
//	tagIEMBoneSkinMesh*	l_pIEMBoneInfo 	= nullptr;
//
//	for (int i = 0; i < m_boneCount; ++i){
//
//		//	�A�h���X�擾
//		l_pAnimation	= &m_pAnimationInfo[i];
//		l_pIEMBoneInfo	= &m_pBoneInfo[i];
//
//		//	�|�[�Y�ݒ�
//		UpdateBonePoseByFrame(frame, l_pIEMBoneInfo, l_pAnimation);
//
//		//	���W�ݒ�
//		UpdateBonePosByFrame(frame, l_pIEMBoneInfo, l_pAnimation);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	�t���[���ɂ����m_pBoneInfo->curPose���X�V����
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::UpdateBonePoseByFrame(const float argFrame, tagIEMBoneSkinMesh* pIEMBoneSkinMesh, tagIEMAnimation* pIEMAnimation)
//{
//	float l_t;
//	DWORD j;
//
//	switch (pIEMAnimation->rotCount){
//
//		//	�t���[�����O�̏ꍇ�͏����p����ݒ肷��
//	case 0:	pIEMBoneSkinMesh->curPose = pIEMBoneSkinMesh->orgPose;		break;
//
//		//	�t���[�����P�̏ꍇ�́A�A�j���[�V�����P�t���[���ڂ̎p����ݒ肷��
//	case 1:	pIEMBoneSkinMesh->curPose = pIEMAnimation->pRot[0];			break;
//
//		//	����ȊO�̏ꍇ
//	default:
//	{
//		XMVECTOR q1;
//		XMVECTOR q2;
//		//	��]�L�[���
//		for (j = 0; j < pIEMAnimation->rotCount - 1; ++j) {
//
//			//	���݈ʒu����
//			if (IsCurrentFrameIsInTheMiddle(argFrame, pIEMAnimation->pRotFrame[j], pIEMAnimation->pRotFrame[j + 1])) {
//
//				//	�o�߃t���[�����擾
//				l_t = GetElapsedFrame(argFrame, pIEMAnimation->pRotFrame[j], pIEMAnimation->pRotFrame[j + 1]);
//
//				//	���
//				q1 = XMLoadFloat4(&pIEMAnimation->pRot[j]);
//				q2 = XMLoadFloat4(&pIEMAnimation->pRot[j + 1]);
//				XMStoreFloat4(&pIEMBoneSkinMesh->curPose, XMQuaternionSlerp(q1, q2, l_t));
//				break;
//			}
//		}
//
//		if (j == pIEMAnimation->rotCount - 1) {
//			pIEMBoneSkinMesh->curPose = pIEMAnimation->pRot[pIEMAnimation->rotCount - 1];
//		}
//		break;
//	}
//	} // switch (pIEMAnimation->rotCount){
//}
//
////-----------------------------------------------------------------------------------------
////
////	�t���[���ɂ����m_pBoneInfo->curPos���X�V����
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::UpdateBonePosByFrame(const float argFrame, tagIEMBoneSkinMesh* pIEMBoneSkinMesh, tagIEMAnimation* pIEMAnimation)
//{
//	float l_t;
//	DWORD j;
//
//	if (pIEMAnimation->posCount == 0){
//		pIEMBoneSkinMesh->curPos = pIEMBoneSkinMesh->orgPos;
//	}
//	else{
//
//		XMVECTOR currPos;
//		XMVECTOR nextPos;
//
//		//	�ʒu���
//		for (j = 0; j < pIEMAnimation->posCount - 1; ++j){
//
//			//	���݈ʒu����
//			if (IsCurrentFrameIsInTheMiddle(argFrame, pIEMAnimation->pPosFrame[j], pIEMAnimation->pPosFrame[j + 1])){
//
//				//	�o�߃t���[�����擾
//				l_t = GetElapsedFrame(argFrame, pIEMAnimation->pPosFrame[j], pIEMAnimation->pPosFrame[j + 1]);
//
//				//	���
//				currPos = XMLoadFloat3(&pIEMAnimation->pPos[j]);
//				nextPos = XMLoadFloat3(&pIEMAnimation->pPos[j + 1]);
//
//				XMStoreFloat3(&pIEMBoneSkinMesh->curPos, currPos + (nextPos - currPos) * l_t);
//				//pIEMBoneSkinMesh->curPos = pIEMAnimation->pPos[j] + (pIEMAnimation->pPos[j + 1] - pIEMAnimation->pPos[j]) *l_t;
//				break;
//			}
//		}
//
//
//		if (j == pIEMAnimation->posCount - 1){
//			pIEMBoneSkinMesh->curPos = pIEMAnimation->pPos[pIEMAnimation->posCount - 1];
//		}
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	���݂̃t���[���ƊJ�n�A�I�����t���[������o�߃t���[�����Z�o����
////
////-----------------------------------------------------------------------------------------
//float DX_SkinMesh::GetElapsedFrame(const float argFrame, const WORD argCurFrame, const WORD argNextFrame)
//{
//	//	(���݃t���[�� - �J�n�t���[��) / (�I���t���[�� - �J�n�t���[��)
//	return (CAST_F(argFrame - argCurFrame) / CAST_F(argNextFrame - argCurFrame));
//}
//
////-----------------------------------------------------------------------------------------
////
////	���݂̃t���[�����A���݂̃��[�V�����Ǝ��̃��[�V�����̊Ԃɂ��邩�ǂ����B�����true
////
////-----------------------------------------------------------------------------------------
//bool DX_SkinMesh::IsCurrentFrameIsInTheMiddle(const float argFrame, const WORD argCurFrame, const WORD argNextFrame)
//{
//	return ((argFrame >= argCurFrame) && (argFrame < argNextFrame));
//}
//
////-----------------------------------------------------------------------------------------
////
////	�{�[���s����X�V
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::UpdateBoneMatrix()
//{
//	tagIEMBoneSkinMesh*	l_pIEMBoneInfo = nullptr;
//	
//	XMMATRIX boneMat;
//	XMMATRIX parentBoneMat;
//	XMMATRIX offfsetMat;
//	XMVECTOR curPose;
//	XMVECTOR curPos;
//	for (int i = 0; i < m_boneCount; ++i){
//
//		//	�A�h���X���擾
//		l_pIEMBoneInfo = &m_pBoneInfo[i];
//
//		curPose = XMLoadFloat4(&l_pIEMBoneInfo->curPose);
//		curPos  = XMLoadFloat3(&l_pIEMBoneInfo->curPos);
//		boneMat = XMLoadFloat4x4(&l_pIEMBoneInfo->boneMatrix);
//
//		//	�N�H�[�^�j�I�������]�s����擾
//		boneMat = XMMatrixRotationQuaternion(curPose);
//		boneMat.r[3] = curPos;
//
//		//D3DXMatrixRotationQuaternion(
//		//	&l_pIEMBoneInfo->boneMatrix,
//		//	&l_pIEMBoneInfo->curPose
//		//	);
//		////	�ʒu���R�s�[
//		//l_pIEMBoneInfo->boneMatrix._41 = l_pIEMBoneInfo->curPos.x;
//		//l_pIEMBoneInfo->boneMatrix._42 = l_pIEMBoneInfo->curPos.y;
//		//l_pIEMBoneInfo->boneMatrix._43 = l_pIEMBoneInfo->curPos.z;
//
//		//	���g�̃{�[���ɐe������΁A�e�̍s���������
//		if (l_pIEMBoneInfo->parent != 0xFFFF){
//			parentBoneMat = XMLoadFloat4x4(&m_pBoneInfo[l_pIEMBoneInfo->parent].boneMatrix);
//			boneMat *= parentBoneMat;
//			//l_pIEMBoneInfo->boneMatrix *= m_pBoneInfo[l_pIEMBoneInfo->parent].boneMatrix;
//		}
//
//		//	�A�j���[�V�����s��  = ��{���s�� * bone�s��
//		offfsetMat = XMLoadFloat4x4(&l_pIEMBoneInfo->offsetMatrix);
//		XMStoreFloat4x4(&m_pBoneMatrix[i], offfsetMat * boneMat);
//		//m_pBoneMatrix[i] = l_pIEMBoneInfo->offsetMatrix * l_pIEMBoneInfo->boneMatrix;
//	}
//}
