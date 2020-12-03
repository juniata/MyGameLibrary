#include <DxLibrary/DX_Library.h>
#include "GameObject.h"
#include "Object2D.h"
#include	<stdio.h>

/// <summary>
/// �����o�ϐ�����������
/// </summary>
Object2D::Object2D() :
	m_height(0),
	m_width(0),
	m_isCloned(false),
	m_isChanged(false),
	m_isLRMirror(false),
	m_isUDMirror(false)
{}

/// <summary>
/// �����o�ϐ��������
/// </summary>
Object2D::~Object2D()
{
}

/// <summary>
/// �e�N�X�`���̓ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s�� 
/// </summary>
/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X(/Resource/�ȉ�����)</param>
/// <returns>����</returns>
bool Object2D::CommonInitialize(const char* fFilepath)
{
	bool isSucceed = false;

	do
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
			break;
		}

		// �e�N�X�`����ǂݍ���
		char texturePath[MAX_PATH] = { '\n' };
		sprintf_s(texturePath, "%s%s", "Resource\\", fFilepath);

		if (LoadTexture(texturePath) == false)
		{
			TRACE("failed to LoadTexture()")
			break;
		}

		isSucceed = true;
	} while (false);

	return isSucceed;
}

/// <summary>
/// �e�N�X�`���̓ǂݍ��݂ƒ��_�o�b�t�@�̍쐬���s�� 
/// </summary>
/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X(/Resource/�ȉ�����)</param>
/// <returns>����</returns>
bool Object2D::Initialize(const char* filepath)
{
	bool isSucceed = CommonInitialize(filepath);

	DX_System* system = DX_System::GetInstance();
	SetRectPos(DX::tagRect(0.0f, 0.0f, DX::CAST::F(system->GetScreenWidth()), DX::CAST::F(system->GetScreenHeight())));
	SetUV(DX::tagRect(0.0f, 0.0f, DX::CAST::F(m_width), DX::CAST::F(m_height)));
	Update();

	return isSucceed;
}

/// <summary>
/// �w�肵���T�C�Y�Ńe�N�X�`���y�ђ��_�o�b�t�@���쐬����
/// </summary>
/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X(/Resource/�ȉ�����)</param>
/// <param name="rectPos">�`�悷��T�C�Y</param>
/// <returns>����</returns>
bool Object2D::Initialize(const char* filepath, const DX::tagRect& rectPos)
{
	bool isSucceed = CommonInitialize(filepath);

	SetRectPos(rectPos);
	SetUV(DX::tagRect(0.0f, 0.0f, DX::CAST::F(m_width), DX::CAST::F(m_height)));
	Update();

	return isSucceed;
}

/// <summary>
///  �w�肵���T�C�Y��UV�Ńe�N�X�`���y�ђ��_�o�b�t�@���쐬����
/// </summary>
/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X(/Resource/�ȉ�����)</param>
/// <param name="rectPos">�`�悷��T�C�Y</param>
/// <param name="uv">�`�悷��e�N�X�`�����W</param>
/// <returns>����</returns>
bool Object2D::Initialize(const char* filepath, const DX::tagRect& rectPos, const DX::tagRect& uv)
{
	bool isSucceed = CommonInitialize(filepath);

	SetRectPos(rectPos);
	SetUV(uv);
	Update();

	return isSucceed;
}


/// <summary>
/// �e�N�X�`���̍������擾����
/// </summary>
/// <returns>�e�N�X�`���̍���</returns>
unsigned int Object2D::GetHeight()const
{
	return m_height;
}

/// <summary>
/// �e�N�X�`���̕����擾����
/// </summary>
/// <returns>�e�N�X�`���̕�</returns>
unsigned int Object2D::GetWidth()const
{
	return m_width;
}

/// <summary>
/// �`�悷��
/// </summary>
/// <returns>����</returns>
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

