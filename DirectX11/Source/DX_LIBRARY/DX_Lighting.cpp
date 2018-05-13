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
void DX_Lighting::SetLightVertexShader()
{
	auto	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	ローカル変数
	ComPtr<ID3D11Buffer>		l_buffer;
	D3D11_BUFFER_DESC			l_bufferDesc = { NULL };
	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };

	//	頂点シェーダーに送る情報
	tagVertexLighting l_vertexLighting;
	l_vertexLighting.ambient	= m_ambient;
	l_vertexLighting.diffuse	= m_diffuse;
	l_vertexLighting.viewPos	= m_viewPos;
	l_vertexLighting.lightDir	= m_lightDir;
	l_vertexLighting.lightPos	= m_lightPos;

	//	16byte alignment check
	DEBUG_VALUE_CHECK((sizeof(l_vertexLighting) % 16 == 0) ? true : false, "16で割り切れません");

	//	定数バッファを作成
	l_bufferDesc.ByteWidth		= sizeof(tagVertexLighting);
	l_bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	l_bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	l_bufferDesc.CPUAccessFlags = 0;

	//	bufferを作成
	DX_System::GetInstance()->GetDevice()->CreateBuffer(&l_bufferDesc, nullptr, &l_buffer);

	//	updateSubResource
	l_deviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, &l_vertexLighting, 0, 0);

	//	PSに送る
	DX_ResourceManager::SetConstantbuffers(l_deviceContext, 3, 1, &l_buffer, DX_SHADER_TYPE::VERTEX_SHADER);
}


//-----------------------------------------------------------------------------------------
//
//  ピクセルシェーダーで行うライト情報を設定
//
//-----------------------------------------------------------------------------------------
void DX_Lighting::SetLightPixelShader()
{
	auto l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	ローカル変数
	ComPtr<ID3D11Buffer>		l_buffer;
	D3D11_BUFFER_DESC			l_bufferDesc = { NULL };
	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };

	//	頂点シェーダーに送る情報
	tagPixelLighting l_pixelLighting;
	l_pixelLighting.specular	= m_specular;
	l_pixelLighting.viewPos		= m_viewPos;
	l_pixelLighting.lightDir	= m_lightDir;

	//	16byte alignment check
	DEBUG_VALUE_CHECK((sizeof(l_pixelLighting) % 16 == 0) ? true : false, "16で割り切れません");

	//	定数バッファを作成
	l_bufferDesc.ByteWidth		= sizeof(tagPixelLighting);
	l_bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	l_bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	l_bufferDesc.CPUAccessFlags = 0;

	//	bufferを作成
	DX_System::GetInstance()->GetDevice()->CreateBuffer(&l_bufferDesc, nullptr, &l_buffer);

	//	updateSubResource
	l_deviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, &l_pixelLighting, 0, 0);

	//	PSに送る
	DX_ResourceManager::SetConstantbuffers(l_deviceContext, 0, 1, &l_buffer, DX_SHADER_TYPE::PIXEL_SHADER);

}