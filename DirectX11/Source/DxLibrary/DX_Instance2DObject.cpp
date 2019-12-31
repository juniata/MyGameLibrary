#include	"DX_Library.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_Instance2DObject::DX_Instance2DObject() :
	m_pShaderResourceView(nullptr),
	m_pVertexBuffer(nullptr),
	m_pPosBuffer(nullptr),
	m_pUVBuffer(nullptr),
	m_pVertices(nullptr),
	m_pUvs(nullptr),
	m_width(0),
	m_height(0),
	m_instanceNum(0),
	m_enabled(true),
	m_changedUV(false),
	m_changedPos(false),
	m_chipSize(DirectX::XMFLOAT2(1.0f,1.0f))
{}
//-----------------------------------------------------------------------------------------
//
//  �f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
DX_Instance2DObject::~DX_Instance2DObject()
{
	DX_TextureManager::Release(m_pShaderResourceView);
	DELETE_OBJ_ARRAY(m_pVertices);
	DELETE_OBJ_ARRAY(m_pUvs);


	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pUVBuffer);
	SAFE_RELEASE(m_pPosBuffer);
}

//-----------------------------------------------------------------------------------------
//
// �����o�[�ϐ������������A�C���X�^���X�`��I�u�W�F�N�g���쐬����
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize, const DirectX::XMFLOAT2& mapChipSize)
{
	m_pVertices = new DirectX::XMFLOAT3[num];
	m_pUvs = new DirectX::XMFLOAT2[num];
	for (UINT i = 0; i < num; ++i)
	{
		m_pVertices[i].x = 0.0f;
		m_pVertices[i].y = 0.0f;
		m_pVertices[i].z = 0.0f;
		m_pUvs[i].x = 0.0f;
		m_pUvs[i].y = 0.0f;
	}
	m_instanceNum = num;

	LoadTexture(pFilepath);

	m_chipSize.x = mapChipSize.x / DX::CAST::F(m_width);
	m_chipSize.y = mapChipSize.y / DX::CAST::F(m_height);

	DX_System* pSystem = DX_System::GetInstance();

	//	1 ~ 0�̒l�ɕϊ�
	auto centerX = 1.0f / (DX::CAST::F(pSystem->GetScreenWidth()) * 0.5f);
	auto centerY = 1.0f / (DX::CAST::F(pSystem->GetScreenHeight()) * 0.5f);


	DX::tagVertex2D vertices[] = {
		/* ���� */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, m_chipSize.y),
		/* ���� */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(m_chipSize.x, m_chipSize.y),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(m_chipSize.x, 0.0f)
	};

	DX::tagRect renderPos(0.0f, 0.0f, renderSize.x, renderSize.y);

	//	���̍��W
	vertices[1].pos.x = vertices[0].pos.x = centerX * renderPos.x - 1.0f;

	//	���̍��W
	vertices[2].pos.y = vertices[0].pos.y = 1.0f - centerY * renderPos.h;

	//	��̍��W
	vertices[3].pos.y = vertices[1].pos.y = 1.0f - centerY * renderPos.y;

	//	�E�̍��W
	vertices[3].pos.x = vertices[2].pos.x = centerX * renderPos.w - 1.0f;

	ID3D11Device* pDevice = pSystem->GetDevice();

	// ���_�o�b�t�@���쐬
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(vertices), vertices);
	if (DebugValueCheck(m_pVertexBuffer, "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B")) {
		return false;
	}

	// ���_�o�b�t�@(���W���쐬)
	m_pPosBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pVertices[0]) * m_instanceNum, m_pVertices);
	if (DebugValueCheck(m_pPosBuffer, "���W�z��o�b�t�@�̍쐬�Ɏ��s���܂����B")) {
		return false;
	}

	// ���_�o�b�t�@(UV���쐬)
	m_pUVBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pUvs[0]) * m_instanceNum, m_pUvs);
	if (DebugValueCheck(m_pUVBuffer, "UV�o�b�t�@�̍쐬�Ɏ��s���܂����B")) {
		return false;
	}

	return true;
}
bool DX_Instance2DObject::Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize)
{
	m_pVertices = new DirectX::XMFLOAT3[num];
	m_pUvs = new DirectX::XMFLOAT2[num];
	for (UINT i = 0; i < num; ++i)
	{
		m_pVertices[i].x = 0.0f;
		m_pVertices[i].y = 0.0f;
		m_pVertices[i].z = 0.0f;
		m_pUvs[i].x = 0.0f;
		m_pUvs[i].y = 0.0f;
	}
	m_instanceNum = num;

	LoadTexture(pFilepath);

	DX_System* pSystem = DX_System::GetInstance();

	//	1 ~ 0�̒l�ɕϊ�
	auto centerX = 1.0f / (DX::CAST::F(pSystem->GetScreenWidth()) * 0.5f);
	auto centerY = 1.0f / (DX::CAST::F(pSystem->GetScreenHeight()) * 0.5f);


	DX::tagVertex2D vertices[] = {
		/* ���� */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* ���� */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};

	DX::tagRect renderPos(0.0f, 0.0f, renderSize.x, renderSize.y);

	//	���̍��W
	vertices[1].pos.x = vertices[0].pos.x = centerX * renderPos.x - 1.0f;

	//	���̍��W
	vertices[2].pos.y = vertices[0].pos.y = 1.0f - centerY * renderPos.h;

	//	��̍��W
	vertices[3].pos.y = vertices[1].pos.y = 1.0f - centerY * renderPos.y;

	//	�E�̍��W
	vertices[3].pos.x = vertices[2].pos.x = centerX * renderPos.w - 1.0f;

	ID3D11Device* pDevice = pSystem->GetDevice();

	// ���_�o�b�t�@���쐬
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(vertices), vertices);

	// ���_�o�b�t�@(���W���쐬)
	m_pPosBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pVertices[0]) * m_instanceNum, m_pVertices);

	// ���_�o�b�t�@(UV���쐬)
	m_pUVBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pUvs[0]) * m_instanceNum, m_pUvs);

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`����ǂݍ���
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::LoadTexture(const char* pFilepath)
{
	bool  result = false;

	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\2dobject\\", pFilepath);

	//	�e�N�X�`�����擾
	m_pShaderResourceView = DX_TextureManager::GetTexture(texturePath);

	//	�e�N�X�`�������[�h�ł��Ă邩�`�F�b�N
	if (m_pShaderResourceView) {
		//	�e�N�X�`�����ׂ��ȏ����擾
		ID3D11Resource* pResource = nullptr;
		m_pShaderResourceView->GetResource(&pResource);

		ID3D11Texture2D* pTexture2D = nullptr;
		if (SUCCEEDED(pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture2D))) {

			D3D11_TEXTURE2D_DESC texDesc;
			pTexture2D->GetDesc(&texDesc);

			//	�e�N�X�`���T�C�Y���擾
			m_height = texDesc.Height;
			m_width = texDesc.Width;

			result = true;
		}

		SAFE_RELEASE(pResource);
		SAFE_RELEASE(pTexture2D);
	}

	return result;
}

