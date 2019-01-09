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
	m_isCloned(false),
	m_isChanged(false),
	m_isLRMirror(false),
	m_isUDMirror(false)
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
bool DX_2DObject::CommonInitialize(const char* pFilepath)
{
	//	���_�o�b�t�@���쐬
	DX::tagVertex2D vertices[] = {
		/* ���� */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* ���� */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(vertices), vertices);

	if (false == DebugValueCheck(m_pVertexBuffer, "�o�b�t�@�̍쐬�Ɏ��s���Ă��܂��B")) {
		return false;
	}

	// �e�N�X�`����ǂݍ���
	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\", pFilepath);
	bool isSucceed = LoadTexture(texturePath);

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s���B
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Initialize(const char* pFilepath)
{
	bool isSucceed = CommonInitialize(pFilepath);

	DX_System* pSystem = DX_System::GetInstance();

	SetRectPos(0.0f, 0.0f, CAST_F(pSystem->GetWindowWidth()), CAST_F(pSystem->GetWindowHeight()));
	SetUV(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height));
	Update();

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s���B
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Initialize(const char* pFilepath, const DX::tagRect& rectPos)
{
	bool isSucceed = CommonInitialize(pFilepath);

	SetRectPos(rectPos);
	SetUV(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height));
	Update();

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s���B
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Initialize(const char* pFilepath, const DX::tagRect& rectPos, const DX::tagRect& uv)
{
	bool isSucceed = CommonInitialize(pFilepath);

	SetRectPos(rectPos);
	SetUV(uv);
	Update();

	return isSucceed;
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
	auto result = false;


	DX_System* pSystem = DX_System::GetInstance();
	//	�f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pContext = pSystem->GetDeviceContext();

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

	if (false == DebugValueCheck(m_pShaderResourceView, "�e�N�X�`���̓ǂݍ��݂��ł��Ă��܂���B")) {
		return false;
	}

	//	�e�N�X�`�����ׂ��ȏ����擾
	ID3D11Resource* pResource = nullptr;
	m_pShaderResourceView->GetResource(&pResource);

	ID3D11Texture2D* pTex2D = nullptr;
	pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTex2D);

		D3D11_TEXTURE2D_DESC texDesc;
	pTex2D->GetDesc(&texDesc);
	//	�e�N�X�`���T�C�Y���擾

	m_height = texDesc.Height;
	m_width = texDesc.Width;

	SAFE_RELEASE(pResource);
	SAFE_RELEASE(pTex2D);

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	��������
//
//-----------------------------------------------------------------------------------------
DX_2DObject* DX_2DObject::Clone()
{
	auto pObject = new DX_2DObject(*this);
	pObject->m_isCloned = true;
	
	return pObject;
}

//-----------------------------------------------------------------------------------------
//
//	�I�u�W�F�N�g�������������̂��ǂ���
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsClone() const
{
	return m_isCloned;
}

//-----------------------------------------------------------------------------------------
//
//	�I�u�W�F�N�g���I���W�i�����ǂ���
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsOriginal() const
{
	return !m_isCloned;
}

//-----------------------------------------------------------------------------------------
//
//	��ʓ��ɕ`�悳��Ă��邩�ǂ���
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsInScreen() const
{
	auto isInScreen = false;

	DX_System* pSystem = DX_System::GetInstance();
	float height = CAST_F(pSystem->GetWindowHeight());
	float width = CAST_F(pSystem->GetWindowWidth());

	if (0.0f <= m_rectPos.y && m_rectPos.bottom <= height &&
		0.0f <= m_rectPos.x && m_rectPos.right <= width)
	{
		isInScreen = true;
	}

	return isInScreen;
}

//-----------------------------------------------------------------------------------------
//
//	�`����W��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::SetRectPos(const DX::tagRect& rect)
{
	m_rectPos = rect;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	�`����W��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::SetRectPos(const float left, const float top, const float right, const float bottom)
{
	m_rectPos.left = left;
	m_rectPos.top = top;
	m_rectPos.right = right;
	m_rectPos.bottom = bottom;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	UV���W��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::SetUV(const DX::tagRect& uv)
{
	m_uv = uv;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	UV���W��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::SetUV(const float left, const float top, const float right, const float bottom)
{
	m_uv.left = left;
	m_uv.top = top;
	m_uv.right = right;
	m_uv.bottom = bottom;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	�`����W���擾����
//
//-----------------------------------------------------------------------------------------
DX::tagRect  DX_2DObject::GetRectPos() const
{
	return m_rectPos;
}

//-----------------------------------------------------------------------------------------
//
//	UV���W���擾����
//
//-----------------------------------------------------------------------------------------
DX::tagRect  DX_2DObject::GetUV() const
{
	return m_uv;
}

//-----------------------------------------------------------------------------------------
//
//	���_�����X�V����
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Update(const bool isLRMirror, const bool isUDMirror)
{
	if (m_isChanged || m_isLRMirror != isLRMirror || m_isUDMirror != isUDMirror)
	{
		DX_System* pSystem = DX_System::GetInstance();
		ID3D11DeviceContext* pContext = pSystem->GetDeviceContext();

		//	���_�����쐬
		CreateVertex(pContext, m_rectPos, m_uv, isLRMirror, isUDMirror);

		m_isChanged = false;
		m_isLRMirror = isLRMirror;
		m_isUDMirror = isUDMirror;
	}
}

//-----------------------------------------------------------------------------------------
//
//  ���_�����쐬����
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::CreateVertex(ID3D11DeviceContext* pContext, const DX::tagRect& rectPos, const DX::tagRect& uv, const bool isLRMirror, const bool isUDMirror)
{
	DX_System* pSystem = DX_System::GetInstance();

	float windowWidth = CAST_F(pSystem->GetWindowWidth());
	float windowHeight = CAST_F(pSystem->GetWindowHeight());

	//	-1.0f ~ 1.0f�ɍ��W�𐳋K������
	DirectX::XMFLOAT2 center(1.0f / (windowWidth * 0.5f), 1.0f / (windowHeight * 0.5f));

	DX::tagRect norRectPos;
	norRectPos.left		= center.x * rectPos.x - 1.0f;
	norRectPos.right	= center.x * rectPos.w - 1.0f;
	norRectPos.bottom	= 1.0f - center.y * rectPos.h;
	norRectPos.top		= 1.0f - center.y * rectPos.y;



	//	UV���W��0.0f ~ 1.0f�ɐ��K������
	DirectX::XMFLOAT2 centerUV(1.0f / CAST_F(m_width), 1.0f / CAST_F(m_height));

	DX::tagRect norUV;

	//	�㉺���E���]���邩�ǂ���
	if (isLRMirror && isUDMirror) {
		norUV.left		= centerUV.x * uv.w;
		norUV.top		= centerUV.y * uv.h;
		norUV.right		= centerUV.x * uv.x;
		norUV.bottom	= centerUV.y * uv.y;
	}
	else {

		// ���E���]���邩�ǂ���
		if (isLRMirror) {
			norUV.left		= centerUV.x * uv.w;
			norUV.top		= centerUV.y * uv.y;
			norUV.right		= centerUV.x * uv.x;
			norUV.bottom	= centerUV.y * uv.h;
		}
		//	�㉺���]���邩�ǂ���
		else if (isUDMirror) {
			norUV.left		= centerUV.x * uv.x;
			norUV.top		= centerUV.y * uv.h;
			norUV.right		= centerUV.x * uv.w;
			norUV.bottom	= centerUV.y * uv.y;
		}
		//	�Ȃ�����Ȃ��ꍇ
		else {
			norUV.left		= centerUV.x * uv.x;
			norUV.top		= centerUV.y * uv.y;
			norUV.right		= centerUV.x * uv.w;
			norUV.bottom	= centerUV.y * uv.h;
		}
	}
	
	//	���_���
	DX::tagVertex2D pVertices[4];

	//	�`����W��ݒ肷��
	//	���̍��W
	pVertices[0].pos.x = norRectPos.left;
	pVertices[1].pos.x = norRectPos.left;

	//	���̍��W
	pVertices[0].pos.y = norRectPos.bottom;
	pVertices[2].pos.y = norRectPos.bottom;

	//	��̍��W
	pVertices[1].pos.y = norRectPos.top;
	pVertices[3].pos.y = norRectPos.top;

	//	�E�̍��W
	pVertices[2].pos.x = norRectPos.right;
	pVertices[3].pos.x = norRectPos.right;

	//	z�l
	pVertices[0].pos.z = 0.0f;
	pVertices[1].pos.z = 0.0f;
	pVertices[2].pos.z = 0.0f;
	pVertices[3].pos.z = 0.0f;

	//	UV���W��ݒ肷��
	//	���̍��W
	pVertices[0].uv.x = norUV.left;
	pVertices[1].uv.x = norUV.left;

	//	���̍��W
	pVertices[0].uv.y = norUV.bottom;
	pVertices[2].uv.y = norUV.bottom;

	//	��̍��W
	pVertices[1].uv.y = norUV.top;
	pVertices[3].uv.y = norUV.top;

	//	�E�̍��W
	pVertices[2].uv.x = norUV.right;
	pVertices[3].uv.x = norUV.right;

	// �o�b�t�@�̏㏑��
	pContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, pVertices, 0, 0);
}