#include	"DX_Library.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  �����o�[�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_2DObject::DX_2DObject() :

//	�e�N�X�`����������
m_pShaderResourceView(nullptr),

//	�e�N�X�`���T�C�Y��������
m_height(0),
m_width(0)
{}


//-----------------------------------------------------------------------------------------
//
//  �����o�[�ϐ������������A2DObject���쐬����
//
//-----------------------------------------------------------------------------------------
DX_2DObject::DX_2DObject(const char* pFilepath) : DX_2DObject()
{
	LoadTexture(pFilepath);
}


//-----------------------------------------------------------------------------------------
//
//  m_pShaderResourceView�����
//
//-----------------------------------------------------------------------------------------
DX_2DObject::~DX_2DObject()
{
	//	�e�N�X�`�����������
	DX_TextureManager::Release(m_pShaderResourceView);
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
void DX_2DObject::Render()
{
	tagVertex2D l_pVertex[] = {
		/* ���� */	XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
		/* ���� */	XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f),
		/* �E�� */	XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),
		/* �E�� */	XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)
	};

	//	�o�b�t�@���쐬
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(),sizeof(tagVertex2D)* 4, l_pVertex);
	
	//	�V�F�[�_�[���擾
	DX_Shader* l_pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader  = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�f�o�C�X�R���e�L�X�g���擾
	auto	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	�V�F�[�_�[�𗘗p
	l_pVertexShader->Begin(l_deviceContext);
	l_pPixelShader->Begin(l_deviceContext);

	//	�`��
	DX_Buffer::Render2D(l_buffer.Get(), m_pShaderResourceView);
	//	�V�F�[�_�[���I��
	l_pVertexShader->End(l_deviceContext);
	l_pPixelShader->End(l_deviceContext);
}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���͈͂ɕ`��
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(const tagRect& renderPos)
{
	//	���_���
	tagVertex2D l_pVertex[4];

	//	���_�����쐬
	CreateVertex(l_pVertex, renderPos, tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	�o�b�t�@���쐬
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(),sizeof(tagVertex2D)* 4, l_pVertex);

	//	�V�F�[�_�[���擾
	DX_Shader* l_pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader  = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�f�o�C�X�R���e�L�X�g���擾
	ComPtr<ID3D11DeviceContext>	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	�V�F�[�_�[�𗘗p
	l_pVertexShader->Begin(l_deviceContext.Get());
	l_pPixelShader->Begin(l_deviceContext.Get());

	//	�`��
	DX_Buffer::Render2D(l_buffer.Get(), m_pShaderResourceView);

	//	�V�F�[�_�[���I��
	l_pVertexShader->End(l_deviceContext.Get());
	l_pPixelShader->End(l_deviceContext.Get());

}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���͈͂ɕ`��
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(DirectX::XMFLOAT2 renderPos, DirectX::XMFLOAT2 renderSize)
{
	//	���_���
	tagVertex2D l_pVertex[4];

	//	���_�����쐬
	CreateVertex(l_pVertex, tagRect(renderPos.x, renderPos.y, (renderSize.x + renderPos.x), (renderSize.y + renderPos.y)), tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	�o�b�t�@���쐬
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(tagVertex2D) * 4, l_pVertex);

	//	�V�F�[�_�[���擾
	DX_Shader* l_pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�f�o�C�X�R���e�L�X�g���擾
	ComPtr<ID3D11DeviceContext>	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	�V�F�[�_�[�𗘗p
	l_pVertexShader->Begin(l_deviceContext.Get());
	l_pPixelShader->Begin(l_deviceContext.Get());

	//	�`��
	DX_Buffer::Render2D(l_buffer.Get(), m_pShaderResourceView);

	//	�V�F�[�_�[���I��
	l_pVertexShader->End(l_deviceContext.Get());
	l_pPixelShader->End(l_deviceContext.Get());

}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���͈͂ɕ`��
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(const tagRect& renderPos, const tagRect& texturePos)
{
	//	���_���
	tagVertex2D l_pVertex[4];

	//	���_�����쐬
	CreateVertex(l_pVertex, renderPos, tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	�o�b�t�@���쐬
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(tagVertex2D) * 4, l_pVertex);

	//	�V�F�[�_�[���擾
	DX_Shader* l_pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader  = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	�f�o�C�X�R���e�L�X�g���擾
	ComPtr<ID3D11DeviceContext>	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	�V�F�[�_�[�𗘗p
	l_pVertexShader->Begin(l_deviceContext.Get());
	l_pPixelShader->Begin(l_deviceContext.Get());

	//	�`��
	DX_Buffer::Render2D(l_buffer.Get(), m_pShaderResourceView);

	//	�V�F�[�_�[���I��
	l_pVertexShader->End(l_deviceContext.Get());
	l_pPixelShader->End(l_deviceContext.Get());

}

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`����ǂݍ���
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::LoadTexture(
	const char* pFilepath
	)
{
	//	�e�N�X�`�����擾
	m_pShaderResourceView = DX_TextureManager::GetTexture(pFilepath);

	//	�e�N�X�`�������[�h�ł��Ă邩�`�F�b�N
	DEBUG_VALUE_CHECK(m_pShaderResourceView, "�e�N�X�`���̃t�@�C���p�X���Ԉ���Ă��܂�");

	//	�e�N�X�`�����ׂ��ȏ����擾
	ComPtr<ID3D11Resource> l_resource;
	m_pShaderResourceView->GetResource(&l_resource);

	//ComPtr<ID3D11Texture2D> l_texture2D;
	//l_resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&aa);
	ID3D11Texture2D* l_texture2D = (ID3D11Texture2D*)l_resource.Get();
	D3D11_TEXTURE2D_DESC l_texDesc;
	l_texture2D->GetDesc(&l_texDesc);

	//	�e�N�X�`���T�C�Y���擾
	m_height = l_texDesc.Height;
	m_width = l_texDesc.Width;
}


//-----------------------------------------------------------------------------------------
//
//  ���_�����쐬����
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::CreateVertex(tagVertex2D* pVertex, const tagRect& renderPos, const tagRect& texturePos)
{
	//	1 ~ 0�̒l�ɕϊ�
	const float l_centerX = 1.0f / (CAST_F(DX_System::GetWindowWidth()) * 0.5f);
	const float l_centerY = 1.0f / (CAST_F(DX_System::GetWindowHeight()) * 0.5f);

	//	0~1�̊����̗��Ƃ�����
	const float l_centerUv_X = (1.0f / m_width);
	const float l_centerUv_Y = (1.0f / m_height);


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


	//	���̍��W
	pVertex[1].uv.x = pVertex[0].uv.x = l_centerUv_X * texturePos.x;

	//	��̍��W
	pVertex[3].uv.y = pVertex[1].uv.y = l_centerUv_Y * texturePos.y;

	//	�E�̍��W
	pVertex[3].uv.x = pVertex[2].uv.x = l_centerUv_X * texturePos.w;

	//	���̍��W
	pVertex[2].uv.y = pVertex[0].uv.y = l_centerUv_Y * texturePos.h;

}