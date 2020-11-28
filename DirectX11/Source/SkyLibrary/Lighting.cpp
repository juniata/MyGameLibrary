#include "SkyLibrary.h"

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT4 Lighting::m_ambient;
DirectX::XMFLOAT4 Lighting::m_diffuse;
DirectX::XMFLOAT4 Lighting::m_specular;
DirectX::XMFLOAT3 Lighting::m_lightPos;

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ�������
//
//-----------------------------------------------------------------------------------------
void Lighting::Initialize()
{
	//	������ݒ�
	SetAmbient(DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f));
	//SetAmbient(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	�g�U����ݒ�
	SetDiffuse(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	���ˌ���ݒ�
	SetSpecular(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	���C�g�̍��W��ݒ�
	SetLightPos(DirectX::XMFLOAT3(40.0f, 200.0, 0.0f));

	//	���C�g���V�F�[�_�[�ɑ���
	SetLightVertexShader();
	SetLightPixelShader();
}

//-----------------------------------------------------------------------------------------
//
//  ������ݒ�
//
//-----------------------------------------------------------------------------------------
void Lighting::SetAmbient(const DirectX::XMFLOAT3& ambient)
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
void Lighting::SetDiffuse(const DirectX::XMFLOAT3& diffuse)
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
void Lighting::SetSpecular(const DirectX::XMFLOAT3& specular)
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
void Lighting::SetLightPos(const DirectX::XMFLOAT3& pos)
{
	m_lightPos = pos;
}


//-----------------------------------------------------------------------------------------
//
//  ���_�V�F�[�_�[�ōs�����C�g����ݒ�
//
//-----------------------------------------------------------------------------------------
bool Lighting::SetLightVertexShader()
{
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	���[�J���ϐ�
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC			bufferDesc = { NULL };

	//	���_�V�F�[�_�[�ɑ�����
	tagVertexLighting vertexLighting;
	vertexLighting.ambient	= m_ambient;
	vertexLighting.diffuse	= m_diffuse;
	vertexLighting.lightPos = m_lightPos;

#if defined(DEBUG) || defined(_DEBUG)
	//	16byte alignment check
	bool is16ByteAlignment = sizeof(vertexLighting) % 16 != 0;
	if (is16ByteAlignment) {
		TRACE("16�Ŋ���؂�܂���")
		return false;
	}
#endif

	//	�萔�o�b�t�@���쐬
	bufferDesc.ByteWidth = sizeof(tagVertexLighting);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//	buffer���쐬
	buffer = DX_BufferCreater::ConstantBuffer(sizeof(vertexLighting));
	if (buffer.Get() == nullptr) {
		TRACE("�萔�o�b�t�@�̍쐬�Ɏ��s���܂����B")
		return false;
	}

	//	updateSubResource
	pContext->UpdateSubresource(buffer.Get(), 0, nullptr, &vertexLighting, 0, 0);

	//	PS�ɑ���
	bool isSucceed = true;
	DX_ShaderManager::GetInstance()->SetConstantBuffers(3, 1, &buffer, SHADER_TYPE::VERTEX_SHADER);

	return isSucceed;
}


//-----------------------------------------------------------------------------------------
//
//  �s�N�Z���V�F�[�_�[�ōs�����C�g����ݒ�
//
//-----------------------------------------------------------------------------------------
bool Lighting::SetLightPixelShader()
{
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	���[�J���ϐ�
	Microsoft::WRL::ComPtr<ID3D11Buffer>	buffer;
	D3D11_BUFFER_DESC						bufferDesc = { NULL };

	//	���_�V�F�[�_�[�ɑ�����
	tagPixelLighting pixelLighting;
	pixelLighting.ambient = m_ambient;
	pixelLighting.diffuse = m_diffuse;
	pixelLighting.lightPos = m_lightPos;

#if defined(DEBUG) || defined(_DEBUG)
	//	16byte alignment check
	bool is16ByteAlignment = sizeof(pixelLighting) % 16 != 0;
	if  (is16ByteAlignment) {
		TRACE("16�Ŋ���؂�܂���")
		return false;
	}
#endif

	//	�萔�o�b�t�@���쐬
	bufferDesc.ByteWidth = sizeof(tagPixelLighting);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//	buffer���쐬
	if (!(buffer = DX_BufferCreater::ConstantBuffer(sizeof(pixelLighting)))) {
		TRACE("�萔�o�b�t�@�̍쐬�Ɏ��s���܂����B")
		return false;
	}

	//	updateSubResource
	pContext->UpdateSubresource(buffer.Get(), 0, nullptr, &pixelLighting, 0, 0);

	//	PS�ɑ���
	bool isSucceed = true;
	DX_ShaderManager::GetInstance()->SetConstantBuffers(0, 1, buffer.GetAddressOf(), SHADER_TYPE::PIXEL_SHADER);

	return isSucceed;
}