#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT4 DX_Lighting::m_ambient;
DirectX::XMFLOAT4 DX_Lighting::m_diffuse;
DirectX::XMFLOAT4 DX_Lighting::m_specular;
DirectX::XMFLOAT3 DX_Lighting::m_viewPos;
DirectX::XMFLOAT3 DX_Lighting::m_lightPos;
DirectX::XMFLOAT3 DX_Lighting::m_lightDir;

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ�������
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::Initialize()
{
	//	������ݒ�
	SetAmbient(DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f));

	//	�g�U����ݒ�
	SetDiffuse(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	���ˌ���ݒ�
	SetSpecular(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	���C�g�̍��W��ݒ�
	SetLightPos(DirectX::XMFLOAT3(40.0f, 200.0, 0.0f));

	//	���C�g�̌�����ݒ�
	SetLightDir(DirectX::XMFLOAT3(0.0f,-1.0f,1.0f));

	//	���_��ݒ�
	SetViewPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

	//	���C�g���V�F�[�_�[�ɑ���
	SetLightVertexShader();
	SetLightPixelShader();
}

//-----------------------------------------------------------------------------------------
//
//  ������ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetAmbient(const DirectX::XMFLOAT3& ambient)
{
	m_ambient.x = ambient.x;
	m_ambient.y = ambient.y;
	m_ambient.z = ambient.z;
	m_ambient.w = 1.0f;
}

//-----------------------------------------------------------------------------------------
//
//  �g�U����ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetDiffuse(const DirectX::XMFLOAT3& diffuse)
{
	m_diffuse.x = diffuse.x;
	m_diffuse.y = diffuse.y;
	m_diffuse.z = diffuse.z;
	m_diffuse.w = 1.0f;
}

//-----------------------------------------------------------------------------------------
//
//  ���ˌ���ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetSpecular(const DirectX::XMFLOAT3& specular)
{
	m_specular.x = specular.x;
	m_specular.y = specular.y;
	m_specular.z = specular.z;
	m_specular.w = 1.0f;

}


//-----------------------------------------------------------------------------------------
//
//	���C�g�̍��W��ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetLightPos(const DirectX::XMFLOAT3& pos)
{
	m_lightPos = pos;
}

//-----------------------------------------------------------------------------------------
//
//  ���C�g�̕�����ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetLightDir(const DirectX::XMFLOAT3& dir)
{
	m_lightDir = dir;
}


//-----------------------------------------------------------------------------------------
//
//  ���_��ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetViewPos(const DirectX::XMFLOAT3& pos)
{
	m_viewPos = pos;
}

//-----------------------------------------------------------------------------------------
//
//  ���_�V�F�[�_�[�ōs�����C�g����ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetLightVertexShader()
{
	auto	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	���[�J���ϐ�
	ComPtr<ID3D11Buffer>		l_buffer;
	D3D11_BUFFER_DESC			l_bufferDesc = { NULL };
	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };

	//	���_�V�F�[�_�[�ɑ�����
	tagVertexLighting l_vertexLighting;
	l_vertexLighting.ambient	= m_ambient;
	l_vertexLighting.diffuse	= m_diffuse;
	l_vertexLighting.viewPos	= m_viewPos;
	l_vertexLighting.lightDir	= m_lightDir;
	l_vertexLighting.lightPos	= m_lightPos;

	//	16byte alignment check
	DEBUG_VALUE_CHECK((sizeof(l_vertexLighting) % 16 == 0) ? true : false, "16�Ŋ���؂�܂���");

	//	�萔�o�b�t�@���쐬
	l_bufferDesc.ByteWidth		= sizeof(tagVertexLighting);
	l_bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	l_bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	l_bufferDesc.CPUAccessFlags = 0;

	//	buffer���쐬
	DX_System::GetInstance()->GetDevice()->CreateBuffer(&l_bufferDesc, nullptr, &l_buffer);

	//	updateSubResource
	l_deviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, &l_vertexLighting, 0, 0);

	//	PS�ɑ���
	DX_ResourceManager::SetConstantbuffers(l_deviceContext, 3, 1, &l_buffer, DX_SHADER_TYPE::VERTEX_SHADER);
}


//-----------------------------------------------------------------------------------------
//
//  �s�N�Z���V�F�[�_�[�ōs�����C�g����ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetLightPixelShader()
{
	auto l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	���[�J���ϐ�
	ComPtr<ID3D11Buffer>		l_buffer;
	D3D11_BUFFER_DESC			l_bufferDesc = { NULL };
	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };

	//	���_�V�F�[�_�[�ɑ�����
	tagPixelLighting l_pixelLighting;
	l_pixelLighting.specular	= m_specular;
	l_pixelLighting.viewPos		= m_viewPos;
	l_pixelLighting.lightDir	= m_lightDir;

	//	16byte alignment check
	DEBUG_VALUE_CHECK((sizeof(l_pixelLighting) % 16 == 0) ? true : false, "16�Ŋ���؂�܂���");

	//	�萔�o�b�t�@���쐬
	l_bufferDesc.ByteWidth		= sizeof(tagPixelLighting);
	l_bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	l_bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	l_bufferDesc.CPUAccessFlags = 0;

	//	buffer���쐬
	DX_System::GetInstance()->GetDevice()->CreateBuffer(&l_bufferDesc, nullptr, &l_buffer);

	//	updateSubResource
	l_deviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, &l_pixelLighting, 0, 0);

	//	PS�ɑ���
	DX_ResourceManager::SetConstantbuffers(l_deviceContext, 0, 1, &l_buffer, DX_SHADER_TYPE::PIXEL_SHADER);

}