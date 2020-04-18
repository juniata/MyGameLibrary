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
bool DX_Lighting::SetLightVertexShader()
{
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	���[�J���ϐ�
	ID3D11Buffer*				pBuffer = nullptr;
	D3D11_BUFFER_DESC			bufferDesc = { NULL };

	//	���_�V�F�[�_�[�ɑ�����
	tagVertexLighting vertexLighting;
	vertexLighting.ambient	= m_ambient;
	vertexLighting.diffuse	= m_diffuse;
	vertexLighting.viewPos	= m_viewPos;
	vertexLighting.lightDir	= m_lightDir;
	vertexLighting.lightPos	= m_lightPos;

	//	16byte alignment check
	if (false == DebugValueCheck((sizeof(vertexLighting) % 16 == 0), "16�Ŋ���؂�܂���")) {
		return false;
	}

	//	�萔�o�b�t�@���쐬
	bufferDesc.ByteWidth = sizeof(tagVertexLighting);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//	buffer���쐬
	pBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(vertexLighting));
	if (false == DebugValueCheck(pBuffer, "�萔�o�b�t�@�̍쐬�Ɏ��s���܂����B")) {
		return false;
	}

	//	updateSubResource
	pContext->UpdateSubresource(pBuffer, 0, nullptr, &vertexLighting, 0, 0);

	//	PS�ɑ���
	bool isSucceed = DX_ResourceManager::SetConstantbuffers(pContext, 3, 1, &pBuffer, DX_SHADER_TYPE::VERTEX_SHADER);

	SAFE_RELEASE(pBuffer);

	return isSucceed;
}


//-----------------------------------------------------------------------------------------
//
//  �s�N�Z���V�F�[�_�[�ōs�����C�g����ݒ�
//
//-----------------------------------------------------------------------------------------
bool DX_Lighting::SetLightPixelShader()
{
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	���[�J���ϐ�
	ID3D11Buffer*				pBuffer = nullptr;
	D3D11_BUFFER_DESC			bufferDesc = { NULL };

	//	���_�V�F�[�_�[�ɑ�����
	tagPixelLighting pixelLighting;
	pixelLighting.specular	= m_specular;
	pixelLighting.viewPos		= m_viewPos;
	pixelLighting.lightDir	= m_lightDir;

	//	16byte alignment check
	if (false == DebugValueCheck((sizeof(pixelLighting) % 16 == 0), "16�Ŋ���؂�܂���B")) {
		return false;
	}

	//	�萔�o�b�t�@���쐬
	bufferDesc.ByteWidth = sizeof(tagPixelLighting);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//	buffer���쐬
	pBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(pixelLighting));
	if (false == DebugValueCheck(pBuffer, "�萔�o�b�t�@�̍쐬�Ɏ��s���܂����B")) {
		return false;
	}

	//	updateSubResource
	pContext->UpdateSubresource(pBuffer, 0, nullptr, &pixelLighting, 0, 0);

	//	PS�ɑ���
	bool isSucceed = DX_ResourceManager::SetConstantbuffers(pContext, 0, 1, &pBuffer, DX_SHADER_TYPE::PIXEL_SHADER);

	SAFE_RELEASE(pBuffer);

	return isSucceed;
}