void DX_Instance2DObject::BufferUpdate()
{
	ID3D11Device* pDevice = DX_System::GetInstance()->GetDevice();

	// ���_�o�b�t�@(���W���쐬)
	if (m_changedPos) {
		SAFE_RELEASE(m_pPosBuffer);
		m_pPosBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pVertices[0]) * m_instanceNum, m_pVertices);
		m_changedPos = false;
	}

	// ���_�o�b�t�@(UV���쐬)
	if (m_changedUV) {
		SAFE_RELEASE(m_pUVBuffer);
		m_pUVBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pUvs[0]) * m_instanceNum, m_pUvs);
		m_changedUV = false;
	}
}


//-----------------------------------------------------------------------------------------
//
//  �`�悷��
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::Render()
{
	auto isSucceed = true;

	if (m_enabled) 
	{
		DX_System* pSystem = DX_System::GetInstance();

		ID3D11Device* pDevice = pSystem->GetDevice();
		ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();


		DX_Shader* pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);
		DX_Shader* pPixelShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

		//	�V�F�[�_�[���p���J�n
		pVertexShader->Begin(pDeviceContext);
		pPixelShader->Begin(pDeviceContext);

		unsigned int strides[] = { sizeof(DX::tagVertex2D) ,sizeof(m_pVertices[0]), sizeof(m_pUvs[0]) };
		unsigned int offsets[] = { 0, 0, 0 };

		// �o�b�t�@���X�V����
		BufferUpdate();

		ID3D11Buffer* buffers[] = { m_pVertexBuffer, m_pPosBuffer, m_pUVBuffer };

		// ���K�������E�B���h�E�T�C�Y�𑗂�
		DX_ShaderManager::SetVector(0, DirectX::XMFLOAT4(2.0f / DX::CAST::F(pSystem->GetScreenWidth()), 2.0f / DX::CAST::F(pSystem->GetScreenHeight()), 0.0f, 0.0f), pDevice, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

		//	VertexBuffer�𑗂�
		pDeviceContext->IASetVertexBuffers(0, 3, buffers, strides, offsets);

		//	InputLayout�̐ݒ�𑗂�
		pDeviceContext->IASetInputLayout(DX_ShaderManager::GetDefaultInputLayoutInstance2D());

		//	Primitive�̐ݒ�𑗂�
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �e�N�X�`�����𑗂�
		isSucceed = DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 1, &m_pShaderResourceView, DX_SHADER_TYPE::PIXEL_SHADER);

		// �C���X�^���X�`����s��
		pDeviceContext->DrawInstanced(4, m_instanceNum, 0, 0);

		//	�V�F�[�_�[���p���I��
		pVertexShader->End(pDeviceContext);
		pPixelShader->End(pDeviceContext);
	}

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���ԍ��̃I�u�W�F�N�g�𖳌������܂�(�`�悳��Ȃ��Ȃ�)
//
//-----------------------------------------------------------------------------------------
void DX_Instance2DObject::Disable()
{
	m_enabled = false;
}

//------------------------------------------------------------------------------
//
//  @brief		�C���^���X�`����s���悤�ɂ���
//
//------------------------------------------------------------------------------
void DX_Instance2DObject::Enable()
{
	m_enabled = true;
}

//------------------------------------------------------------------------------
//
//  @brief		�w�肵���C���X�^���X���������ǂ���
//
//------------------------------------------------------------------------------
bool DX_Instance2DObject::IsDisable() const
{
	return !m_enabled;
}

//------------------------------------------------------------------------------
//
//  @brief		�w�肵���C���X�^���X���L�����ǂ���
//
//------------------------------------------------------------------------------
bool DX_Instance2DObject::IsEnable() const
{
	return m_enabled;
}