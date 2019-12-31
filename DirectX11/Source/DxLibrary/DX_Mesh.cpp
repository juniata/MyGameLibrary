//#include	"DX_Library.h"
//#include	"../ModelLoader/IMOLoader.h"
//using namespace DirectX;
//
////-----------------------------------------------------------------------------------------
////
////	�}�N����`
////
////-----------------------------------------------------------------------------------------
////	DWORD����VECTOR4�ɕϊ�
//#define VECTOR4_FROM_DWORD(dword) XMFLOAT4( (float((dword & 0xFF000000) >> 24) / 255.0f),(float((dword & 0x00FF0000) >> 16) / 255.0f),(float((dword & 0x0000FF00) >> 8) / 255.0f) ,(float((dword & 0x000000FF)) / 255.0f))
//
//#define TEXTURE_DECAL_RESOURCE_NUM		0
//#define TEXTURE_NORMAL_RESOURCE_NUM		1
//#define TEXTURE_SPECULAR_RESOURCE_NUM	2
//#define TEXTURE_HEIGHT_RESOURCE_NUM		3
//
////-----------------------------------------------------------------------------------------
////
////	�����o�[�ϐ��̏�����
////
////-----------------------------------------------------------------------------------------
//DX_Mesh::DX_Mesh() :
//
////	�O���[�v���b�V����������
//m_pGroupMesh(nullptr),
//
////	���f���̒��_���Ȃǂ�������
//m_indexCount(0),
//m_materialCount(0),
//m_vertexCount(0),
//m_faceCount(0),
//
////	���W����������
//m_pos(XMFLOAT3(0.0f,0.0f,0.0f)),
//m_scale(XMFLOAT3(1.0f,1.0f,1.0f)),
//m_angle(XMFLOAT3(0.0f,0.0f,0.0f)),
//
////	�ύX�t���O�ƃN���[���t���O�ƃ��C�s�b�N�t���O��������
//m_bChanged(true),
//m_bClone(false),
//m_bRayPick(false),
//
//m_pIndexBuffer(nullptr),
//m_pRayPickBuffer(nullptr),
//m_pVertexBuffer(nullptr)
//{
//	//	�s��X�V���s��
//	Update();
//}
//
////-----------------------------------------------------------------------------------------
////
////	�����o�ϐ������
////
////-----------------------------------------------------------------------------------------
//DX_Mesh::~DX_Mesh()
//{
//	//	���g���I���W�i���f�[�^�Ȃ�A����������s��
//	if (IsOriginal()){
//
//		//	�e�N�X�`�������
//		for (DWORD i = 0; i < m_materialCount; ++i){
//			DX_TextureManager::Release(m_pGroupMesh[i].pTextureDecal);
//			DX_TextureManager::Release(m_pGroupMesh[i].pTextureNormal);
//			DX_TextureManager::Release(m_pGroupMesh[i].pTextureSpecular);
//			DX_TextureManager::Release(m_pGroupMesh[i].pTextureHeight);
//		}
//
//		//	groupMesh�����
//		DELETE_OBJ_ARRAY(m_pGroupMesh);
//
//		SAFE_RELEASE(m_pIndexBuffer);
//		SAFE_RELEASE(m_pRayPickBuffer);
//		SAFE_RELEASE(m_pVertexBuffer);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	�����o�ϐ������������A���f����ǂݍ���
////
////-----------------------------------------------------------------------------------------
//DX_Mesh::DX_Mesh(const char* pFilepath) : DX_Mesh()
//{
//	//	���f����ǂݍ���
//	LoadModel(pFilepath);
//}
//
////-----------------------------------------------------------------------------------------
////
////	�I�u�W�F�N�g�𕡐�
////
////-----------------------------------------------------------------------------------------
//DX_Mesh* DX_Mesh::Clone()const
//{
//	//	�f�t�H���g�R�s�[�R���X�g���N�^�Ő�������
//	DX_Mesh*	l_pClone = new DX_Mesh(*this);
//
//	//	�N���[���t���O�𗧂Ă�
//	l_pClone->m_bClone = true;
//
//	return l_pClone;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	���W�������z����
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::Update()
//{
//	//	Set�֐��ō��W����ύX���ꂽ�ꍇ�̂݁A�s��v�Z���s��
//	if (m_bChanged){
//
//		//	�g��s�� * ��]�s�� * ���s�s��
//		XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
//		XMMATRIX angle = XMMatrixRotationRollPitchYaw(m_angle.x, m_angle.y, m_angle.z);
//		XMMATRIX translation = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
//		
//		XMStoreFloat4x4(&m_worldMatrix, angle * scale * translation);
//
//		m_bChanged = false;
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	���f����`��
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::Render()
//{
//	//	�f�o�C�X�R���e�L�X�g���擾
//	ID3D11DeviceContext*	l_pDeviceContext = DX_System::GetDeviceContext();
//
//	//	buffer stride and offset
//	unsigned int l_stride = sizeof(tagMeshVertex);
//	unsigned int l_offset = 0;
//
//	//	VertexBuffer�𑗂�
//	l_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &l_stride, &l_offset);
//
//	//	���C�s�b�N�t���O�������Ă���΁A���C�s�b�N�����̓������W�I���g���V�F�[�_�[���擾
//	DX_Shader* l_pGeometryShader =  ((m_bRayPick) ? DX_ShaderManager::GetInstance()->GetDefaultGeometryShaderRayPick() : DX_ShaderManager::GetInstance()->GetDefaultGeometryShader3D());
//
//	if (m_bRayPick){
//		unsigned int l_offset[] = { 0 };
//		l_pDeviceContext->SOSetTargets(1, &m_pRayPickBuffer, l_offset);
//	}
//	//	�`��
//	Render(
//		l_pDeviceContext,
//		DX_ShaderManager::GetInstance()->GetDefaultVertexShader3D(),
//		l_pGeometryShader,
//		DX_ShaderManager::GetInstance()->GetDefaultPixelShader3D(),
//		DX_ShaderManager::GetInstance()->GetDefaultInputLayout3D()
//		);
//
//	if (m_bRayPick){
//		unsigned int l_offset[] = { 0 };
//		ID3D11Buffer* l_pNullBuffer = nullptr;
//		l_pDeviceContext->SOSetTargets(1, &l_pNullBuffer, l_offset);
//
//	}
//	//	���C�s�b�N�����t���O��false�ɂ���
//	m_bRayPick = false;
//}
//
////-----------------------------------------------------------------------------------------
////
////	���f����ǂݍ���
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::LoadModel(const char* pFilepath)
//{
//	//	�f�o�C�X���擾����
//	ID3D11Device*	l_pDevice = DX_System::GetDevice();
//
//	//	IMOLoader�𐶐�����
//	IMOLoader l_imoLoader(pFilepath);
//
//	//	IMO��񂩂�e�ϐ��̍ő吔���擾
//	m_indexCount	= l_imoLoader.GetIndexCount();
//	m_materialCount = l_imoLoader.GetMaterialCount();
//	m_vertexCount	= l_imoLoader.GetVertexCount();
//	m_faceCount		= l_imoLoader.GetFaceCount();
//	
//	//	�T�C�Y����C�Ɋm�ۂ���
//	std::vector<tagMeshVertex>	l_vertexList(m_vertexCount);
//	std::vector<WORD>			l_indexList(m_indexCount);
//
//	//	�O���[�v�`��p�\���̂𐶐�
//	m_pGroupMesh = new tagGroupMesh[m_materialCount];
//
//	//	���_�f�[�^���R�s�[����
//	VertexCopy(&l_vertexList, l_imoLoader.GetVertices());
//
//	//	�C���f�b�N�X���}�e���A�����ɒ���
//	CreateIndexMaterialOrder(&l_indexList, l_imoLoader.GetIndices(), l_imoLoader.GetAttributes());
//	
//	//	���_�o�b�t�@���쐬
//	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(l_pDevice,sizeof(tagMeshVertex) * m_vertexCount, (tagMeshVertex*)&l_vertexList[0]);
//
//	//	�C���f�b�N�X�o�b�t�@
//	m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(l_pDevice, sizeof(WORD)* l_indexList.size(), (LPWORD)&l_indexList[0]);
//
//	//	���C�s�b�N�o�b�t�@���炭����
//	m_pRayPickBuffer = DX_Buffer::CreateStreamOutputBuffer(l_pDevice, (sizeof(XMFLOAT4)+sizeof(XMFLOAT3)) * m_faceCount * 3);
//
//	//	�}�e���A�������쐬
//	CreateTextureInfo(l_imoLoader.GetTextures());
//
//	//	���
//	l_vertexList.clear();
//	l_indexList.clear();
//
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	���C�s�b�N
////
////-----------------------------------------------------------------------------------------
//bool DX_Mesh::RayPick(
//	XMFLOAT3*		pHitPos,
//	XMFLOAT3*		pHitFaceNormal,
//	const XMFLOAT3& pos,
//	const XMFLOAT3& vec,
//	const float dist
//	)
//{
//	bool l_bHit = false;
//	struct tagRayPick{
//		XMFLOAT4 pos;
//		XMFLOAT3 vec;
//		float dist;
//	}l_rayPick;
//
//	l_rayPick.pos.x = pos.x;
//	l_rayPick.pos.y = pos.y;
//	l_rayPick.pos.z = pos.z;
//	l_rayPick.pos.w = 1.0f;
//	l_rayPick.vec	= vec;
//	l_rayPick.dist	= dist;
//
//	//	���[�J���ϐ�
//	ID3D11Buffer*				l_pBuffer			= nullptr;
//	D3D11_BUFFER_DESC			l_bufferDesc		= { NULL };
//	D3D11_MAPPED_SUBRESOURCE	l_subResource		= { NULL };
//	ID3D11DeviceContext*		l_pDeviceContext	= DX_System::GetDeviceContext();
//
//	//	�萔�o�b�t�@���쐬
//	l_bufferDesc.ByteWidth		= sizeof(tagRayPick);
//	l_bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
//	l_bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
//	l_bufferDesc.CPUAccessFlags = 0;
//
//	//	buffer���쐬
//	DX_System::GetDevice()->CreateBuffer(&l_bufferDesc, nullptr, &l_pBuffer);
//
//	//	updateSubResource
//	l_pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &l_rayPick, 0, 0);
//	
//	//	GS�ɑ���
//	DX_ResourceManager::SetConstantbuffers(l_pDeviceContext, 0, 1, &l_pBuffer, DX_SHADER_TYPE::GEOMETRY_SHADER);
//
//	//	�W�I���g������o�͂����郌�C�s�b�N���
//	struct tagRaypickOutput{
//		XMFLOAT4 pos;
//		XMFLOAT3 normal;
//	};
//
//	//	CPU�Œl��ǂ߂�o�b�t�@���쐬
//	ID3D11Buffer* l_pCpuReadBuffer = DX_Buffer::CPUReadBuffer(DX_System::GetDevice(), (sizeof(XMFLOAT4)+sizeof(XMFLOAT4)) * m_faceCount * 3);
//
//	//	�l�`�F�b�N
//	DEBUG_VALUE_CHECK((l_pCpuReadBuffer == nullptr) ? false : true, "DX_Buffer::CPUReadBuffer() : failed");
//
//	//	���C�s�b�N�p�o�b�t�@��CPUReadBuffer�ɃR�s�[
//	l_pDeviceContext->CopyResource(l_pCpuReadBuffer, m_pRayPickBuffer);
//
//	//	�l��ǂݍ���
//	D3D11_MAPPED_SUBRESOURCE l_map;
//	l_pDeviceContext->Map(l_pCpuReadBuffer, 0, D3D11_MAP::D3D11_MAP_READ, 0, &l_map);
//
//	tagRaypickOutput* l_pRayPick = (tagRaypickOutput*)l_map.pData;
//
//	for (DWORD i = 0; i < m_faceCount * 3; ++i){
//
//		//	w��1�̏ꍇ�́A�q�b�g�������ƂɂȂ�
//		if (l_pRayPick[i].pos.w == 1){
//
//			//	�q�b�g�����ʍ��W
//			pHitPos->x = l_pRayPick[i].pos.x;
//			pHitPos->y = l_pRayPick[i].pos.y;
//			pHitPos->z = l_pRayPick[i].pos.z;
//
//			//	�q�b�g�����ʖ@��
//			*pHitFaceNormal = l_pRayPick[i].normal;
//
//			//	�q�b�g�t���O��true�ɂ���
//			l_bHit = true;
//
//			//	for���𔲂���
//			break;
//		}
//
//	}
//
//	//	�ǂݍ��ݏI��
//	l_pDeviceContext->Unmap(l_pCpuReadBuffer, 0);
//
//	//	���C�s�b�N�����t���O��true�ɂ���
//	m_bRayPick = true;
//
//	SAFE_RELEASE(l_pBuffer);
//	SAFE_RELEASE(l_pCpuReadBuffer);
//
//	//	�q�b�g�t���O��Ԃ�
//	return l_bHit;
//}
////-----------------------------------------------------------------------------------------
////
////  ���f���̍��W��ݒ�
////
////------------------------------------------------------------------------------
//void DX_Mesh::SetPos(const XMFLOAT3& pos)
//{
//	m_pos		= pos;
//	m_bChanged	= true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  ���f���̌�����ݒ�
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::SetAngle(const XMFLOAT3& angle)
//{
//	m_angle		= angle;
//	m_bChanged	= true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  ���f���̑傫����ݒ�
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::SetScale(const XMFLOAT3& scale)
//{
//	m_scale		= scale;
//	m_bChanged	= true;
//
//}
//void DX_Mesh::SetScale(const float scale)
//{
//	m_scale.x = m_scale.y = m_scale.z = scale;
//	m_bChanged = true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  ���f���̍��W�A�����A�傫����ݒ�
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::SetTransform(const XMFLOAT3& pos, const XMFLOAT3& angle, const XMFLOAT3& scale)
//{
//	m_pos		= pos;
//	m_angle		= angle;
//	m_scale		= scale;
//	m_bChanged	= true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  ���f���̍��W���擾
////
////-----------------------------------------------------------------------------------------
//XMFLOAT3 DX_Mesh::GetPos()const
//{
//	return m_pos;
//}
//
////-----------------------------------------------------------------------------------------
////
////  ���f���̌������擾
////
////-----------------------------------------------------------------------------------------
//XMFLOAT3 DX_Mesh::GetAngle()const
//{
//	return m_angle;
//}
//
////-----------------------------------------------------------------------------------------
////
////  ���f���̑傫�����擾
////
////-----------------------------------------------------------------------------------------
//XMFLOAT3 DX_Mesh::GetScale()const
//{
//	return m_scale;
//}
//
////-----------------------------------------------------------------------------------------
////
////  ���f���̃��[���h�s����擾
////
////-----------------------------------------------------------------------------------------
//XMFLOAT4X4*	DX_Mesh::GetWorldMatrix()
//{
//	return &m_worldMatrix;
//}
//
////-----------------------------------------------------------------------------------------
////
////	���f�����I���W�i�����ǂ����𔻒�
////
////-----------------------------------------------------------------------------------------
//bool DX_Mesh::IsClone()const
//{
//	return m_bClone;
//}
//
////-----------------------------------------------------------------------------------------
////
////	���f�����I���W�i�����ǂ����𔻒�
////
////-----------------------------------------------------------------------------------------
//bool DX_Mesh::IsOriginal()const
//{
//	return !m_bClone;
//}
//
////-----------------------------------------------------------------------------------------
////
////	���_�f�[�^���R�s�[
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::VertexCopy(std::vector<tagMeshVertex>* pVertexList, void* pIMOVertex)
//{
//	LPIMOMESHVERTEX	l_pIMOVertex	= (LPIMOMESHVERTEX)pIMOVertex;
//	LPIMOMESHVERTEX l_pTempVertex	= nullptr;
//	tagMeshVertex*	l_pVertexLlist	= nullptr;
//
//	for (DWORD i = 0; i < m_vertexCount; ++i){
//
//		//	�A�h���X���擾
//		l_pTempVertex = &l_pIMOVertex[i];
//		l_pVertexLlist = &(*pVertexList)[i];
//
//		//	�f�[�^���R�s�[
//		l_pVertexLlist->pos		= l_pTempVertex->pos;
//		l_pVertexLlist->uv		= l_pTempVertex->uv;
//		l_pVertexLlist->normal	= l_pTempVertex->normal;
//		l_pVertexLlist->color	= VECTOR4_FROM_DWORD(l_pTempVertex->color);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	�}�e���A�����ɃC���f�b�N�X���쐬
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::CreateIndexMaterialOrder(std::vector<WORD>* pIndicesList,LPWORD pIndices, LPDWORD pAttributes)
//{
//	//	�Y���A�N�Z�X�p�ϐ�
//	int l_vectorIndexCount = 0;
//
//	//	startIndexLocation���J�E���g����ϐ�
//	int l_startIndexCount = 0;
//
//	//	�}�e���A�����ɒ��_��g�ݑւ���
//	for (DWORD m = 0; m < m_materialCount; ++m){
//
//		//	m_pGropuMesh��������
//		m_pGroupMesh[m].startIndexLocation	= 0;
//		m_pGroupMesh[m].indexCount			= 0;
//		m_pGroupMesh[m].baseVertexLocation	= 0;
//
//		//	�C���f�b�N�X�J�E���g
//		m_pGroupMesh[m].startIndexLocation = l_startIndexCount;
//
//		//	�������Ɩʐ��͓���
//		for (DWORD i = 0; i < m_faceCount; ++i){
//
//			//	����materiauNum�Ǝ擾�����}�e���A���ԍ����Ⴄ����΁Acontinue
//			if (m != pAttributes[i]){ continue; }
//
//			//	index == m_faceCount * 3
//			for (int j = 0; j < 3; ++j){
//				++l_startIndexCount;
//
//				//	���_�C���f�b�N�X��ǉ�
//				(*pIndicesList)[l_vectorIndexCount++] = pIndices[i * 3 + j];
//
//				//	�C���f�b�N�X���J�E���g����
//				m_pGroupMesh[m].indexCount++;
//			}
//		}
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	�e�N�X�`�������쐬����
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::CreateTextureInfo(char pTextures[32][MAX_PATH])
//{
//	char l_pTemp[32][MAX_PATH] = { NULL };
//
//	//	�}�e���A�����e�N�X�`����ǂݍ���
//	for (WORD i = 0; i < m_materialCount; ++i){
//
//		//	�ʏ�e�N�X�`��
//		m_pGroupMesh[i].pTextureDecal = DX_TextureManager::GetTexture(pTextures[i]);
//
//		//	�@���e�N�X�`��
//		sprintf_s(l_pTemp[i], MAX_PATH, "N%s", pTextures[i]);
//		m_pGroupMesh[i].pTextureNormal = DX_TextureManager::GetTexture(l_pTemp[i]);
//
//		//	�X�y�L�����e�N�X�`��
//		sprintf_s(l_pTemp[i], MAX_PATH, "S%s", pTextures[i]);
//		m_pGroupMesh[i].pTextureSpecular = DX_TextureManager::GetTexture(l_pTemp[i]);
//
//		//	�����e�N�X�`��
//		sprintf_s(l_pTemp[i], MAX_PATH, "H%s", pTextures[i]);
//		m_pGroupMesh[i].pTextureHeight = DX_TextureManager::GetTexture(l_pTemp[i]);
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	���f����`��(���ʂ̒��g)
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::Render(
//	ID3D11DeviceContext*	pDeviceContext,
//	DX_Shader*				pVertexShader,
//	DX_Shader*				pGeometryShader,
//	DX_Shader*				pPixelShader,
//	ID3D11InputLayout*		pInputLayout
//	)
//{
//
//	//	IndexBuffer�𑗂�
//	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//
//	//	InputLayout�̐ݒ�𑗂�
//	pDeviceContext->IASetInputLayout(pInputLayout);
//
//	//	Primitive�̐ݒ�𑗂�
//	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//	���[���h�s��𑗂�
//	DX_ShaderManager::GetInstance()->SetWorldMat(m_worldMatrix, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);
//
//	//	�r���[�Ɋւ���s��𑗂�
//	DX_View::SetMatrixForTheView();
//
//	//	�V�F�[�_�[���p���J�n
//	pVertexShader->Begin(pDeviceContext);
//	pGeometryShader->Begin(pDeviceContext);
//	pPixelShader->Begin(pDeviceContext);
//
//	//	�}�e���A���O���[�v�`��
//	tagGroupMesh* l_pGroupMesh = nullptr;
//
//	//	�e�N�X�`������Z�߂đ���
//	ID3D11ShaderResourceView*	l_pSRV[4];
//
//	//	�}�e���A�����ɕ`��
//	for (DWORD i = 0; i < m_materialCount; ++i){
//
//		//	�A�h���X���擾
//		l_pGroupMesh = &m_pGroupMesh[i];
//
//		//	�e�N�X�`����ݒ�
//		l_pSRV[TEXTURE_DECAL_RESOURCE_NUM]		= l_pGroupMesh->pTextureDecal;
//		l_pSRV[TEXTURE_NORMAL_RESOURCE_NUM]		= l_pGroupMesh->pTextureNormal;
//		l_pSRV[TEXTURE_SPECULAR_RESOURCE_NUM]	= l_pGroupMesh->pTextureSpecular;
//		l_pSRV[TEXTURE_HEIGHT_RESOURCE_NUM]		= l_pGroupMesh->pTextureHeight;
//
//		//	register(t0)�ɑ���
//		DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 4, l_pSRV,DX_SHADER_TYPE::PIXEL_SHADER);
//		
//		//	�C���f�b�N�X�`��
//		pDeviceContext->DrawIndexed(
//			l_pGroupMesh->indexCount,
//			l_pGroupMesh->startIndexLocation,
//			l_pGroupMesh->baseVertexLocation
//			);
//	}
//
//	//	�V�F�[�_�[���p���I��
//	pVertexShader->End(pDeviceContext);
//	pGeometryShader->End(pDeviceContext);
//	pPixelShader->End(pDeviceContext);
//
//
//}
