#include	"DX_Library.h"

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
	m_isCloned(false)
{
	CreateBuffer();
	Update();
}

//-----------------------------------------------------------------------------------------
//
//	�������
//
//-----------------------------------------------------------------------------------------
DX_Box::~DX_Box()
{
	if (m_isCloned) {
		SAFE_RELEASE(m_pVertexBuffer);
		SAFE_RELEASE(m_pIndexBuffer);
		SAFE_RELEASE(m_pConstantBuffer);
	}
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
	DX_System*	pSystem = DX_System::GetInstance();

	ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();

	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();

	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_OBJECT_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_OBJECT_SHADER::PIXEL_SHADER);

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
	pDeviceContext->IASetInputLayout(pShaderManager->GetDefaultInputLayoutObject());

	//	Primitive�̐ݒ�𑗂�
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	���[���h�s��𑗂�
	pShaderManager->SetWorldMat(m_pConstantBuffer, m_worldMat, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

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
bool DX_Box::RayCast(const DirectX::XMFLOAT3 pos)
{
	auto ret = false;

	return ret;
}
//-----------------------------------------------------------------------------------------
//
//	���_�o�b�t�@�����쐬����
//
//-----------------------------------------------------------------------------------------
void DX_Box::CreateBuffer()
{
	const int VERTEX_NUM = 8;

	DX::tagObjectVertext pVertices[VERTEX_NUM];

	const float LEFT_POS = -1.0f;
	const float RIGHT_POS = 1.0f;
	const float UP_POS = 1.0f;
	const float DOWN_POS = -1.0f;
	const float FRONT_POS = -1.0f;
	const float BACK_POS = 1.0f;

	const int FRONT_LEFT_UP_INDEX = 0;
	const int FRONT_RIGHT_UP_INDEX = 1;
	const int FRONT_RIGHT_DOWN_INDEX = 2;
	const int FRONT_LEFT_DOWN_INDEX = 3;
	const int BACK_LEFT_UP_INDEX = 4;
	const int BACK_RIGHT_UP_INDEX = 5;
	const int BACK_RIGHT_DOWN_INDEX = 6;
	const int BACK_LEFT_DOWN_INDEX = 7;

	// �O
	// ����
	pVertices[FRONT_LEFT_UP_INDEX].pos.x = LEFT_POS;
	pVertices[FRONT_LEFT_UP_INDEX].pos.y = UP_POS;
	pVertices[FRONT_LEFT_UP_INDEX].pos.z = FRONT_POS;

	// �E��
	pVertices[FRONT_RIGHT_UP_INDEX].pos.x = RIGHT_POS;
	pVertices[FRONT_RIGHT_UP_INDEX].pos.y = UP_POS;
	pVertices[FRONT_RIGHT_UP_INDEX].pos.z = FRONT_POS;

	// �E��
	pVertices[FRONT_RIGHT_DOWN_INDEX].pos.x = RIGHT_POS;
	pVertices[FRONT_RIGHT_DOWN_INDEX].pos.y = DOWN_POS;
	pVertices[FRONT_RIGHT_DOWN_INDEX].pos.z = FRONT_POS;

	// ����
	pVertices[FRONT_LEFT_DOWN_INDEX].pos.x = LEFT_POS;
	pVertices[FRONT_LEFT_DOWN_INDEX].pos.y = DOWN_POS;
	pVertices[FRONT_LEFT_DOWN_INDEX].pos.z = FRONT_POS;

	// ��
	// ����
	pVertices[BACK_LEFT_UP_INDEX].pos.x = LEFT_POS;
	pVertices[BACK_LEFT_UP_INDEX].pos.y = UP_POS;
	pVertices[BACK_LEFT_UP_INDEX].pos.z = BACK_POS;

	// �E��
	pVertices[BACK_RIGHT_UP_INDEX].pos.x = RIGHT_POS;
	pVertices[BACK_RIGHT_UP_INDEX].pos.y = UP_POS;
	pVertices[BACK_RIGHT_UP_INDEX].pos.z = BACK_POS;

	// �E��
	pVertices[BACK_RIGHT_DOWN_INDEX].pos.x = RIGHT_POS;
	pVertices[BACK_RIGHT_DOWN_INDEX].pos.y = DOWN_POS;
	pVertices[BACK_RIGHT_DOWN_INDEX].pos.z = BACK_POS;

	// ����
	pVertices[BACK_LEFT_DOWN_INDEX].pos.x = LEFT_POS;
	pVertices[BACK_LEFT_DOWN_INDEX].pos.y = DOWN_POS;
	pVertices[BACK_LEFT_DOWN_INDEX].pos.z = BACK_POS;

	// ���F
	for (int i = 0; i < VERTEX_NUM; ++i) {
		pVertices[i].color = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f);
	}
	
	// �ʖ@�����v�Z
	// ���_�@�����v�Z
	// ���_�@�����i�[
	// �W�I���g���Ŗʖ@�����v�Z�@���C�e�B���O���s��

	// �C���f�b�N�X��ݒ�
	unsigned short pIndices[] =
	{
		// �O��
		FRONT_LEFT_UP_INDEX,	FRONT_RIGHT_UP_INDEX,	FRONT_LEFT_DOWN_INDEX,
		FRONT_RIGHT_UP_INDEX,	FRONT_RIGHT_DOWN_INDEX, FRONT_LEFT_DOWN_INDEX,

		// �E��
		FRONT_RIGHT_UP_INDEX,	BACK_RIGHT_UP_INDEX,	FRONT_RIGHT_DOWN_INDEX,
		BACK_RIGHT_UP_INDEX,	BACK_RIGHT_DOWN_INDEX,	FRONT_RIGHT_DOWN_INDEX,

		// ���
		BACK_RIGHT_UP_INDEX,	BACK_LEFT_UP_INDEX,		BACK_RIGHT_DOWN_INDEX,
		BACK_LEFT_UP_INDEX,		BACK_LEFT_DOWN_INDEX,	BACK_RIGHT_DOWN_INDEX,

		// ���
		BACK_LEFT_UP_INDEX,		BACK_RIGHT_UP_INDEX,	FRONT_LEFT_UP_INDEX,
		BACK_RIGHT_UP_INDEX,	FRONT_RIGHT_UP_INDEX,	FRONT_LEFT_UP_INDEX,

		// ����
		BACK_LEFT_UP_INDEX,		FRONT_LEFT_UP_INDEX,	BACK_LEFT_DOWN_INDEX,
		FRONT_LEFT_UP_INDEX,	FRONT_LEFT_DOWN_INDEX,	BACK_LEFT_DOWN_INDEX,

		// ����
		FRONT_LEFT_DOWN_INDEX,	FRONT_RIGHT_DOWN_INDEX, BACK_LEFT_DOWN_INDEX,
		FRONT_RIGHT_DOWN_INDEX,	BACK_RIGHT_DOWN_INDEX,	BACK_LEFT_DOWN_INDEX
	};

	ID3D11Device* pDevice = DX_System::GetInstance()->GetDevice();

	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(pVertices), pVertices);
	m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(pDevice, sizeof(pIndices), pIndices);
	m_pConstantBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(DirectX::XMFLOAT4X4));
}