#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//	static�萔
//
//-----------------------------------------------------------------------------------------
const float DX_Box::LEFT_POS	= -1.0f;
const float DX_Box::RIGHT_POS	= 1.0f;
const float DX_Box::UP_POS		= 1.0f;
const float DX_Box::DOWN_POS	= -1.0f;
const float DX_Box::FRONT_POS	= -1.0f;
const float DX_Box::BACK_POS	= 1.0f;

//-----------------------------------------------------------------------------------------
//
//	�ϐ��̏������ƃo�b�t�@�̍쐬
//
//-----------------------------------------------------------------------------------------
DX_Box::DX_Box() : 
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pConstantBuffer(nullptr),
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
DX_Box::~DX_Box()
{
	if (!m_isCloned) {
		SAFE_RELEASE(m_pVertexBuffer);
		SAFE_RELEASE(m_pIndexBuffer);
		SAFE_RELEASE(m_pConstantBuffer);
	}
}

//-----------------------------------------------------------------------------------------
//
//	�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
bool DX_Box::Initialize()
{
	return CreateBuffer();
}

//-----------------------------------------------------------------------------------------
//
//	���W��ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Box::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	�傫����ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Box::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_scale = scale;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	������ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Box::SetAngle(const DirectX::XMFLOAT3& angle)
{
	m_angle = angle;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	���W���擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 DX_Box::GetPos() const
{
	return m_pos;
}

//-----------------------------------------------------------------------------------------
//
//	�������擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 DX_Box::GetAngle() const
{
	return m_angle;
}

//-----------------------------------------------------------------------------------------
//
//	�傫�����擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 DX_Box::GetScale() const
{
	return m_scale;
}

//-----------------------------------------------------------------------------------------
//
//	���W�����X�V����
//
//-----------------------------------------------------------------------------------------
void DX_Box::Update()
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
void DX_Box::Render()
{
	DX_System* pSystem = DX_System::GetInstance();
	ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();

	DX_Shader* pVertexShader = DX_ShaderManager::GetShader(DEFAULT_OBJECT_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = DX_ShaderManager::GetShader(DEFAULT_OBJECT_SHADER::PIXEL_SHADER);

	//	�V�F�[�_�[���p���J�n
	pVertexShader->Begin(pDeviceContext);
	pPixelShader->Begin(pDeviceContext);

	unsigned int stride = sizeof(DX::tagObjectVertext);
	unsigned int offset = 0;

	//	VertexBuffer�𑗂�
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// IndexBuffer�𑗂�
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);

	//	InputLayout�̐ݒ�𑗂�
	pDeviceContext->IASetInputLayout(DX_ShaderManager::GetDefaultInputLayoutObject());

	//	Primitive�̐ݒ�𑗂�
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	���[���h�s��𑗂�
	DX_ShaderManager::SetWorldMat(m_pConstantBuffer, m_worldMat, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

	// �`����s��
	pDeviceContext->DrawIndexed(36, 0, 0);

	//	�V�F�[�_�[���p���I��
	pVertexShader->End(pDeviceContext);
	pPixelShader->End(pDeviceContext);
}


//-----------------------------------------------------------------------------------------
//
//	��������
//
//-----------------------------------------------------------------------------------------
DX_Box* DX_Box::Clone()
{
	auto pBox = new DX_Box(*this);
	pBox->m_isCloned = true;

	return pBox;
}


//------------------------------------------------------------------------------
//
//  �I�u�W�F�N�g�������������̂��ǂ���
//
//------------------------------------------------------------------------------
bool DX_Box::IsClone() const
{
	return m_isCloned;
}

//------------------------------------------------------------------------------
//
//  �I�u�W�F�N�g���I���W�i�����ǂ���
//
//------------------------------------------------------------------------------
bool DX_Box::IsOriginal() const
{
	return !m_isCloned;
}

//------------------------------------------------------------------------------
//
//  ���C�L���X�g����
//
//------------------------------------------------------------------------------
bool DX_Box::RayCast(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& vec, const float distance)
{
	auto ret = false;

	return ret;
}

//-----------------------------------------------------------------------------------------
//
//	���_�o�b�t�@�����쐬����
//
//-----------------------------------------------------------------------------------------
bool DX_Box::CreateBuffer()
{
	const int VERTEX_NUM = 8;

	DX::tagObjectVertext pVertices[VERTEX_NUM];

	// �O
	// ����
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_UP].pos.x = DX_Box::LEFT_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_UP].pos.y = DX_Box::UP_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_UP].pos.z = DX_Box::FRONT_POS;

	// �E��
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_UP].pos.x = DX_Box::RIGHT_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_UP].pos.y = DX_Box::UP_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_UP].pos.z = DX_Box::FRONT_POS;

	// �E��
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN].pos.x = DX_Box::RIGHT_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN].pos.y = DX_Box::DOWN_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN].pos.z = DX_Box::FRONT_POS;

	// ����
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_DOWN].pos.x = DX_Box::LEFT_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_DOWN].pos.y = DX_Box::DOWN_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_DOWN].pos.z = DX_Box::FRONT_POS;

	// ��
	// ����
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_UP].pos.x = DX_Box::LEFT_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_UP].pos.y = DX_Box::UP_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_UP].pos.z = DX_Box::BACK_POS;

	// �E��
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_UP].pos.x = DX_Box::RIGHT_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_UP].pos.y = DX_Box::UP_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_UP].pos.z = DX_Box::BACK_POS;

	// �E��
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_DOWN].pos.x = DX_Box::RIGHT_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_DOWN].pos.y = DX_Box::DOWN_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_DOWN].pos.z = DX_Box::BACK_POS;

	// ����
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_DOWN].pos.x = DX_Box::LEFT_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_DOWN].pos.y = DX_Box::DOWN_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_DOWN].pos.z = DX_Box::BACK_POS;

	// ���F
	for (int i = 0; i < VERTEX_NUM; ++i) {
		pVertices[i].color = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f);
	}
	
	// �ʖ@�����v�Z
	//CreateFaceNormal(pVertices);

	// ���_�@�����v�Z
	// ���_�@�����i�[
	// �W�I���g���Ŗʖ@�����v�Z�@���C�e�B���O���s��

	// �C���f�b�N�X��ݒ�
	unsigned short pIndices[] =
	{
		// �O��
		DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN, DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,

		// �E��
		DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,

		// ���
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,

		// ���
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,

		// ����
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,

		// ����
		DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,		DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,		DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
	};

	ID3D11Device* pDevice = DX_System::GetInstance()->GetDevice();

	if (nullptr == (m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(pVertices), pVertices))) {
		return false;
	}

	if (nullptr == (m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(pDevice, sizeof(pIndices), pIndices))) {
		return false;
	}
	
	if (nullptr == (m_pConstantBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(m_worldMat)))) {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//	�ʖ@�����쐬����
//
//-----------------------------------------------------------------------------------------
void DX_Box::CreateFaceNormal(DX::tagObjectVertext* pVertex)
{
	// �C���f�b�N�X��ݒ�
	unsigned short pIndices[] =
	{
		// �O��
		DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN, DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,

		// �E��
		DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,

		// ���
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,

		// ���
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,

		// ����
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,

		// ����
		DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,		DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,		DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
	};


	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	DirectX::XMVECTOR faceNor;
	DirectX::XMFLOAT3 faceNorV;

	int index0 = 0;
	int index1 = 0;
	int index2 = 0;

	for (unsigned short i = 0; i < 36; i += 3)
	{
		index0 = pIndices[i];
		index1 = pIndices[i + 1];
		index2 = pIndices[i + 2];

		p0 = DirectX::XMLoadFloat3(&pVertex[index0].pos);
		p1 = DirectX::XMLoadFloat3(&pVertex[index1].pos);
		p2 = DirectX::XMLoadFloat3(&pVertex[index2].pos);

		faceNor = CalcTraiangleNormal(p0, p1, p2);

		DirectX::XMStoreFloat3(&faceNorV, faceNor);
	}
}


//-----------------------------------------------------------------------------------------
//
//	�O�p�`�̖ʖ@�����Z�o����
//
//-----------------------------------------------------------------------------------------
DirectX::XMVECTOR DX_Box::CalcTraiangleNormal(const DirectX::XMVECTOR& p0, const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2)
{
	return DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(p1, p0), DirectX::XMVectorSubtract(p2, p1)));
}

//-----------------------------------------------------------------------------------------
//
//	���_�@�����쐬����
//
//-----------------------------------------------------------------------------------------
void DX_Box::CreateVertexNormal()
{

}