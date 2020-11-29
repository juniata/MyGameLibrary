#include <DxLibrary/DX_Library.h>
#include "GameObject.h"
#include "Object2D.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  �����o�[�ϐ���������
//
//-----------------------------------------------------------------------------------------
Object2D::Object2D() :
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
Object2D::~Object2D()
{
}

//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s���B
//
//-----------------------------------------------------------------------------------------
bool Object2D::CommonInitialize(const char* pFilepath)
{
	//	���_�o�b�t�@���쐬
	DX::tagVertex2D vertices[] = {
		/* ���� */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* ���� */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};

	m_vertexBuffer.Attach(DX_BufferCreater::VertexBuffer(sizeof(vertices), vertices));
	if (nullptr == m_vertexBuffer.Get()) {
		TRACE("failed to DX_Buffer::CreateVertexBuffer()");
		return false;
	}

	// �e�N�X�`����ǂݍ���
	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\", pFilepath);

	return LoadTexture(texturePath);
}

//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s���B
//
//-----------------------------------------------------------------------------------------
bool Object2D::Initialize(const char* pFilepath)
{
	bool isSucceed = CommonInitialize(pFilepath);

	DX_System* pSystem = DX_System::GetInstance();
	SetRectPos(0.0f, 0.0f, DX::CAST::F(pSystem->GetScreenWidth()), DX::CAST::F(pSystem->GetScreenHeight()));
	SetUV(0.0f, 0.0f, DX::CAST::F(m_width), DX::CAST::F(m_height));
	Update();

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s���B
//
//-----------------------------------------------------------------------------------------
bool Object2D::Initialize(const char* pFilepath, const DX::tagRect& rectPos)
{
	bool isSucceed = CommonInitialize(pFilepath);

	SetRectPos(rectPos);
	SetUV(0.0f, 0.0f, DX::CAST::F(m_width), DX::CAST::F(m_height));
	Update();

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s���B
//
//-----------------------------------------------------------------------------------------
bool Object2D::Initialize(const char* pFilepath, const DX::tagRect& rectPos, const DX::tagRect& uv)
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
unsigned int Object2D::GetHeight()const
{
	return m_height;
}

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`���̕����擾����
//
//-----------------------------------------------------------------------------------------
unsigned int Object2D::GetWidth()const
{
	return m_width;
}

//-----------------------------------------------------------------------------------------
//
//  �S��ʂɕ`��
//
//-----------------------------------------------------------------------------------------
bool Object2D::Render()
{
	auto result = true;

	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	DX_ShaderManager* shaderManager = DX_ShaderManager::GetInstance();

	DX_Shader* vertexShader = shaderManager->GetShader(SHADER_OBJECT_2D::VERTEX_SHADER);
	DX_Shader* pixelShader = shaderManager->GetShader(SHADER_OBJECT_2D::PIXEL_SHADER);

	vertexShader->Begin();
	pixelShader->Begin();

	unsigned int stride = sizeof(DX::tagVertex2D);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetInputLayout(shaderManager->GetInputLayout2D());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	shaderManager->SetShaderResources(0, 1, m_srv.GetAddressOf(), SHADER_TYPE::PIXEL_SHADER);

	deviceContext->Draw(4, 0);

	vertexShader->End();
	pixelShader->End();

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`����ǂݍ���
//
//-----------------------------------------------------------------------------------------
bool Object2D::LoadTexture(const char* pFilepath)
{
	//	�e�N�X�`�����擾
	m_srv.Attach(DX_TextureManager::GetInstance()->GetTexture(pFilepath));

	if (nullptr == m_srv.Get()) {
		TRACE("failed to DX_TextureManager::GetTexture(), filepath = %s", pFilepath);
		return false;
	}

	DX_TextureManager::GetInstance()->GetTextureSize(&m_height, &m_width, m_srv.Get());

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	��������
//
//-----------------------------------------------------------------------------------------
Object2D* Object2D::Clone()
{
	auto pObject = new Object2D(*this);
	pObject->m_isCloned = true;
	pObject->m_vertexBuffer = m_vertexBuffer;

	return pObject;
}

//-----------------------------------------------------------------------------------------
//
//	�I�u�W�F�N�g�������������̂��ǂ���
//
//-----------------------------------------------------------------------------------------
bool Object2D::IsClone() const
{
	return m_isCloned;
}

//-----------------------------------------------------------------------------------------
//
//	�I�u�W�F�N�g���I���W�i�����ǂ���
//
//-----------------------------------------------------------------------------------------
bool Object2D::IsOriginal() const
{
	return !m_isCloned;
}

//-----------------------------------------------------------------------------------------
//
//	��ʓ��ɕ`�悳��Ă��邩�ǂ���
//
//-----------------------------------------------------------------------------------------
bool Object2D::IsInScreen() const
{
	auto isInScreen = false;

	DX_System* pSystem = DX_System::GetInstance();

	float height = DX::CAST::F(pSystem->GetScreenHeight());
	float width = DX::CAST::F(pSystem->GetScreenWidth());

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
void Object2D::SetRectPos(const DX::tagRect& rect)
{
	m_rectPos = rect;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	�`����W��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void Object2D::SetRectPos(const float left, const float top, const float right, const float bottom)
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
void Object2D::SetUV(const DX::tagRect& uv)
{
	m_uv = uv;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	UV���W��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void Object2D::SetUV(const float left, const float top, const float right, const float bottom)
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
DX::tagRect  Object2D::GetRectPos() const
{
	return m_rectPos;
}

//-----------------------------------------------------------------------------------------
//
//	UV���W���擾����
//
//-----------------------------------------------------------------------------------------
DX::tagRect  Object2D::GetUV() const
{
	return m_uv;
}

//-----------------------------------------------------------------------------------------
//
//	���_�����X�V����
//
//-----------------------------------------------------------------------------------------
void Object2D::Update(const bool isLRMirror, const bool isUDMirror)
{
	if (m_isChanged || m_isLRMirror != isLRMirror || m_isUDMirror != isUDMirror)
	{
		ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

		//	���_�����쐬
		CreateVertex(m_rectPos, m_uv, isLRMirror, isUDMirror);

		m_isChanged = false;
		m_isLRMirror = isLRMirror;
		m_isUDMirror = isUDMirror;
	}
}

void Object2D::SetCalcRectVertex(DX::tagVertex2D** rectVertices, DX::tagRect& rectPos)
{
	DX_System* system = DX_System::GetInstance();
	ID3D11DeviceContext* deviceContext = system->GetDeviceContext();

	float windowWidth	= static_cast<float>(system->GetScreenWidth());
	float windowHeight	= static_cast<float>(system->GetScreenHeight());

	//	-1.0f ~ 1.0f�ɍ��W�𐳋K������
	DirectX::XMFLOAT2 center(1.0f / (windowWidth * 0.5f), 1.0f / (windowHeight * 0.5f));

	DX::tagRect resultRect;
	resultRect.left		= center.x * rectPos.x - 1.0f;
	resultRect.right	= center.x * rectPos.w - 1.0f;
	resultRect.bottom	= 1.0f - center.y * rectPos.h;
	resultRect.top		= 1.0f - center.y * rectPos.y;


	//	��
	rectVertices[0]->pos.x = resultRect.left;
	rectVertices[1]->pos.x = resultRect.left;

	//	���̍��W
	rectVertices[0]->pos.y = resultRect.bottom;
	rectVertices[2]->pos.y = resultRect.bottom;

	//	��̍��W
	rectVertices[1]->pos.y = resultRect.top;
	rectVertices[3]->pos.y = resultRect.top;

	//	�E�̍��W
	rectVertices[2]->pos.x = resultRect.right;
	rectVertices[3]->pos.x = resultRect.right;

	//	z�l
	rectVertices[0]->pos.z = 0.0f;
	rectVertices[1]->pos.z = 0.0f;
	rectVertices[2]->pos.z = 0.0f;
	rectVertices[3]->pos.z = 0.0f;
}
//-----------------------------------------------------------------------------------------
//
//  ���_�����쐬����
//
//-----------------------------------------------------------------------------------------
void Object2D::CreateVertex(const DX::tagRect& rectPos, const DX::tagRect& uv, const bool isLRMirror, const bool isUDMirror)
{
	DX_System* pSystem = DX_System::GetInstance();
	ID3D11DeviceContext* pContext = pSystem->GetDeviceContext();

	float windowWidth = DX::CAST::F(pSystem->GetScreenWidth());
	float windowHeight = DX::CAST::F(pSystem->GetScreenHeight());

	//	-1.0f ~ 1.0f�ɍ��W�𐳋K������
	DirectX::XMFLOAT2 center(1.0f / (windowWidth * 0.5f), 1.0f / (windowHeight * 0.5f));

	DX::tagRect norRectPos;
	norRectPos.left = center.x * rectPos.x - 1.0f;
	norRectPos.right = center.x * rectPos.w - 1.0f;
	norRectPos.bottom = 1.0f - center.y * rectPos.h;
	norRectPos.top = 1.0f - center.y * rectPos.y;



	//	UV���W��0.0f ~ 1.0f�ɐ��K������
	DirectX::XMFLOAT2 centerUV(1.0f / DX::CAST::F(m_width), 1.0f / DX::CAST::F(m_height));

	DX::tagRect norUV;

	//	�㉺���E���]���邩�ǂ���
	if (isLRMirror && isUDMirror) {
		norUV.left = centerUV.x * uv.w;
		norUV.top = centerUV.y * uv.h;
		norUV.right = centerUV.x * uv.x;
		norUV.bottom = centerUV.y * uv.y;
	}
	else {

		// ���E���]���邩�ǂ���
		if (isLRMirror) {
			norUV.left = centerUV.x * uv.w;
			norUV.top = centerUV.y * uv.y;
			norUV.right = centerUV.x * uv.x;
			norUV.bottom = centerUV.y * uv.h;
		}
		//	�㉺���]���邩�ǂ���
		else if (isUDMirror) {
			norUV.left = centerUV.x * uv.x;
			norUV.top = centerUV.y * uv.h;
			norUV.right = centerUV.x * uv.w;
			norUV.bottom = centerUV.y * uv.y;
		}
		//	�Ȃ�����Ȃ��ꍇ
		else {
			norUV.left = centerUV.x * uv.x;
			norUV.top = centerUV.y * uv.y;
			norUV.right = centerUV.x * uv.w;
			norUV.bottom = centerUV.y * uv.h;
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
	pContext->UpdateSubresource(m_vertexBuffer.Get(), 0, nullptr, pVertices, 0, 0);
}