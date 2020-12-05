#include "SkyLibrary.h"

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
tagLighting Lighting::m_lightInfo;

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ�������
//
//-----------------------------------------------------------------------------------------
void Lighting::Initialize()
{
#if defined(DEBUG) || defined(_DEBUG)
	//	16byte alignment check
	bool is16ByteAlignment = sizeof(m_lightInfo) % 16 != 0;
	if (is16ByteAlignment) {
		TRACE("16�Ŋ���؂�܂���")
	}
#endif

	ZeroMemory(&m_lightInfo, sizeof(m_lightInfo));

	//	������ݒ�
	SetAmbient(DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f));
	//SetAmbient(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	�g�U����ݒ�
	SetDiffuse(DirectX::XMFLOAT3(0.8f, 0.6f, 0.7f));

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
	m_lightInfo.ambient.x = ambient.x;
	m_lightInfo.ambient.y = ambient.y;
	m_lightInfo.ambient.z = ambient.z;
	m_lightInfo.ambient.w = 1.0f;
}

//-----------------------------------------------------------------------------------------
//
//  �g�U����ݒ�
//
//-----------------------------------------------------------------------------------------
void Lighting::SetDiffuse(const DirectX::XMFLOAT3& diffuse)
{
	m_lightInfo.diffuse.x = diffuse.x;
	m_lightInfo.diffuse.y = diffuse.y;
	m_lightInfo.diffuse.z = diffuse.z;
	m_lightInfo.diffuse.w = 1.0f;
}

//-----------------------------------------------------------------------------------------
//
//  ���ˌ���ݒ�
//
//-----------------------------------------------------------------------------------------
void Lighting::SetSpecular(const DirectX::XMFLOAT3& specular)
{
	/*m_lightInfo.specular.x = specular.x;
	m_lightInfo.specular.y = specular.y;
	m_lightInfo.specular.z = specular.z;
	m_lightInfo.specular.w = 1.0f;*/
}


//-----------------------------------------------------------------------------------------
//
//	���C�g�̍��W��ݒ�
//
//-----------------------------------------------------------------------------------------
void Lighting::SetLightPos(const DirectX::XMFLOAT3& pos)
{
	m_lightInfo.lightPos = pos;
}


//-----------------------------------------------------------------------------------------
//
//  ���_�V�F�[�_�[�ōs�����C�g����ݒ�
//
//-----------------------------------------------------------------------------------------
bool Lighting::SetLightVertexShader()
{
	auto succeed = false;

	do
	{
		ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

		buffer.Attach(DX_BufferCreater::ConstantBuffer(sizeof(m_lightInfo)));
		if (buffer.Get() == nullptr) {
			TRACE("�萔�o�b�t�@�̍쐬�Ɏ��s���܂����B")
			break;
		}

		pContext->UpdateSubresource(buffer.Get(), 0, nullptr, &m_lightInfo, 0, 0);

		DX_ShaderManager::GetInstance()->SetConstantBuffers(2, 1, buffer.GetAddressOf(), SHADER_TYPE::VERTEX_SHADER);

		succeed = true;

	} while (false);

	return succeed;
}


//-----------------------------------------------------------------------------------------
//
//  �s�N�Z���V�F�[�_�[�ōs�����C�g����ݒ�
//
//-----------------------------------------------------------------------------------------
bool Lighting::SetLightPixelShader()
{
	auto succeed = false;

	do
	{
		ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

		buffer.Attach(DX_BufferCreater::ConstantBuffer(sizeof(m_lightInfo)));
		if (buffer.Get() == nullptr) {
			TRACE("�萔�o�b�t�@�̍쐬�Ɏ��s���܂����B")
			break;
		}

		pContext->UpdateSubresource(buffer.Get(), 0, nullptr, &m_lightInfo, 0, 0);

		DX_ShaderManager::GetInstance()->SetConstantBuffers(2, 1, buffer.GetAddressOf(), SHADER_TYPE::PIXEL_SHADER);

		succeed = true;
	} while (false);

	return succeed;
}