#ifndef __DX_MESH_H_
#define __DX_MESH_H_

//--------------------------------------------------
//	�}�e���A�����`��p���
//--------------------------------------------------
struct tagGroupMesh
{
	//	�`�悷��C���f�b�N�X��
	unsigned int indexCount;

	//	�C���f�b�N�X�o�b�t�@����GUP�œǂݎ��ŏ��̃C���f�b�N�X�̏ꏊ
	unsigned int startIndexLocation;

	//	���_�o�b�t�@�[���璸�_��ǂݎ��O�ɁA�e�C���f�b�N�X�ɉ��Z����l�ł�
	int baseVertexLocation;

	//	�e�N�X�`��
	ID3D11ShaderResourceView*	pTextureDecal;			//	�ʏ�e�N�X�`��
	ID3D11ShaderResourceView*	pTextureNormal;			//	�@���e�N�X�`��
	ID3D11ShaderResourceView*	pTextureSpecular;		//	�X�y�L�����e�N�X�`��
	ID3D11ShaderResourceView*	pTextureHeight;			//	�����e�N�X�`��
};

//--------------------------------------------------
//	Mesh�p���_���
//--------------------------------------------------
struct tagMeshVertex{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
};

//--------------------------------------------------
//	�ʏ��
//--------------------------------------------------
struct tagFase{
	tagMeshVertex	vertex[3];		//	���_���
	DirectX::XMFLOAT3			normal;			//	�ʖ@��
	DWORD			materialNum;	//	�}�e���A���ԍ�
	WORD			index[3];		//	�C���f�b�N�X�ԍ�
};

//****************************************************************************************************
//
//	Mesh
//
//****************************************************************************************************
class Mesh : public dx_library::GameObject
{
protected:
	ComPtr<ID3D11Buffer>	m_vertexBuffer;	//	���_�o�b�t�@
	ComPtr<ID3D11Buffer>	m_indexBuffer;		//	�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11Buffer>	m_rayPickBuffer;	//	���C�s�b�N�p�o�b�t�@

	//	�}�e���A�����`��p�\���̃|�C���^�ϐ�
	dx_library::tagGroupMesh*	m_pGroupMesh;

	DWORD  m_indexCount;
	DWORD  m_materialCount;
	DWORD  m_vertexCount;
	DWORD  m_faceCount;

	DirectX::XMFLOAT4X4  m_worldMatrix;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_angle;

	bool m_bChanged;
	bool m_bClone;
	bool m_bRayPick;

