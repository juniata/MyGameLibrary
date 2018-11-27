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
	m_width(0),
	m_height(0),
	m_pInstance2DList(nullptr),
	m_instanceNum(0),
	m_enabled(true)
{}
//-----------------------------------------------------------------------------------------
//
//  �f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
DX_Instance2DObject::~DX_Instance2DObject()
{
	DX_TextureManager::GetInstance()->Release(m_pShaderResourceView);
	DELETE_OBJ_ARRAY(m_pInstance2DList);
	SAFE_RELEASE(m_pVertexBuffer);
}

//-----------------------------------------------------------------------------------------
//
// �����o�[�ϐ������������A�C���X�^���X�`��I�u�W�F�N�g���쐬����
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize)
{
	m_pInstance2DList = new DX::tagInstance2D[num];
	for (UINT i = 0; i < num; ++i)
	{
		m_pInstance2DList[i].x = 0.0f;
		m_pInstance2DList[i].y = 0.0f;
		m_pInstance2DList[i].z = 0.0f;
	}
	m_instanceNum = num;

	LoadTexture(pFilepath);

	//	1 ~ 0�̒l�ɕϊ�
	const float centerX = 1.0f / (CAST_F(DX_System::GetWindowWidth()) * 0.5f);
	const float centerY = 1.0f / (CAST_F(DX_System::GetWindowHeight()) * 0.5f);


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

	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(vertices), vertices);

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
	m_pShaderResourceView = DX_TextureManager::GetInstance()->GetTexture(texturePath);

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


//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���X�g���擾����
//
//-----------------------------------------------------------------------------------------
DX::tagInstance2D* DX_Instance2DObject::GetInstanceList()
{
	return m_pInstance2DList;
}

//-----------------------------------------------------------------------------------------
//
//  �`�悷��
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::Render()
{
	bool result = false;

	DEBUG_VALUE_CHECK(m_pVertexBuffer, "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B");
	DEBUG_VALUE_CHECK(m_pShaderResourceView, "�e�N�X�`���̃t�@�C���p�X���Ԉ���Ă��܂�");
	
	if (m_enabled) 
	{
		DX_System* pSystem = DX_System::GetInstance();
		DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
		ID3D11Device* pDevice = pSystem->GetDevice();
		ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();


		DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);
		DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

		//	�V�F�[�_�[���p���J�n
		pVertexShader->Begin(pDeviceContext);
		pPixelShader->Begin(pDeviceContext);

		unsigned int strides[] = { sizeof(DX::tagVertex2D) ,sizeof(m_pInstance2DList[0]) };
		unsigned int offsets[] = { 0,0 };

		ID3D11Buffer* pInstanceBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pInstance2DList[0]) * m_instanceNum, m_pInstance2DList);
		ID3D11Buffer* buffers[] = { m_pVertexBuffer, pInstanceBuffer };
		DEBUG_VALUE_CHECK(pInstanceBuffer, "�C���X�^���X�o�b�t�@�̍쐬�Ɏ��s���܂����B");


		// ���K�������E�B���h�E�T�C�Y�𑗂�
		pShaderManager->SetVector(0, DirectX::XMFLOAT4(2.0f / CAST_F(pSystem->GetWindowWidth()), 2.0f / CAST_F(pSystem->GetWindowHeight()), 0.0f, 0.0f), pDevice, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

		//	VertexBuffer�𑗂�
		pDeviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);

		//	InputLayout�̐ݒ�𑗂�
		pDeviceContext->IASetInputLayout(pShaderManager->GetDefaultInputLayoutInstance2D());

		//	Primitive�̐ݒ�𑗂�
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �e�N�X�`�����𑗂�
		result = DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 1, &m_pShaderResourceView, DX_SHADER_TYPE::PIXEL_SHADER);

		// �C���X�^���X�`����s��
		pDeviceContext->DrawInstanced(4, m_instanceNum, 0, 0);

		//	�V�F�[�_�[���p���I��
		pVertexShader->End(pDeviceContext);
		pPixelShader->End(pDeviceContext);

		SAFE_RELEASE(pInstanceBuffer);
	}

	return result;
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