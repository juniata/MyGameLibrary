#include "SkyLibrary.h"


//-----------------------------------------------------------------------------------------
//
//	�ϐ��̏������ƃo�b�t�@�̍쐬
//
//-----------------------------------------------------------------------------------------
Box::Box() :
	m_pos(0.0f, 0.0f, 0.0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_angle(0.0f, 0.0f, 0.0f),
	m_isChanged(true),
	m_isCloned(false),
	m_pastRaypickIndexk(-1)
{
	Update();
}

//-----------------------------------------------------------------------------------------
//
//	�������
//
//-----------------------------------------------------------------------------------------
Box::~Box()
{}

//-----------------------------------------------------------------------------------------
//
//	�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
bool Box::Initialize()
{
	return CreateBuffer();
}

//-----------------------------------------------------------------------------------------
//
//	���W��ݒ�
//
//-----------------------------------------------------------------------------------------
void Box::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	�傫����ݒ�
//
//-----------------------------------------------------------------------------------------
void Box::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_scale = scale;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	������ݒ�
//
//-----------------------------------------------------------------------------------------
void Box::SetAngle(const DirectX::XMFLOAT3& angle)
{
	m_angle = angle;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	���W���擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 Box::GetPos() const
{
	return m_pos;
}

//-----------------------------------------------------------------------------------------
//
//	�������擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 Box::GetAngle() const
{
	return m_angle;
}

//-----------------------------------------------------------------------------------------
//
//	�傫�����擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 Box::GetScale() const
{
	return m_scale;
}

//-----------------------------------------------------------------------------------------
//
//	���W�����X�V����
//
//-----------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------
//
//	�`�悷��
//
//-----------------------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------------------
//
//	��������
//
//-----------------------------------------------------------------------------------------
Box* Box::Clone()
{
	auto pBox = new Box(*this);
	pBox->m_isCloned = true;

	return pBox;
}


//------------------------------------------------------------------------------
//
//  �I�u�W�F�N�g�������������̂��ǂ���
//
//------------------------------------------------------------------------------
bool Box::IsClone() const
{
	return m_isCloned;
}

//------------------------------------------------------------------------------
//
//  �I�u�W�F�N�g���I���W�i�����ǂ���
//
//------------------------------------------------------------------------------
bool Box::IsOriginal() const
{
	return !m_isCloned;
}

//------------------------------------------------------------------------------
//
//  ���C�L���X�g����
//
//------------------------------------------------------------------------------
bool Box::RayCast(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& vec, const float distance)
{
	auto ret = false;

	UNREFERENCED_PARAMETER(pos);
	UNREFERENCED_PARAMETER(vec);
	UNREFERENCED_PARAMETER(distance);

	return ret;
}

//-----------------------------------------------------------------------------------------
//
//	���_�o�b�t�@�����쐬����
//
//-----------------------------------------------------------------------------------------
bool Box::CreateBuffer()
{
	const int VERTEX_NUM = 8;

	DX::tagObjectVertext pVertices[VERTEX_NUM];
	ZeroMemory(pVertices, sizeof(pVertices));

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
	for (int i = 0; i < VERTEX_NUM; ++i) {
		pVertices[i].color = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f);
	}

	// �@�����v�Z
	CreateNormal(pVertices);

	// TODO:CreateNormal�Ȃ��ōs���ƃG���[�ɂȂ�B�A�h���X�̊֌W�H
	for (int i = 0; i < VERTEX_NUM; ++i)
	{
		DirectX::XMStoreFloat3(&pVertices[i].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&pVertices[i].normal)));
		
	}
	// ���_�@�����v�Z
	// ���_�@�����i�[
	// �W�I���g���Ŗʖ@�����v�Z�@���C�e�B���O���s��

	// �C���f�b�N�X��ݒ�
	unsigned short pIndices[] =
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


	m_vertexBuffer.Attach(DX_BufferCreater::VertexBuffer(sizeof(pVertices), pVertices));
	if (nullptr == m_vertexBuffer) {
		return false;
	}

	m_indexBuffer.Attach(DX_BufferCreater::IndexBuffer(sizeof(pIndices), pIndices));
	if (nullptr == m_indexBuffer) {
		return false;
	}

	m_constantBuffer.Attach(DX_BufferCreater::ConstantBuffer(sizeof(m_worldMat)));
	if (nullptr == m_constantBuffer) {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//	�ʖ@�����쐬����
//
//-----------------------------------------------------------------------------------------
void Box::CreateNormal(DX::tagObjectVertext* pVertex)
{
	// �C���f�b�N�X��ݒ�
	unsigned short pIndices[] =
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


	DirectX::XMVECTOR p0, p1, p2;
	DirectX::XMFLOAT3 faceN;

	int index0 = 0;
	int index1 = 0;
	int index2 = 0;

	for (int i = 0; i < 36; i += 3)
	{
		// �ʖ@�����쐬����
		index0 = pIndices[i];
		index1 = pIndices[i + 1];
		index2 = pIndices[i + 2];


		p0 = DirectX::XMLoadFloat3(&pVertex[index0].pos);
		p1 = DirectX::XMLoadFloat3(&pVertex[index1].pos);
		p2 = DirectX::XMLoadFloat3(&pVertex[index2].pos);

		CalcFaceNormal(faceN, p0, p1, p2);

		// �ʖ@���𒸓_�@���ɉ��Z���Ă����B
		AddFaceNormal(pVertex[index0].normal, pVertex[index0].normal, faceN);
		AddFaceNormal(pVertex[index1].normal, pVertex[index1].normal, faceN);
		AddFaceNormal(pVertex[index2].normal, pVertex[index2].normal, faceN);

	}

	//DirectX::XMVECTOR nor;
	//for (int i = 0; i < 36; ++i)
	//{
	//	nor = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&pVertex[i].normal));
	//	DirectX::XMStoreFloat3(&pVertex[i].normal, nor);
	//}
}


//-----------------------------------------------------------------------------------------
//
//	�O�p�`�̖ʖ@�����Z�o����
//
//-----------------------------------------------------------------------------------------
void Box::CalcFaceNormal(DirectX::XMFLOAT3& faceV, const DirectX::XMVECTOR& p0, const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2)
{
	DirectX::XMStoreFloat3(&faceV, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(p1, p0), DirectX::XMVectorSubtract(p2, p1))));
}

//-----------------------------------------------------------------------------------------
//
//	���_�@���ɖʖ@�������Z����
//
//-----------------------------------------------------------------------------------------
void Box::AddFaceNormal(DirectX::XMFLOAT3& outVf, const DirectX::XMFLOAT3& vN, const DirectX::XMFLOAT3& faceN)
{
	DirectX::XMStoreFloat3(&outVf, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&vN), DirectX::XMLoadFloat3(&faceN)));
}
