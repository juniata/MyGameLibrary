#include "SkyLibrary.h"

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
tagLighting Lighting::m_lightInfo;

//-----------------------------------------------------------------------------------------
//
//  メンバ変数初期化
//
//-----------------------------------------------------------------------------------------
void Lighting::Initialize()
{
#if defined(DEBUG) || defined(_DEBUG)
	//	16byte alignment check
	bool is16ByteAlignment = sizeof(m_lightInfo) % 16 != 0;
	if (is16ByteAlignment) {
		TRACE("16で割り切れません")
	}
#endif

	ZeroMemory(&m_lightInfo, sizeof(m_lightInfo));

	//	環境光を設定
	SetAmbient(DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f));
	//SetAmbient(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	拡散光を設定
	SetDiffuse(DirectX::XMFLOAT3(0.8f, 0.6f, 0.7f));

	//	反射光を設定
	SetSpecular(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	ライトの座標を設定
	SetLightPos(DirectX::XMFLOAT3(40.0f, 200.0, 0.0f));

	//	ライトをシェーダーに送る
	SetLightVertexShader();
	SetLightPixelShader();
}

//-----------------------------------------------------------------------------------------
//
//  環境光を設定
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
//  拡散光を設定
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
//  反射光を設定
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
//	ライトの座標を設定
//
//-----------------------------------------------------------------------------------------
void Lighting::SetLightPos(const DirectX::XMFLOAT3& pos)
{
	m_lightInfo.lightPos = pos;
}


//-----------------------------------------------------------------------------------------
//
//  頂点シェーダーで行うライト情報を設定
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
			TRACE("定数バッファの作成に失敗しました。")
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
//  ピクセルシェーダーで行うライト情報を設定
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
			TRACE("定数バッファの作成に失敗しました。")
			break;
		}

		pContext->UpdateSubresource(buffer.Get(), 0, nullptr, &m_lightInfo, 0, 0);

		DX_ShaderManager::GetInstance()->SetConstantBuffers(2, 1, buffer.GetAddressOf(), SHADER_TYPE::PIXEL_SHADER);

		succeed = true;
	} while (false);

	return succeed;
}