	//	�R�s�[�R���X���N�^�Ƒ�����֎~�ɂ���
//	dx_library::Mesh& operator=(const dx_library::Mesh& copy);
//	dx_library::Mesh(const dx_library::Mesh& copy);
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�[�ϐ��̏�����
	//
	//------------------------------------------------------------------------------
	Mesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ������
	//
	//------------------------------------------------------------------------------
	virtual ~Mesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ������������A���f����ǂݍ���
	//	@param[in]	pFilepath	���f���̃t�@�C���p�X
	//
	//------------------------------------------------------------------------------
	Mesh(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		�I�u�W�F�N�g�𕡐�
	//	@retrun		�|�C���^���Ԃ�
	//
	//------------------------------------------------------------------------------
	Mesh* Clone()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		���W���̍X�V���s��
	//
	//------------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------------
	//
	//  @brief		���f����`��
	//
	//------------------------------------------------------------------------------
	virtual void Render();

	//------------------------------------------------------------------------------
	//
	//  @brief		���f���̍��W��ݒ�
	//	@param[in]	pos	���W
	//
	//------------------------------------------------------------------------------
	void SetPos(const DirectX::XMFLOAT3& pos);

	//------------------------------------------------------------------------------
	//
	//  @brief		���f���̌�����ݒ�
	//	@param[in]	angle	����
	//
	//------------------------------------------------------------------------------
	void SetAngle(const DirectX::XMFLOAT3& angle);

	//------------------------------------------------------------------------------
	//
	//  @brief		���f���̑傫����ݒ�
	//	@param[in]	scale	�傫��
	//
	//------------------------------------------------------------------------------
	void SetScale(const DirectX::XMFLOAT3& scale);
	void SetScale(const float scale);

	//------------------------------------------------------------------------------
	//
	//  @brief		���f���̍��W�A�����A�傫����ݒ�
	//	@param[in]	pos		���W
	//	@param[in]	angle	����
	//	@param[in]	scale	�傫��
	//
	//------------------------------------------------------------------------------
	void SetTransform(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& scale);

	//------------------------------------------------------------------------------
	//
	//  @brief		���f���̍��W���擾
	//	@return		m_pos
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetPos()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		���f���̌������擾
	//	@return		m_angle
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetAngle()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		���f���̑傫�����擾
	//	@param[in]	m_scale
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetScale()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		���f���̃��[���h�s����擾
	//	@param[in]	&m_worldMat
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT4X4*	GetWorldMatrix();

	//------------------------------------------------------------------------------
	//
	//  @brief		���f�����I���W�i�����ǂ����𔻒�
	//	@return		true:�N���[��	false:�I���W�i��
	//
	//------------------------------------------------------------------------------
	bool IsClone()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		���f�����I���W�i�����ǂ����𔻒�
	//	@return		true:�I���W�i��	false:�N���[��
	//
	//------------------------------------------------------------------------------
	bool IsOriginal()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		���f����ǂݍ���
	//	@param[in]	pFilepath	���f���̃t�@�C���p�X
	//
	//------------------------------------------------------------------------------
	virtual void LoadModel(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		���C�s�b�N
	//	@param[out]	pHitPos			���C�̂��������ʍ��W
	//	@param[out]	pHitFaceNormal	���C�̂��������ʖ@��
	//	@param[in]	pos		���g�̍��W
	//	@param[in]	vec		���C�̌���
	//	@param[in]	dist	���C�̒���
	//	@return		true:�q�b�g	false:�q�b�g���ĂȂ�
	//
	//------------------------------------------------------------------------------
	bool RayPick(
		DirectX::XMFLOAT3*		pHitPos,
		DirectX::XMFLOAT3*		pHitFaceNormal,
		const DirectX::XMFLOAT3&	pos,
		const DirectX::XMFLOAT3&	vec,
		const float		dist
		);

private:
	//------------------------------------------------------------------------------
	//
	//  @brief			���_�f�[�^���R�s�[
	//	@param[in out]	pVertexList		pIMOVertex�̃f�[�^���������ɃR�s�[
	//	@param[in]		pIMOVertex	IMOLoader::GetVertex()
	//
	//------------------------------------------------------------------------------
	void VertexCopy(std::vector<dx_library::tagMeshVertex>* pVertexList,void* pIMOVertex);

protected:
	//------------------------------------------------------------------------------
	//
	//  @brief			�}�e���A�����ɃC���f�b�N�X���쐬
	//	@param[in out]	pIndicesList		pIMOVertex�̃f�[�^���������ɃR�s�[
	//	@param[in]		pIMOVertex	IMOLoader::GetVertex()
	//
	//------------------------------------------------------------------------------
	void CreateIndexMaterialOrder(std::vector<WORD>* pIndicesList, LPWORD pIndices, LPDWORD pAttributes);

	//------------------------------------------------------------------------------
	//
	//  @brief		�e�N�X�`�������쐬����
	//	@param[in]	pTextures	IMOLoader::GetTextures()
	//
	//------------------------------------------------------------------------------
	void CreateTextureInfo(char pTextures[32][MAX_PATH]);

	//------------------------------------------------------------------------------
	//
	//  @brief		Mesh��SkinMesh�̋��ʕ`��֐�
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	pVertexShader	DX_ShaderManager::���_�V�F�[�_�[
	//	@param[in]	pGeometryShader	DX_ShaderManager::GetDefaultGeometryShader3D()
	//	@param[in]	pPixelShader	DX_ShaderManager::GetDefaultPixelShader3D()
	//	@param[in]	pInputLayout	DX_ShaderManager::�C���v�b�g���C�A�E�g
	//
	//------------------------------------------------------------------------------
	void Render(
		ID3D11DeviceContext*	pDeviceContext,
		dx_library::Shader*		pVertexShader,
		dx_library::Shader*		pGeometryShader,
		dx_library::Shader*		pPixelShader,
		ID3D11InputLayout*		pInputLayout
		);

};
#endif // !__DX_MESH_H_
