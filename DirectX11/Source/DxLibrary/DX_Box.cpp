#include	"DX_Library.h"
using namespace DirectX;

DX_Box::DX_Box() : 
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pos(0.0f, 0.0f, 0.0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_angle(0.0f, 0.0f, 0.0f),
	m_bChanged(false)
{
	tagVertex vertex[8];

	// �O
	// ����
	vertex[0].pos.x = -1.0f;
	vertex[0].pos.y = 1.0f;
	vertex[0].pos.z = -1.0f;
	
	// �E��
	vertex[1].pos.x = 1.0f;
	vertex[1].pos.y = 1.0f;
	vertex[1].pos.z = -1.0f;

	// �E��
	vertex[2].pos.x = 1.0f;
	vertex[2].pos.y = -1.0f;
	vertex[2].pos.z = -1.0f;

	// ����
	vertex[3].pos.x = -1.0f;
	vertex[3].pos.y = -1.0f;
	vertex[3].pos.z = -1.0f;

	// ��
	// ����
	vertex[4].pos.x = -1.0f;
	vertex[4].pos.y = 1.0f;
	vertex[4].pos.z = 1.0f;

	// �E��
	vertex[5].pos.x = 1.0f;
	vertex[5].pos.y = 1.0f;
	vertex[5].pos.z = 1.0f;

	// �E��
	vertex[6].pos.x = 1.0f;
	vertex[6].pos.y = -1.0f;
	vertex[6].pos.z = 1.0f;

	// ����
	vertex[7].pos.x = -1.0f;
	vertex[7].pos.y = -1.0f;
	vertex[7].pos.z = 1.0f;

	unsigned short index[] = 
	{
		// �O��
		0, 1, 3,
		1, 2, 3,

		// ���
		4, 5, 7,
		5, 6, 7,

		// �E��
		1, 5, 2,
		5, 6, 2,

		// ����
		4, 0, 7,
		0, 3, 7,

		// ���
		4, 5, 0,
		5, 1, 0,

		// ����
		7, 6, 3,
		6, 2, 3
	};

	ID3D11Device* pDevice = DX_System::GetInstance()->GetDevice();
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(vertex), vertex);
	m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(pDevice, sizeof(index), index);

	Update();
}

void DX_Box::SetPos(const DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
	m_bChanged = true;
}
void DX_Box::SetScale(const DirectX::XMFLOAT3 scale)
{
	m_scale = scale;
	m_bChanged = true;
}
void DX_Box::SetAngle(const DirectX::XMFLOAT3 angle)
{
	m_angle = angle;
	m_bChanged = true;
}
DX_Box::~DX_Box()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void DX_Box::Update()
{
	if (m_bChanged)
	{
		//	��]�s�� * �g��s�� * ���s�s��
		XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		XMMATRIX angle = XMMatrixRotationRollPitchYaw(m_angle.x, m_angle.y, m_angle.z);
		XMMATRIX translation = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

		XMStoreFloat4x4(&m_worldMat, angle * scale * translation);
		m_bChanged = false;
	}
}
void DX_Box::Render()
{
	DX_System*	pSystem = DX_System::GetInstance();

	ID3D11Device* pDevice = pSystem->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();

	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();


	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�V�F�[�_�[���p���J�n
	pVertexShader->Begin(pDeviceContext);
	pPixelShader->Begin(pDeviceContext);

	unsigned int stride = sizeof(tagVertex2D);
	unsigned int offset = 0;

	//	VertexBuffer�𑗂�
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// IndexBuffer�𑗂�
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);

	//	InputLayout�̐ݒ�𑗂�
	pDeviceContext->IASetInputLayout(pShaderManager->GetDefaultInputLayoutInstance2D());

	//	Primitive�̐ݒ�𑗂�
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �`����s��
	pDeviceContext->DrawIndexed(36, 0, 0);

	//	�V�F�[�_�[���p���I��
	pVertexShader->End(pDeviceContext);
	pPixelShader->End(pDeviceContext);
}