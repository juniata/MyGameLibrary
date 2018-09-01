#include	"DX_Library.h"
#include	<stdio.h>
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  �����t���R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
DX_Instance2DObject::DX_Instance2DObject(const char* pFilepath, const UINT num, const XMFLOAT2& renderSize) : m_pPosList(new DirectX::XMFLOAT3[num]), m_instanceNum(num), m_enabled(false)
{
	LoadTexture(pFilepath);
	ZeroMemory(m_pPosList, sizeof(m_pPosList[0]) * num);

	//	1 ~ 0�̒l�ɕϊ�
	const float l_centerX = 1.0f / (CAST_F(DX_System::GetWindowWidth()) * 0.5f);
	const float l_centerY = 1.0f / (CAST_F(DX_System::GetWindowHeight()) * 0.5f);


	tagVertex2D pVertex[] = {
		/* ���� */	XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
		/* ���� */	XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f),
		/* �E�� */	XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),
		/* �E�� */	XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)
	};

	tagRect renderPos(0.0f, 0.0f, renderSize.x, renderSize.y);

	//	���̍��W
	pVertex[1].pos.x = pVertex[0].pos.x = l_centerX * renderPos.x - 1.0f;

	//	���̍��W
	pVertex[2].pos.y = pVertex[0].pos.y = 1.0f - l_centerY * renderPos.h;

	//	��̍��W
	pVertex[3].pos.y = pVertex[1].pos.y = 1.0f - l_centerY * renderPos.y;

	//	�E�̍��W
	pVertex[3].pos.x = pVertex[2].pos.x = l_centerX * renderPos.w - 1.0f;


	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(pVertex[0]) * 4, pVertex);
}

//-----------------------------------------------------------------------------------------
//
//  �f�X�g���N�^
//
//-----------------------------------------------------------------------------------------
DX_Instance2DObject::~DX_Instance2DObject()
{
	DX_TextureManager::GetInstance()->Release(m_pShaderResourceView);
	DELETE_OBJ_ARRAY(m_pPosList);
	SAFE_RELEASE(m_pVertexBuffer);
}

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`����ǂݍ���
//
//-----------------------------------------------------------------------------------------
void DX_Instance2DObject::LoadTexture(const char* pFilepath)
{
	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\2dobject\\", pFilepath);

	//	�e�N�X�`�����擾
	m_pShaderResourceView = DX_TextureManager::GetInstance()->GetTexture(texturePath);

	//	�e�N�X�`�������[�h�ł��Ă邩�`�F�b�N
	if (m_pShaderResourceView) {
		//	�e�N�X�`�����ׂ��ȏ����擾
		ID3D11Resource* l_pResource = nullptr;
		m_pShaderResourceView->GetResource(&l_pResource);

		ID3D11Texture2D* l_pTexture2D = nullptr;
		l_pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&l_pTexture2D);

		D3D11_TEXTURE2D_DESC l_texDesc;
		l_pTexture2D->GetDesc(&l_texDesc);

		//	�e�N�X�`���T�C�Y���擾
		m_height = l_texDesc.Height;
		m_width = l_texDesc.Width;

		SAFE_RELEASE(l_pResource);
		SAFE_RELEASE(l_pTexture2D);
	}
}


//-----------------------------------------------------------------------------------------
//
//  ���W�ꗗ���擾����
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3* DX_Instance2DObject::GetPosList()
{
	return m_pPosList;
}


//-----------------------------------------------------------------------------------------
//
//  �w�肵���ӏ�����̍��W�ꗗ���擾����
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3* DX_Instance2DObject::GetPosList(const unsigned int index)
{
	return &m_pPosList[index];
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

		unsigned int strides[] = { sizeof(tagVertex2D) ,sizeof(m_pPosList[0]) };
		unsigned int offsets[] = { 0,0 };

		ID3D11Buffer* pInstanceBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pPosList[0]) * m_instanceNum, m_pPosList);
		ID3D11Buffer* buffers[] = { m_pVertexBuffer, pInstanceBuffer };
		DEBUG_VALUE_CHECK(pInstanceBuffer, "�C���X�^���X�o�b�t�@�̍쐬�Ɏ��s���܂����B");


		// ���K�������E�B���h�E�T�C�Y�𑗂�
		pShaderManager->SetVector(0, XMFLOAT4(2.0f / CAST_F(pSystem->GetWindowWidth()), 2.0f / CAST_F(pSystem->GetWindowHeight()), 0.0f, 0.0f), pDevice, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

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
void DX_Instance2DObject::Disable(const size_t index)
{
	// �`�悳��Ȃ��Ȃ�悤z�l��1���傫������(����ȊO�������킩���B�B�B)
	m_pPosList[index].z = 1.1f;
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


//-----------------------------------------------------------------------------------------
//
//  �w�肵���ԍ��̃I�u�W�F�N�g�̗L��������i�`�悳���悤�ɂȂ�j
//
//-----------------------------------------------------------------------------------------
void DX_Instance2DObject::Enable(const size_t index)
{
	m_pPosList[index].z = 0.0f;
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
//  @brief		�w�肵���C���X�^���X���L�����ǂ���
//
//------------------------------------------------------------------------------
bool DX_Instance2DObject::IsDisable(const size_t index) const
{
	return (m_pPosList[index].z <= 1.0f);
}

//------------------------------------------------------------------------------
//
//  @brief		�w�肵���C���X�^���X���������ǂ���
//
//------------------------------------------------------------------------------
bool DX_Instance2DObject::IsEnable(const size_t index) const
{
	return (m_pPosList[index].z > 1.0f);
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