#pragma once


/// <summary>
/// �����`�̃��f��������
/// </summary>
class Box : public GameObject
{
public:
	/// <summary>
	/// �ϐ���������������
	/// </summary>
	Box();

	/// <summary>
	/// �������
	/// </summary>
	~Box();

	/// <summary>
	/// �o�b�t�@���̍쐬���s��
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const DirectX::XMFLOAT3& pos);

	/// <summary>
	/// �傫����ݒ肷��
	/// </summary>
	/// <param name="scale">�傫��</param>
	void SetScale(const DirectX::XMFLOAT3& scale);

	/// <summary>
	/// ������ݒ肷��
	/// </summary>
	/// <param name="angle">����</param>
	void SetAngle(const DirectX::XMFLOAT3& angle);

	/// <summary>
	/// ���W���擾����
	/// </summary>
	/// <returns>���W</returns>
	DirectX::XMFLOAT3 GetPos() const;

	/// <summary>
	/// �������擾����
	/// </summary>
	/// <returns>����</returns>
	DirectX::XMFLOAT3 GetAngle() const;

	/// <summary>
	/// �傫�����擾����
	/// </summary>
	/// <returns>�傫��</returns>
	DirectX::XMFLOAT3 GetScale() const;

	/// <summary>
	/// ���W�����X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�悷��
	/// </summary>
	void Render();

	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>���������I�u�W�F�N�g</returns>
	Box* Clone();

	/// <summary>
	/// �I�u�W�F�N�g�������������̂��ǂ������擾����
	/// </summary>
	/// <returns>�����������̂��ǂ���</returns>
	bool IsClone() const;

	/// <summary>
	/// �I�u�W�F�N�g���I���W�i�����ǂ������擾����
	/// </summary>
	/// <returns>�I���W�i�����ǂ���</returns>
	bool IsOriginal() const;

	/// <summary>
	/// ���C�L���X�g����
	/// </summary>
	/// <param name="pos">�������肽���I�u�W�F�N�g�̍��W</param>
	/// <param name="vec">�������肽���I�u�W�F�N�g�̃x�N�g��</param>
	/// <param name="distance">���C�̒���</param>
	/// <returns>�����������ǂ���</returns>
	bool RayCast(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& vec, const float distance);
private:
	static const int VERTEX_COUNT = 12;
	static const int INDEX_COUNT = 38;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_angle;
	DirectX::XMFLOAT4X4 m_worldMat;
	
	int m_pastRaypickIndex;
	unsigned short m_indices[INDEX_COUNT];
	bool m_isChanged;
	bool m_isCloned;

	/// <summary>
	/// �o�b�t�@���쐬����
	/// </summary>
	/// <returns>�쐬�ł������ǂ���</returns>
	bool CreateBuffer();

	/// <summary>
	/// �@�����쐬����
	/// </summary>
	/// <param name="pVertices">���_���</param>
	void CreateNormal(DX::tagObjectVertext* pVertices);

	/// <summary>
	/// �ʖ@�����Z�o����
	/// </summary>
	/// <param name="faceV">�Z�o�����ʖ@�����i�[�����</param>
	/// <param name="p0">���_0�̍��W</param>
	/// <param name="p1">���_1�̍��W</param>
	/// <param name="p2">���_2�̍��W</param>
	void CalcFaceNormal(DirectX::XMFLOAT3& faceV, const DirectX::XMVECTOR& p0, const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2);

	/// <summary>
	/// �Z�o�����ʖ@�����A���_�̖@���ɉ��Z���Ă���
	/// </summary>
	/// <param name="outVf">�ʖ@�����i�[�����</param>
	/// <param name="vN">���_�@��</param>
	/// <param name="faceN">�ʖ@��</param>
	void AddFaceNormal(DirectX::XMFLOAT3& outVf, const DirectX::XMFLOAT3& vN, const DirectX::XMFLOAT3& faceN);


};