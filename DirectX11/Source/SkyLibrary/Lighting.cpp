#include "SkyLibrary.h"

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT4 Lighting::m_ambient;
DirectX::XMFLOAT4 Lighting::m_diffuse;
DirectX::XMFLOAT4 Lighting::m_specular;
DirectX::XMFLOAT3 Lighting::m_lightPos;

//-----------------------------------------------------------------------------------------
//
//  メンバ変数初期化
//
//-----------------------------------------------------------------------------------------
void Lighting::Initialize()
{
	//	環境光を設定
	SetAmbient(DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f));
	//SetAmbient(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	拡散光を設定
	SetDiffuse(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

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
	m_ambient.x = ambient.x;
	m_ambient.y = ambient.y;
	m_ambient.z = ambient.z;
	m_ambient.w = 1.0f;
}

//-----------------------------------------------------------------------------------------
//
//  拡散光を設定
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
//  反射光を設定
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
//	ライトの座標を設定
//
//-----------------------------------------------------------------------------------------
void Lighting::SetLightPos(const DirectX::XMFLOAT3& pos)
{
	m_lightPos = pos;
}


//-----------------------------------------------------------------------------------------
//
//  頂点シェーダーで行うライト情報を設定
//
//-----------------------------------------------------------------------------------------
bool Lighting::SetLightVertexShader()
{
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	ローカル変数
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC			bufferDesc = { NULL };

	//	頂点シェーダーに送る情報
	tagVertexLighting vertexLighting;
	vertexLighting.ambient	= m_ambient;
	vertexLighting.diffuse	= m_diffuse;
	vertexLighting.lightPos = m_lightPos;

#if defined(DEBUG) || defined(_DEBUG)
	//	16byte alignment check
	bool is16ByteAlignment = sizeof(vertexLighting) % 16 != 0;
	if (is16ByteAlignment) {
		TRACE("16で割り切れません")
		return false;
	}
#endif

	//	定数バッファを作成
	bufferDesc.ByteWidth = sizeof(tagVertexLighting);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//	bufferを作成
	buffer = DX_BufferCreater::ConstantBuffer(sizeof(vertexLighting));
	if (buffer.Get() == nullptr) {
		TRACE("定数バッファの作成に失敗しました。")
		return false;
	}

	//	updateSubResource
	pContext->UpdateSubresource(buffer.Get(), 0, nullptr, &vertexLighting, 0, 0);

	//	PSに送る
	bool isSucceed = true;
	DX_ShaderManager::GetInstance()->SetConstantBuffers(3, 1, &buffer, SHADER_TYPE::VERTEX_SHADER);

	return isSucceed;
}


//-----------------------------------------------------------------------------------------
//
//  ピクセルシェーダーで行うライト情報を設定
//
//-----------------------------------------------------------------------------------------
bool Lighting::SetLightPixelShader()
{
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	ローカル変数
	Microsoft::WRL::ComPtr<ID3D11Buffer>	buffer;
	D3D11_BUFFER_DESC						bufferDesc = { NULL };

	//	頂点シェーダーに送る情報
	tagPixelLighting pixelLighting;
	pixelLighting.ambient = m_ambient;
	pixelLighting.diffuse = m_diffuse;
	pixelLighting.lightPos = m_lightPos;

#if defined(DEBUG) || defined(_DEBUG)
	//	16byte alignment check
	bool is16ByteAlignment = sizeof(pixelLighting) % 16 != 0;
	if  (is16ByteAlignment) {
		TRACE("16で割り切れません")
		return false;
	}
#endif

	//	定数バッファを作成
	bufferDesc.ByteWidth = sizeof(tagPixelLighting);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//	bufferを作成
	if (!(buffer = DX_BufferCreater::ConstantBuffer(sizeof(pixelLighting)))) {
		TRACE("定数バッファの作成に失敗しました。")
		return false;
	}

	//	updateSubResource
	pContext->UpdateSubresource(buffer.Get(), 0, nullptr, &pixelLighting, 0, 0);

	//	PSに送る
	bool isSucceed = true;
	DX_ShaderManager::GetInstance()->SetConstantBuffers(0, 1, buffer.GetAddressOf(), SHADER_TYPE::PIXEL_SHADER);

	return isSucceed;
}