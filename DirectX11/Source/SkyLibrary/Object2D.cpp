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
	m_pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
	m_scale(DirectX::XMFLOAT2(1.0f,1.0f)),
	m_angle(0.0f)
{
	ZeroMemory(&m_rect, sizeof(m_rect));
	ZeroMemory(&m_uv, sizeof(m_uv));
}

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

	SetRect(DX::tagRect(0.0f, 0.0f, DX::CAST::F(m_system->GetScreenWidth()), DX::CAST::F(m_system->GetScreenHeight())));
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

	SetRect(rectPos);
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

	SetRect(rectPos);
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

	ID3D11DeviceContext* deviceContext = m_system->GetDeviceContext();

	DX_Shader* vertexShader = m_shaderManager->GetShader(SHADER_OBJECT_2D::VERTEX_SHADER);
	DX_Shader* pixelShader = m_shaderManager->GetShader(SHADER_OBJECT_2D::PIXEL_SHADER);

	vertexShader->Begin();
	pixelShader->Begin();

	unsigned int stride = sizeof(DX::tagVertex2D);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetInputLayout(m_shaderManager->GetInputLayout2D());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	m_shaderManager->SetShaderResources(0, 1, m_srv.GetAddressOf(), SHADER_TYPE::PIXEL_SHADER);

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
		m_srv = m_textureManager->GetTexture(filepath);

		if (nullptr == m_srv.Get()) {
			TRACE("failed to DX_TextureManager::GetTexture(), filepath = %s", filepath);
			break;
		}

		m_textureManager->GetTextureSize(&m_width, &m_height, m_srv.Get());

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

	float height = DX::CAST::F(m_system->GetScreenHeight());
	float width = DX::CAST::F(m_system->GetScreenWidth());

	if (0.0f <= m_rect.y && m_rect.bottom <= height && 0.0f <= m_rect.x && m_rect.right <= width)
	{
		isInScreen = true;
	}

	return isInScreen;
}


/// <summary>
/// �`�悷��I�u�W�F�N�g�̃T�C�Y��ݒ肷��
/// </summary>
/// <param name="rect">�`����W</param>
void Object2D::SetRect(const DX::tagRect& rect)
{
	m_rect = rect;
	m_isChanged = true;
}

/// <summary>
/// ���W��ݒ�
/// </summary>
/// <param name="pos">���W</param>
void Object2D::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_isChanged = true;
}

/// <summary>
/// �傫����ݒ肷��(�`�悷��I�u�W�F�N�g�̑傫���ɑ΂��Ċ|���Z����)
/// </summary>
/// <param name="scale"></param>
void Object2D::SetScale(const DirectX::XMFLOAT2& scale)
{
	m_scale = scale;
	m_isChanged = true;
}

/// <summary>
/// �摜�̌����̐ݒ肷��
/// </summary>
/// <param name="val">������ݒ�</param>
/// <param name="isAngle">false�Ȃ烉�W�A���Atrue�Ȃ�x���Őݒ�</param>
void Object2D::SetAngle(const float val, bool isAngle)
{
	if (isAngle)
	{
		m_angle = val * (DirectX::XM_PI / 180.0f);
	}
	else
	{
		m_angle = val;
	}
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
	return m_rect;
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
void Object2D::Update()
{
	if (m_isChanged)
	{
		UpdateVertexBuffer();

		m_isChanged = false;
	}
}

/// <summary>
/// ���_�o�b�t�@���X�V����
/// </summary>
void Object2D::UpdateVertexBuffer()
{
	ID3D11DeviceContext* context = m_system->GetDeviceContext();

	float windowWidth	= DX::CAST::F(m_system->GetScreenWidth());
	float windowHeight	= DX::CAST::F(m_system->GetScreenHeight());

	//	-1.0f ~ 1.0f�ɍ��W�𐳋K������
	DirectX::XMFLOAT2 center(1.0f / (windowWidth * 0.5f), 1.0f / (windowHeight * 0.5f));

	// ���K���������W
	DX::tagRect norRectPos = 
	{
		center.x * m_rect.x - 1.0f,
		1.0f - center.y * m_rect.y,
		center.x * m_rect.w - 1.0f,
		1.0f - center.y * m_rect.h
	};
	
	//	UV���W��0.0f ~ 1.0f�ɐ��K������
	DirectX::XMFLOAT2 centerUV(1.0f / DX::CAST::F(m_width), 1.0f / DX::CAST::F(m_height));

	// ���K������UV���W
	DX::tagRect norUV =
	{
		centerUV.x * m_uv.left,
		centerUV.y * m_uv.top,
		centerUV.x * m_uv.right,
		centerUV.y * m_uv.bottom
	};

	DX::tagVertex2D vertices[] =
	{
		// ����
		{DirectX::XMFLOAT3(norRectPos.left, norRectPos.bottom, 0.0f), DirectX::XMFLOAT2(norUV.left, norUV.bottom) },

		// ����
		{DirectX::XMFLOAT3(norRectPos.left, norRectPos.top, 0.0f), DirectX::XMFLOAT2(norUV.left, norUV.top) },
		
		// �E��
		{DirectX::XMFLOAT3(norRectPos.right, norRectPos.bottom, 0.0f), DirectX::XMFLOAT2(norUV.right, norUV.bottom) },
		
		// �E��
		{DirectX::XMFLOAT3(norRectPos.right, norRectPos.top, 0.0f), DirectX::XMFLOAT2(norUV.right, norUV.top) }
	};

	const float originX = 0.5f * (vertices[0].pos.x + vertices[3].pos.x);
	const float originY = 0.5f * (vertices[0].pos.y + vertices[1].pos.y);
	DirectX::XMVECTOR scalling		= DirectX::XMVectorSet(m_scale.x, m_scale.y, 1.0f, 0.0f);
	DirectX::XMVECTOR origin		= DirectX::XMVectorSet(originX, originY, 0.0f, 0.0f);
	DirectX::XMVECTOR translation	= DirectX::XMVectorSet(m_pos.x * (2.0f / windowWidth), -m_pos.y * (2.0f / windowHeight), m_pos.z, 1.0f);
	DirectX::XMMATRIX transform		= DirectX::XMMatrixAffineTransformation2D(scalling, origin, m_angle, translation);
	
	DirectX::XMVECTOR pos;
	for (auto& v : vertices)
	{
		pos = DirectX::XMLoadFloat3(&v.pos);
		pos = DirectX::XMVector3TransformCoord(pos, transform);
		DirectX::XMStoreFloat3(&v.pos, pos);
	}	
	
	// �o�b�t�@�̏㏑��
	context->UpdateSubresource(m_vertexBuffer.Get(), 0, nullptr, vertices, 0, 0);
}