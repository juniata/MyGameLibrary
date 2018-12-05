#include	"DX_Library.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  �����o�[�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_2DObject::DX_2DObject() :
	m_pVertexBuffer(nullptr),
	m_pShaderResourceView(nullptr),
	m_height(0),
	m_width(0),
	m_bClone(false)
{}


//-----------------------------------------------------------------------------------------
//
//  m_pShaderResourceView�����
//
//-----------------------------------------------------------------------------------------
DX_2DObject::~DX_2DObject()
{
	if (IsOriginal()) {
		SAFE_RELEASE(m_pVertexBuffer);
		DX_TextureManager::GetInstance()->Release(m_pShaderResourceView);
	}
}


//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s���B
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Initialize(const char* pFilepath)
{
	bool result = false;

	//	���_�o�b�t�@���쐬
	DX::tagVertex2D vertices[] = {
		/* ���� */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* ���� */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(vertices), vertices);
	DEBUG_VALUE_CHECK(m_pVertexBuffer, "�o�b�t�@�̍쐬�Ɏ��s���Ă��܂��B");

	// �e�N�X�`����ǂݍ���
	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\2dobject\\", pFilepath);
	result = LoadTexture(texturePath);

	return result;
}


//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`���̍������擾����
//
//-----------------------------------------------------------------------------------------
unsigned int DX_2DObject::GetHeight()const
{
	return m_height;
}

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`���̕����擾����
//
//-----------------------------------------------------------------------------------------
unsigned int DX_2DObject::GetWidth()const
{
	return m_width;
}

//-----------------------------------------------------------------------------------------
//
//  �S��ʂɕ`��
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render()
{
	bool result = false;

	//	�f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	�V�F�[�_�[���擾
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�V�F�[�_�[�𗘗p
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	�`��
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	�V�F�[�_�[���I��
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���͈͂ɕ`��
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render(const DX::tagRect& renderPos)
{
	bool result = false;

	//	�f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	���_�����쐬
	CreateVertex(pContext, renderPos, DX::tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	�V�F�[�_�[���擾
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�V�F�[�_�[�𗘗p
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	�`��
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	�V�F�[�_�[���I��
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���͈́A�w�肵���摜�T�C�Y��`��
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render(const DirectX::XMFLOAT2& renderPos, const DirectX::XMFLOAT2& renderSize)
{
	bool result = false;

	//	�f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	���_�����쐬
	CreateVertex(pContext, DX::tagRect(renderPos.x, renderPos.y, (renderSize.x + renderPos.x), (renderSize.y + renderPos.y)), DX::tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	�V�F�[�_�[���擾
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�V�F�[�_�[�𗘗p
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	�`��
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	�V�F�[�_�[���I��
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���͈͂ɕ`��
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render(const float left, const float top, const float right, const float bottom, bool isMirror)
{

	bool result = false;

	//	�f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	���_�����쐬
	CreateVertex(pContext, DX::tagRect(left, top, right, bottom), DX::tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)), isMirror);

	//	�V�F�[�_�[���擾
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�V�F�[�_�[�𗘗p
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	�`��
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	�V�F�[�_�[���I��
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���͈͂ɕ`��
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render(const DX::tagRect& renderPos, const DX::tagRect& texturePos)
{
	bool result = false;

	//	�f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	���_�����쐬
	CreateVertex(pContext, renderPos, DX::tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	�V�F�[�_�[���擾
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�V�F�[�_�[�𗘗p
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	�`��
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	�V�F�[�_�[���I��
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`����ǂݍ���
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::LoadTexture(const char* pFilepath)
{
	//	�e�N�X�`�����擾
	m_pShaderResourceView = DX_TextureManager::GetInstance()->GetTexture(pFilepath);

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
	else {
		DEBUG_VALUE_CHECK(false, "�e�N�X�`���̓ǂݍ��݂��ł��Ă��܂���B");
	}

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	��������
//
//-----------------------------------------------------------------------------------------
DX_2DObject* DX_2DObject::Clone()
{
	DX_2DObject* pObject = new DX_2DObject(*this);
	pObject->m_bClone = true;
	
	return pObject;
}

//-----------------------------------------------------------------------------------------
//
//	�I�u�W�F�N�g�������������̂��ǂ���
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsClone() const
{
	return m_bClone;
}

//-----------------------------------------------------------------------------------------
//
//	�I�u�W�F�N�g���I���W�i�����ǂ���
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsOriginal() const
{
	return !m_bClone;
}

//-----------------------------------------------------------------------------------------
//
//  ���_�����쐬����
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::CreateVertex(ID3D11DeviceContext* pContext, const DX::tagRect& renderPos, const DX::tagRect& texturePos, bool isMirror)
{
	//	���_���
	DX::tagVertex2D pVertex[4];

	//	1 ~ 0�̒l�ɕϊ�
	const float l_centerX = 1.0f / (CAST_F(DX_System::GetWindowWidth()) * 0.5f);
	const float l_centerY = 1.0f / (CAST_F(DX_System::GetWindowHeight()) * 0.5f);

	//	0~1�̊����̗��Ƃ�����
	const float l_centerUv_X = (1.0f / m_width);
	const float l_centerUv_Y = (1.0f / m_height);

	// UV�̂ق��������t�ɂ���
	//	���̍��W
	pVertex[1].pos.x = pVertex[0].pos.x = l_centerX * renderPos.x - 1.0f;

	//	���̍��W
	pVertex[2].pos.y = pVertex[0].pos.y = 1.0f - l_centerY * renderPos.h;

	//	��̍��W
	pVertex[3].pos.y = pVertex[1].pos.y = 1.0f - l_centerY * renderPos.y;

	//	�E�̍��W
	pVertex[3].pos.x = pVertex[2].pos.x = l_centerX * renderPos.w - 1.0f;

	//	z�l
	pVertex[0].pos.z = 0.0f;
	pVertex[1].pos.z = 0.0f;
	pVertex[2].pos.z = 0.0f;
	pVertex[3].pos.z = 0.0f;

	if (isMirror) {
		//	���̍��W
		pVertex[1].uv.x = pVertex[0].uv.x = l_centerUv_X * texturePos.w;

		//	��̍��W
		pVertex[3].uv.y = pVertex[1].uv.y = l_centerUv_Y * texturePos.y;

		//	�E�̍��W
		pVertex[3].uv.x = pVertex[2].uv.x = l_centerUv_X * texturePos.x;

		//	���̍��W
		pVertex[2].uv.y = pVertex[0].uv.y = l_centerUv_Y * texturePos.h;
	}
	else {
		//	���̍��W
		pVertex[1].uv.x = pVertex[0].uv.x = l_centerUv_X * texturePos.x;

		//	��̍��W
		pVertex[3].uv.y = pVertex[1].uv.y = l_centerUv_Y * texturePos.y;

		//	�E�̍��W
		pVertex[3].uv.x = pVertex[2].uv.x = l_centerUv_X * texturePos.w;

		//	���̍��W
		pVertex[2].uv.y = pVertex[0].uv.y = l_centerUv_Y * texturePos.h;
	}


	// �o�b�t�@�̏㏑��
	pContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, pVertex, 0, 0);
}