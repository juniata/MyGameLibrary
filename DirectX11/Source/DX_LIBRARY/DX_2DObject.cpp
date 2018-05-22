#include	"DX_Library.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  メンバー変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_2DObject::DX_2DObject() :

//	テクスチャを初期化
m_pShaderResourceView(nullptr),

//	テクスチャサイズを初期化
m_height(0),
m_width(0)
{}


//-----------------------------------------------------------------------------------------
//
//  メンバー変数を初期化し、2DObjectを作成する
//
//-----------------------------------------------------------------------------------------
DX_2DObject::DX_2DObject(const char* pFilepath) : DX_2DObject()
{
	LoadTexture(pFilepath);
}


//-----------------------------------------------------------------------------------------
//
//  m_pShaderResourceViewを解放
//
//-----------------------------------------------------------------------------------------
DX_2DObject::~DX_2DObject()
{
	//	テクスチャを解放する
	DX_TextureManager::Release(m_pShaderResourceView);
}

//-----------------------------------------------------------------------------------------
//
//  テクスチャの高さを取得する
//
//-----------------------------------------------------------------------------------------
unsigned int DX_2DObject::GetHeight()const
{
	return m_height;
}

//-----------------------------------------------------------------------------------------
//
//  テクスチャの幅を取得する
//
//-----------------------------------------------------------------------------------------
unsigned int DX_2DObject::GetWidth()const
{
	return m_width;
}

//-----------------------------------------------------------------------------------------
//
//  全画面に描画
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render()
{
	tagVertex2D l_pVertex[] = {
		/* 左下 */	XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
		/* 左上 */	XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f),
		/* 右下 */	XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),
		/* 右上 */	XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)
	};

	//	バッファを作成
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(),sizeof(tagVertex2D)* 4, l_pVertex);
	
	//	シェーダーを取得
	DX_Shader* l_pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader  = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	デバイスコンテキストを取得
	auto	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	シェーダーを利用
	l_pVertexShader->Begin(l_deviceContext);
	l_pPixelShader->Begin(l_deviceContext);

	//	描画
	DX_Buffer::Render2D(l_buffer.Get(), m_pShaderResourceView);
	//	シェーダーを終了
	l_pVertexShader->End(l_deviceContext);
	l_pPixelShader->End(l_deviceContext);
}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(const tagRect& renderPos)
{
	//	頂点情報
	tagVertex2D l_pVertex[4];

	//	頂点情報を作成
	CreateVertex(l_pVertex, renderPos, tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	バッファを作成
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(),sizeof(tagVertex2D)* 4, l_pVertex);

	//	シェーダーを取得
	DX_Shader* l_pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader  = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	デバイスコンテキストを取得
	ComPtr<ID3D11DeviceContext>	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	シェーダーを利用
	l_pVertexShader->Begin(l_deviceContext.Get());
	l_pPixelShader->Begin(l_deviceContext.Get());

	//	描画
	DX_Buffer::Render2D(l_buffer.Get(), m_pShaderResourceView);

	//	シェーダーを終了
	l_pVertexShader->End(l_deviceContext.Get());
	l_pPixelShader->End(l_deviceContext.Get());

}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(DirectX::XMFLOAT2 renderPos, DirectX::XMFLOAT2 renderSize)
{
	//	頂点情報
	tagVertex2D l_pVertex[4];

	//	頂点情報を作成
	CreateVertex(l_pVertex, tagRect(renderPos.x, renderPos.y, (renderSize.x + renderPos.x), (renderSize.y + renderPos.y)), tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	バッファを作成
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(tagVertex2D) * 4, l_pVertex);

	//	シェーダーを取得
	DX_Shader* l_pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	デバイスコンテキストを取得
	ComPtr<ID3D11DeviceContext>	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	シェーダーを利用
	l_pVertexShader->Begin(l_deviceContext.Get());
	l_pPixelShader->Begin(l_deviceContext.Get());

	//	描画
	DX_Buffer::Render2D(l_buffer.Get(), m_pShaderResourceView);

	//	シェーダーを終了
	l_pVertexShader->End(l_deviceContext.Get());
	l_pPixelShader->End(l_deviceContext.Get());

}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(const tagRect& renderPos, const tagRect& texturePos)
{
	//	頂点情報
	tagVertex2D l_pVertex[4];

	//	頂点情報を作成
	CreateVertex(l_pVertex, renderPos, tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	バッファを作成
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(tagVertex2D) * 4, l_pVertex);

	//	シェーダーを取得
	DX_Shader* l_pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader  = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	デバイスコンテキストを取得
	ComPtr<ID3D11DeviceContext>	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	シェーダーを利用
	l_pVertexShader->Begin(l_deviceContext.Get());
	l_pPixelShader->Begin(l_deviceContext.Get());

	//	描画
	DX_Buffer::Render2D(l_buffer.Get(), m_pShaderResourceView);

	//	シェーダーを終了
	l_pVertexShader->End(l_deviceContext.Get());
	l_pPixelShader->End(l_deviceContext.Get());

}

