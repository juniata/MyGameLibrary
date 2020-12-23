#pragma once

/// <summary>
/// �e�N�X�`����\������@�\���������N���X
/// </summary>
class Object2D : public GameObject
{
public:
	/// <summary>
	/// �����o�ϐ�����������
	/// </summary>
	Object2D();

	/// <summary>
	/// �����o�ϐ��������
	/// </summary>
	virtual ~Object2D();

	/// <summary>
	/// �e�N�X�`���̓ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s�� 
	/// </summary>
	/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X(/Resource/�ȉ�����)</param>
	/// <returns>����</returns>
	bool Initialize(const char* filepath);

	/// <summary>
	/// �w�肵���T�C�Y�Ńe�N�X�`���y�ђ��_�o�b�t�@���쐬����
	/// </summary>
	/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X(/Resource/�ȉ�����)</param>
	/// <param name="rectPos">�`�悷��T�C�Y</param>
	/// <returns>����</returns>
	bool Initialize(const char* filepath, const DX::tagRect& rectPos);

	/// <summary>
	///  �w�肵���T�C�Y��UV�Ńe�N�X�`���y�ђ��_�o�b�t�@���쐬����
	/// </summary>
	/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X(/Resource/�ȉ�����)</param>
	/// <param name="rectPos">�`�悷��T�C�Y</param>
	/// <param name="uv">�`�悷��e�N�X�`�����W</param>
	/// <returns>����</returns>
	bool Initialize(const char* filepath, const DX::tagRect& rectPos, const DX::tagRect& uv);

	/// <summary>
	/// �e�N�X�`���̍������擾����
	/// </summary>
	/// <returns>�e�N�X�`���̍���</returns>
	unsigned int GetHeight()const;

	/// <summary>
	/// �e�N�X�`���̕����擾����
	/// </summary>
	/// <returns>�e�N�X�`���̕�</returns>
	unsigned int GetWidth()const;

	/// <summary>
	/// �`�悷��
	/// </summary>
	/// <returns>����</returns>
	bool Render();

	/// <summary>
	/// �e�N�X�`���𕡐�����
	/// </summary>
	/// <returns>���������I�u�W�F�N�g</returns>
	Object2D* Clone();

	/// <summary>
	/// �I�u�W�F�N�g�������������̂��ǂ������擾����
	/// </summary>
	/// <returns>�����������̂Ȃ�true</returns>
	bool IsClone() const;

	/// <summary>
	/// �I�u�W�F�N�g���I���W�i�����ǂ������擾����
	/// </summary>
	/// <returns>�I���W�i���Ȃ�true</returns>
	bool IsOriginal() const;

	/// <summary>
	/// �X�N���[�����ɕ`�悳��Ă��邩�ǂ������擾����
	/// </summary>
	/// <returns>�`�悳��Ă���Ȃ�true</returns>
	bool IsInScreen() const;

	/// <summary>
	/// �`�悷��I�u�W�F�N�g�̃T�C�Y��ݒ肷��
	/// </summary>
	/// <param name="rect">�`����W</param>
	void SetRect(const DX::tagRect& rect);

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const DirectX::XMFLOAT3& pos);
	
	/// <summary>
	/// �傫����ݒ肷��(�`�悷��I�u�W�F�N�g�̑傫���ɑ΂��Ċ|���Z����)
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const DirectX::XMFLOAT2& scale);

	/// <summary>
	/// �摜�̌����̐ݒ肷��
	/// </summary>
	/// <param name="val">������ݒ�</param>
	/// <param name="isAngle">false�Ȃ烉�W�A���Atrue�Ȃ�x���Őݒ�</param>
	void SetAngle(const float val, bool isAngle = false);

	/// <summary>
	/// UV���W��ݒ肷��
	/// </summary>
	/// <param name="uv">UV���W</param>
	void SetUV(const DX::tagRect& uv);

	/// <summary>
	/// �`����W���擾����
	/// </summary>
	/// <returns>�`����W</returns>
	const DX::tagRect& GetRectPos();

	/// <summary>
	/// UV���W���擾����
	/// </summary>
	/// <returns>UV���W</returns>
	const DX::tagRect& GetUV();

	/// <summary>
	/// ���W�����X�V����
	/// </summary>
	void Update();

protected:
	DX::tagRect m_rect;
	DX::tagRect m_uv;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT2 m_scale;
	float m_angle;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_srv;
	unsigned int m_height;
	unsigned int m_width;
	bool m_isCloned;
	bool m_isChanged;

	/// <summary>
	/// �e�N�X�`���̓ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s�� 
	/// </summary>
	/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X(/Resource/�ȉ�����)</param>
	/// <returns>����</returns>
	bool CommonInitialize(const char* filepath);

	/// <summary>
	/// �e�N�X�`����ǂݍ���
	/// </summary>
	/// <param name="filepath"></param>
	/// <returns>����</returns>
	bool LoadTexture(const char* filepath);

	/// <summary>
	/// ���_�o�b�t�@���X�V����
	/// </summary>
	void UpdateVertexBuffer();
};
