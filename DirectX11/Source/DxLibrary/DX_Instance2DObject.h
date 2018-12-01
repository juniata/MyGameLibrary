#ifndef __DX_INSTANCE_2DOBJECT_H_
#define __DX_INSTANCE_2DOBJECT_H_

//------------------------------------------------------------------------------
//
// 2DObject���C���X�^���X�`�悷��
// �摜�T�C�Y���傫���قǕ`�敉�ׂ�������̂ŁA�����ɂ͋C��t���ĉ�����
// ���͍��W��z�l�ŕ`�悷�邩���Ȃ��������߂Ă�
//
//------------------------------------------------------------------------------
class DX_Instance2DObject
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�[�ϐ���������
	//
	//------------------------------------------------------------------------------
	DX_Instance2DObject();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�X�g���N�^
	//
	//------------------------------------------------------------------------------
	~DX_Instance2DObject();

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�[�ϐ������������A�C���X�^���X�`��I�u�W�F�N�g���쐬����
	//	@param[in]	pFilepath	�e�N�X�`���̃t�@�C���p�X
	//	@param[in]	num			�C���X�^���X��
	//	@param[in]	renderSize	�`�悷��T�C�Y
	//
	//------------------------------------------------------------------------------
	bool Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize);
	bool Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize, const DirectX::XMFLOAT2& mapChipSize);


	//------------------------------------------------------------------------------
	//
	//  @brief		�`�悷��
	//
	//------------------------------------------------------------------------------
	bool Render();

	//------------------------------------------------------------------------------
	//
	//  @brief		�C���^���X�`����s����Ȃ��悤�ɂ���
	//
	//------------------------------------------------------------------------------
	void Disable();

	//------------------------------------------------------------------------------
	//
	//  @brief		�C���^���X�`����s���悤�ɂ���
	//
	//------------------------------------------------------------------------------
	void Enable();

	//------------------------------------------------------------------------------
	//
	//  @brief		�w�肵���C���X�^���X���������ǂ���
	//	@return		�������ǂ���
	//
	//------------------------------------------------------------------------------
	bool IsDisable() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		�w�肵���C���X�^���X���L�����ǂ���
	//	@return		�L�����ǂ���
	//
	//------------------------------------------------------------------------------
	bool IsEnable() const;

	inline bool IsEnalbe(const unsigned int index) { return m_pVertices[index].z == 0.0f; }
	inline bool IsDisable(const unsigned int index) { return m_pVertices[index].z != 0.0f; }
	inline void Enable(const unsigned int index) { m_pVertices[index].z = 0.0f; }
	inline void Disable(const unsigned int index) {	m_pVertices[index].z = 1.1f; }
	inline void SetPos(const unsigned int index, const DirectX::XMFLOAT2& pos) { m_pVertices[index].x = pos.x; m_pVertices[index].y = pos.y; m_changedPos = true; }
	inline DirectX::XMFLOAT2 GetPos(const unsigned int index) { return DirectX::XMFLOAT2(m_pVertices[index].x, m_pVertices[index].y); }
	inline void SetUV(const unsigned int index, const unsigned int uvIndexX, const unsigned int uvIndexY) { 
		m_pUvs[index].x = m_chipSize.x * CAST_F(uvIndexX);
		m_pUvs[index].y = m_chipSize.y * CAST_F(uvIndexY);
		m_changedUV = true; 
	}
	inline DirectX::XMFLOAT2 GetUV(const unsigned int index) { return m_pUvs[index]; }

private:
	//	�e�N�X�`�����
	ID3D11ShaderResourceView*		m_pShaderResourceView;

	// ���_�o�b�t�@
	ID3D11Buffer* m_pVertexBuffer;

	// ���_�o�b�t�@(���W)
	ID3D11Buffer* m_pPosBuffer;

	// ���_�o�b�t�@(UV)
	ID3D11Buffer* m_pUVBuffer;

	// ���_���
	DirectX::XMFLOAT3* m_pVertices;
	DirectX::XMFLOAT2* m_pUvs;

	DirectX::XMFLOAT2 m_chipSize;

	// �e�N�X�`���̃T�C�Y
	size_t m_width;
	size_t m_height;

	// �C���X�^���X��
	UINT m_instanceNum;

	// �L�����ǂ���
	bool m_enabled;

	// ���W���X�V�������ǂ���
	bool m_changedPos;

	// UV���W���X�V�������ǂ���
	bool m_changedUV;

	//------------------------------------------------------------------------------
	//
	//  @brief		�e�N�X�`����ǂݍ���
	//	@param[in]	pFilepath	�e�N�X�`���̃t�@�C���p�X
	//
	//------------------------------------------------------------------------------
	bool LoadTexture(const char* pFilepath);

	void BufferUpdate();
};
#endif // !__DX_INSTANCE_2DOBJECT_H_