/// <summary>
/// �e�N�X�`����ǂݍ���
/// </summary>
/// <param name="filepath"></param>
/// <returns>����</returns>
bool Object2D::LoadTexture(const char* filepath)
{
	auto isSucceed = false;

	do
	{
		m_srv = DX_TextureManager::GetInstance()->GetTexture(filepath);

		if (nullptr == m_srv.Get()) {
			TRACE("failed to DX_TextureManager::GetTexture(), filepath = %s", filepath);
			break;
		}

		DX_TextureManager::GetInstance()->GetTextureSize(&m_height, &m_width, m_srv.Get());

		isSucceed = true;
	} while (false);

	return isSucceed;
}


/// <summary>
/// �e�N�X�`���𕡐�����
/// </summary>
/// <returns>���������I�u�W�F�N�g</returns>
Object2D* Object2D::Clone()
{
	auto pObject = new Object2D(*this);
	pObject->m_isCloned = true;

	return pObject;
}

/// <summary>
	/// �I�u�W�F�N�g�������������̂��ǂ������擾����
	/// </summary>
	/// <returns>�����������̂Ȃ�true</returns>
bool Object2D::IsClone() const
{
	return m_isCloned;
}

/// <summary>
/// �I�u�W�F�N�g���I���W�i�����ǂ������擾����
/// </summary>
/// <returns>�I���W�i���Ȃ�true</returns>
bool Object2D::IsOriginal() const
{
	return (m_isCloned == false);
}

/// <summary>
/// �X�N���[�����ɕ`�悳��Ă��邩�ǂ������擾����
/// </summary>
/// <returns>�`�悳��Ă���Ȃ�true</returns>
bool Object2D::IsInScreen() const
{
	auto isInScreen = false;

	DX_System* system = DX_System::GetInstance();

	float height = DX::CAST::F(system->GetScreenHeight());
	float width = DX::CAST::F(system->GetScreenWidth());

	if (0.0f <= m_rectPos.y && m_rectPos.bottom <= height &&
		0.0f <= m_rectPos.x && m_rectPos.right <= width)
	{
		isInScreen = true;
	}

	return isInScreen;
}


/// <summary>
/// �`����W��ݒ肷��
/// </summary>
/// <param name="rect">�`����W</param>
void Object2D::SetRectPos(const DX::tagRect& rect)
{
	m_rectPos = rect;
	m_isChanged = true;
}

/// <summary>
/// UV���W��ݒ肷��
/// </summary>
/// <param name="uv">UV���W</param>
void Object2D::SetUV(const DX::tagRect& uv)
{
	m_uv = uv;
	m_isChanged = true;
}


/// <summary>
/// �`����W���擾����
/// </summary>
/// <returns>�`����W</returns>
const DX::tagRect& Object2D::GetRectPos()
{
	return m_rectPos;
}

/// <summary>
/// UV���W���擾����
/// </summary>
/// <returns>UV���W</returns>
const DX::tagRect& Object2D::GetUV()
{
	return m_uv;
}

/// <summary>
/// ���W�����X�V����
/// </summary>
/// <param name="isLRMirror">���E���]�`�悷�邩�ǂ���</param>
/// <param name="isUDMirror">�㉺���]�`�悷�邩�ǂ���</param>
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

/// <summary>
/// ���_���W���쐬����
/// </summary>
/// <param name="rectPos">��ʂɕ`�悷��͈�</param>
/// <param name="uv">�`�悷��摜�͈̔�</param>
/// <param name="isLRMirror">���E���]���邩�ǂ���</param>
/// <param name="isUDMirror">�㉺���]���邩�ǂ���</param>
void Object2D::CreateVertex(const DX::tagRect& rectPos, const DX::tagRect& uv, const bool isLRMirror, const bool isUDMirror)
{
	DX_System* system = DX_System::GetInstance();
	ID3D11DeviceContext* context = system->GetDeviceContext();

	float windowWidth = DX::CAST::F(system->GetScreenWidth());
	float windowHeight = DX::CAST::F(system->GetScreenHeight());

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
	context->UpdateSubresource(m_vertexBuffer.Get(), 0, nullptr, pVertices, 0, 0);
}