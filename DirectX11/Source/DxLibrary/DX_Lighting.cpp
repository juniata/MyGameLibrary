#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
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
//  メンバ変数初期化
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::Initialize()
{
	//	環境光を設定
	SetAmbient(DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f));

	//	拡散光を設定
	SetDiffuse(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	反射光を設定
	SetSpecular(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	//	ライトの座標を設定
	SetLightPos(DirectX::XMFLOAT3(40.0f, 200.0, 0.0f));

	//	ライトの向きを設定
	SetLightDir(DirectX::XMFLOAT3(0.0f,-1.0f,1.0f));

	//	視点を設定
	SetViewPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

	//	ライトをシェーダーに送る
	SetLightVertexShader();
	SetLightPixelShader();
}

//-----------------------------------------------------------------------------------------
//
//  環境光を設定
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
//  拡散光を設定
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
//  反射光を設定
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
//	ライトの座標を設定
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetLightPos(const DirectX::XMFLOAT3& pos)
{
	m_lightPos = pos;
}

//-----------------------------------------------------------------------------------------
//
//  ライトの方向を設定
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetLightDir(const DirectX::XMFLOAT3& dir)
{
	m_lightDir = dir;
}


//-----------------------------------------------------------------------------------------
//
//  視点を設定
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetViewPos(const DirectX::XMFLOAT3& pos)
{
	m_viewPos = pos;
}

//-----------------------------------------------------------------------------------------
//
//  頂点シェーダーで行うライト情報を設定
//
//-----------------------------------------------------------------------------------------
bool DX_Lighting::SetLightVertexShader()
{
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	ローカル変数
	ID3D11Buffer*				pBuffer = nullptr;
	D3D11_BUFFER_DESC			bufferDesc = { NULL };

	//	頂点シェーダーに送る情報
	tagVertexLighting vertexLighting;
	vertexLighting.ambient	= m_ambient;
	vertexLighting.diffuse	= m_diffuse;
	vertexLighting.viewPos	= m_viewPos;
	vertexLighting.lightDir	= m_lightDir;
	vertexLighting.lightPos	= m_lightPos;

	//	16byte alignment check
	if (false == DebugValueCheck((sizeof(vertexLighting) % 16 == 0), "16で割り切れません")) {
		return false;
	}

	//	定数バッファを作成
	bufferDesc.ByteWidth = sizeof(tagVertexLighting);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//	bufferを作成
	pBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(vertexLighting));
	if (false == DebugValueCheck(pBuffer, "定数バッファの作成に失敗しました。")) {
		return false;
	}

	//	updateSubResource
	pContext->UpdateSubresource(pBuffer, 0, nullptr, &vertexLighting, 0, 0);

	//	PSに送る
	bool isSucceed = DX_ResourceManager::SetConstantbuffers(pContext, 3, 1, &pBuffer, DX_SHADER_TYPE::VERTEX_SHADER);

	SAFE_RELEASE(pBuffer);

	return isSucceed;
}


//-----------------------------------------------------------------------------------------
//
//  ピクセルシェーダーで行うライト情報を設定
//
//-----------------------------------------------------------------------------------------
bool DX_Lighting::SetLightPixelShader()
{
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	ローカル変数
	ID3D11Buffer*				pBuffer = nullptr;
	D3D11_BUFFER_DESC			bufferDesc = { NULL };

	//	頂点シェーダーに送る情報
	tagPixelLighting pixelLighting;
	pixelLighting.specular	= m_specular;
	pixelLighting.viewPos		= m_viewPos;
	pixelLighting.lightDir	= m_lightDir;

	//	16byte alignment check
	if (false == DebugValueCheck((sizeof(pixelLighting) % 16 == 0), "16で割り切れません。")) {
		return false;
	}

	//	定数バッファを作成
	bufferDesc.ByteWidth = sizeof(tagPixelLighting);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	//	bufferを作成
	pBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(pixelLighting));
	if (false == DebugValueCheck(pBuffer, "定数バッファの作成に失敗しました。")) {
		return false;
	}

	//	updateSubResource
	pContext->UpdateSubresource(pBuffer, 0, nullptr, &pixelLighting, 0, 0);

	//	PSに送る
	bool isSucceed = DX_ResourceManager::SetConstantbuffers(pContext, 0, 1, &pBuffer, DX_SHADER_TYPE::PIXEL_SHADER);

	SAFE_RELEASE(pBuffer);

	return isSucceed;
}