//-----------------------------------------------------------------------------------------
//
//  テクスチャを読み込む
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::LoadTexture(
	const char* pFilepath
	)
{
	//	テクスチャを取得
	m_pShaderResourceView = DX_TextureManager::GetTexture(pFilepath);

	//	テクスチャがロードできてるかチェック
	DEBUG_VALUE_CHECK(m_pShaderResourceView, "テクスチャのファイルパスが間違っています");

	//	テクスチャを細かな情報を取得
	ComPtr<ID3D11Resource> l_resource;
	m_pShaderResourceView->GetResource(&l_resource);

	//ComPtr<ID3D11Texture2D> l_texture2D;
	//l_resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&aa);
	ID3D11Texture2D* l_texture2D = (ID3D11Texture2D*)l_resource.Get();
	D3D11_TEXTURE2D_DESC l_texDesc;
	l_texture2D->GetDesc(&l_texDesc);

	//	テクスチャサイズを取得
	m_height = l_texDesc.Height;
	m_width = l_texDesc.Width;
}


//-----------------------------------------------------------------------------------------
//
//  頂点情報を作成する
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::CreateVertex(tagVertex2D* pVertex, const tagRect& renderPos, const tagRect& texturePos)
{
	//	1 ~ 0の値に変換
	const float l_centerX = 1.0f / (CAST_F(DX_System::GetWindowWidth()) * 0.5f);
	const float l_centerY = 1.0f / (CAST_F(DX_System::GetWindowHeight()) * 0.5f);

	//	0~1の割合の落とし込む
	const float l_centerUv_X = (1.0f / m_width);
	const float l_centerUv_Y = (1.0f / m_height);


	//	左の座標
	pVertex[1].pos.x = pVertex[0].pos.x = l_centerX * renderPos.x - 1.0f;

	//	下の座標
	pVertex[2].pos.y = pVertex[0].pos.y = 1.0f - l_centerY * renderPos.h;

	//	上の座標
	pVertex[3].pos.y = pVertex[1].pos.y = 1.0f - l_centerY * renderPos.y;

	//	右の座標
	pVertex[3].pos.x = pVertex[2].pos.x = l_centerX * renderPos.w - 1.0f;

	//	z値
	pVertex[0].pos.z = 0.0f;
	pVertex[1].pos.z = 0.0f;
	pVertex[2].pos.z = 0.0f;
	pVertex[3].pos.z = 0.0f;


	//	左の座標
	pVertex[1].uv.x = pVertex[0].uv.x = l_centerUv_X * texturePos.x;

	//	上の座標
	pVertex[3].uv.y = pVertex[1].uv.y = l_centerUv_Y * texturePos.y;

	//	右の座標
	pVertex[3].uv.x = pVertex[2].uv.x = l_centerUv_X * texturePos.w;

	//	下の座標
	pVertex[2].uv.y = pVertex[0].uv.y = l_centerUv_Y * texturePos.h;

}