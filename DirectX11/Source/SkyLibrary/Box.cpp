#include "SkyLibrary.h"

/// <summary>
/// �ϐ���������������
/// </summary>
Box::Box() :
	m_pos(0.0f, 0.0f, 0.0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_angle(0.0f, 0.0f, 0.0f),
	m_isChanged(true),
	m_isCloned(false),
	m_pastRaypickIndex(-1)
{
	unsigned short indices[] =
	{
		// �O��
		0,1,3,
		1,2,3,

		// �E��
		1,5,2,
		5,6,2,

		// ���
		5,4,6,
		4,7,6,

		// ���
		4,5,0,
		5,1,0,

		// ����
		4,0,7,
		0,3,7,

		// ����
		3,2,7,
		2,6,7
	};
	memcpy(m_indices, indices, sizeof(indices));

	Update();
}

/// <summary>
/// �������
/// </summary>
Box::~Box()
{}

/// <summary>
/// �o�b�t�@���̍쐬���s��
/// </summary>
/// <returns>����</returns>
bool Box::Initialize()
{
	return CreateBuffer();
}

/// <summary>
/// ���W��ݒ肷��
/// </summary>
/// <param name="pos">���W</param>
void Box::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_isChanged = true;
}

/// <summary>
/// �傫����ݒ肷��
/// </summary>
/// <param name="scale">�傫��</param>
void Box::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_scale = scale;
	m_isChanged = true;
}

/// <summary>
/// ������ݒ肷��
/// </summary>
/// <param name="angle">����</param>
void Box::SetAngle(const DirectX::XMFLOAT3& angle)
{
	m_angle = angle;
	m_isChanged = true;
}

/// <summary>
/// ���W���擾����
/// </summary>
/// <returns>���W</returns>
DirectX::XMFLOAT3 Box::GetPos() const
{
	return m_pos;
}

/// <summary>
/// �������擾����
/// </summary>
/// <returns>����</returns>
DirectX::XMFLOAT3 Box::GetAngle() const
{
	return m_angle;
}

/// <summary>
/// �傫�����擾����
/// </summary>
/// <returns>�傫��</returns>
DirectX::XMFLOAT3 Box::GetScale() const
{
	return m_scale;
}

/// <summary>
/// ���W�����X�V����
/// </summary>
void Box::Update()
{
	if (m_isChanged)
	{
		//	��]�s�� * �g��s�� * ���s�s��
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		DirectX::XMMATRIX angle = DirectX::XMMatrixRotationRollPitchYaw(m_angle.x, m_angle.y, m_angle.z);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

		XMStoreFloat4x4(&m_worldMat, angle * scale * translation);
		m_isChanged = false;
	}
}

/// <summary>
/// �`�悷��
/// </summary>
void Box::Render()
{
	DX_System* system = DX_System::GetInstance();
	ID3D11DeviceContext* deviceContext = system->GetDeviceContext();

	DX_ShaderManager* shaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* vertexShader = shaderManager->GetShader(SHADER_BOX::VERTEX_SHADER);
	DX_Shader* pixelShader	= shaderManager->GetShader(SHADER_BOX::PIXEL_SHADER);

	vertexShader->Begin();
	pixelShader->Begin();

	unsigned int stride = sizeof(DX::tagObjectVertext);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetInputLayout(shaderManager->GetInputLayoutBox());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shaderManager->SetWorldMat(m_worldMat, SHADER_TYPE::VERTEX_SHADER);

	deviceContext->DrawIndexed(36, 0, 0);

	vertexShader->End();
	pixelShader->End();
}


/// <summary>
/// ��������
/// </summary>
/// <returns>���������I�u�W�F�N�g</returns>
Box* Box::Clone()
{
	auto pBox = new Box(*this);
	pBox->m_isCloned = true;

	return pBox;
}

/// <summary>
/// �I�u�W�F�N�g�������������̂��ǂ������擾����
/// </summary>
/// <returns>�����������̂��ǂ���</returns>
bool Box::IsClone() const
{
	return m_isCloned;
}

/// �I�u�W�F�N�g���I���W�i�����ǂ������擾����
/// </summary>
/// <returns>�I���W�i�����ǂ���</returns>
bool Box::IsOriginal() const
{
	return !m_isCloned;
}

/// /// <summary>
/// ���C�L���X�g����
/// </summary>
/// <param name="pos">�������肽���I�u�W�F�N�g�̍��W</param>
/// <param name="vec">�������肽���I�u�W�F�N�g�̃x�N�g��</param>
/// <param name="distance">���C�̒���</param>
/// <returns>�����������ǂ���</returns>
bool Box::RayCast(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& vec, const float distance)
{
	auto ret = false;

	UNREFERENCED_PARAMETER(pos);
	UNREFERENCED_PARAMETER(vec);
	UNREFERENCED_PARAMETER(distance);

	return ret;
}

/// <summary>
/// �o�b�t�@���쐬����
/// </summary>
/// <returns>�쐬�ł������ǂ���</returns>
bool Box::CreateBuffer()
{
	DX::tagObjectVertext pVertices[VERTEX_COUNT];

	// �O
	// ����
	pVertices[0].pos.x = -0.5f;
	pVertices[0].pos.y = 0.5f;
	pVertices[0].pos.z = -0.5f;

	// �E��
	pVertices[1].pos.x = 0.5f;
	pVertices[1].pos.y = 0.5f;
	pVertices[1].pos.z = -0.5f;

	// �E��
	pVertices[2].pos.x = 0.5f;
	pVertices[2].pos.y = -0.5f;
	pVertices[2].pos.z = -0.5f;

	// ����
	pVertices[3].pos.x = -0.5f;
	pVertices[3].pos.y = -0.5f;
	pVertices[3].pos.z = -0.5f;

	// ��
	// ����
	pVertices[4].pos.x = -0.5f;
	pVertices[4].pos.y = 0.5f;
	pVertices[4].pos.z = 0.5f;

	// �E��
	pVertices[5].pos.x = 0.5f;
	pVertices[5].pos.y = 0.5f;
	pVertices[5].pos.z = 0.5f;

	// �E��
	pVertices[6].pos.x = 0.5f;
	pVertices[6].pos.y = -0.5f;
	pVertices[6].pos.z = 0.5f;

	// ����
	pVertices[7].pos.x = -0.5f;
	pVertices[7].pos.y = -0.5f;
	pVertices[7].pos.z = 0.5f;

	// ���F
	for (int i = 0; i < VERTEX_COUNT; ++i) {
		pVertices[i].color = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	}

	// �@�����v�Z
	CreateNormal(pVertices);

	m_vertexBuffer.Attach(DX_BufferCreater::VertexBuffer(sizeof(pVertices), pVertices));
	if (nullptr == m_vertexBuffer) {
		return false;
	}

	m_indexBuffer.Attach(DX_BufferCreater::IndexBuffer(sizeof(m_indices), m_indices));
	if (nullptr == m_indexBuffer) {
		return false;
	}

	m_constantBuffer.Attach(DX_BufferCreater::ConstantBuffer(sizeof(m_worldMat)));
	if (nullptr == m_constantBuffer) {
		return false;
	}

	return true;
}

/// <summary>
/// �@�����쐬����
/// </summary>
/// <param name="pVertices">���_���</param>
void Box::CreateNormal(DX::tagObjectVertext* pVertices)
{
	DirectX::XMVECTOR p0, p1, p2;
	DirectX::XMFLOAT3 faceN;

	int index0 = 0;
	int index1 = 0;
	int index2 = 0;

	for (int i = 0; i < INDEX_COUNT; i += 3)
	{
		// �ʖ@�����쐬����
		index0 = m_indices[i];
		index1 = m_indices[i + 1];
		index2 = m_indices[i + 2];

		p0 = DirectX::XMLoadFloat3(&pVertices[index0].pos);
		p1 = DirectX::XMLoadFloat3(&pVertices[index1].pos);
		p2 = DirectX::XMLoadFloat3(&pVertices[index2].pos);

		CalcFaceNormal(faceN, p0, p1, p2);

		// �ʖ@���𒸓_�@���ɉ��Z���Ă����B
		AddFaceNormal(pVertices[index0].normal, pVertices[index0].normal, faceN);
		AddFaceNormal(pVertices[index1].normal, pVertices[index1].normal, faceN);
		AddFaceNormal(pVertices[index2].normal, pVertices[index2].normal, faceN);
	}

	for (int i = 0; i < 12; ++i)
	{
		DirectX::XMStoreFloat3(&pVertices[i].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&pVertices[i].normal)));
	}
}


/// <summary>
/// �ʖ@�����Z�o����
/// </summary>
/// <param name="faceV">�Z�o�����ʖ@�����i�[�����</param>
/// <param name="p0">���_0�̍��W</param>
/// <param name="p1">���_1�̍��W</param>
/// <param name="p2">���_2�̍��W</param>
void Box::CalcFaceNormal(DirectX::XMFLOAT3& faceV, const DirectX::XMVECTOR& p0, const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2)
{
	DirectX::XMStoreFloat3(&faceV, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(p1, p0), DirectX::XMVectorSubtract(p2, p0))));
}

/// <summary>
/// �Z�o�����ʖ@�����A���_�̖@���ɉ��Z���Ă���
/// </summary>
/// <param name="outVf">�ʖ@�����i�[�����</param>
/// <param name="vN">���_�@��</param>
/// <param name="faceN">�ʖ@��</param>
void Box::AddFaceNormal(DirectX::XMFLOAT3& outVf, const DirectX::XMFLOAT3& vN, const DirectX::XMFLOAT3& faceN)
{
	DirectX::XMStoreFloat3(&outVf, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&vN), DirectX::XMLoadFloat3(&faceN)));
}
