#include	"DX_Library.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  メンバー変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_2DObject::DX_2DObject() :
	m_pVertexBuffer(nullptr),
	m_pShaderResourceView(nullptr),
	m_height(0),
	m_width(0),
	m_bClone(false)
{}


//-----------------------------------------------------------------------------------------
//
//  m_pShaderResourceViewを解放
//
//-----------------------------------------------------------------------------------------
DX_2DObject::~DX_2DObject()
{
	if (IsOriginal()) {
		SAFE_RELEASE(m_pVertexBuffer);
		DX_TextureManager::GetInstance()->Release(m_pShaderResourceView);
	}
}


//-----------------------------------------------------------------------------------------
//
//	テクスチャ読み込みと頂点バッファの作成を行う。
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Initialize(const char* pFilepath)
{
	bool result = false;

	//	頂点バッファを作成
	DX::tagVertex2D vertices[] = {
		/* 左下 */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* 左上 */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* 右下 */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* 右上 */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(vertices), vertices);
	DEBUG_VALUE_CHECK(m_pVertexBuffer, "バッファの作成に失敗しています。");

	// テクスチャを読み込む
	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\2dobject\\", pFilepath);
	result = LoadTexture(texturePath);

	return result;
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
bool DX_2DObject::Render()
{
	bool result = false;

	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	描画
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render(const DX::tagRect& renderPos)
{
	bool result = false;

	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	頂点情報を作成
	CreateVertex(pContext, renderPos, DX::tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	描画
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲、指定した画像サイズを描画
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render(const DirectX::XMFLOAT2& renderPos, const DirectX::XMFLOAT2& renderSize)
{
	bool result = false;

	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	頂点情報を作成
	CreateVertex(pContext, DX::tagRect(renderPos.x, renderPos.y, (renderSize.x + renderPos.x), (renderSize.y + renderPos.y)), DX::tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	描画
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render(const float left, const float top, const float right, const float bottom, bool isMirror)
{

	bool result = false;

	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	頂点情報を作成
	CreateVertex(pContext, DX::tagRect(left, top, right, bottom), DX::tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)), isMirror);

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	描画
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Render(const DX::tagRect& renderPos, const DX::tagRect& texturePos)
{
	bool result = false;

	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	頂点情報を作成
	CreateVertex(pContext, renderPos, DX::tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	pVertexShader->Begin(pContext);
	pPixelShader->Begin(pContext);

	//	描画
	result = DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	pVertexShader->End(pContext);
	pPixelShader->End(pContext);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  テクスチャを読み込む
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::LoadTexture(const char* pFilepath)
{
	//	テクスチャを取得
	m_pShaderResourceView = DX_TextureManager::GetInstance()->GetTexture(pFilepath);

	//	テクスチャがロードできてるかチェック
	if (m_pShaderResourceView) {
		//	テクスチャを細かな情報を取得
		ID3D11Resource* l_pResource = nullptr;
		m_pShaderResourceView->GetResource(&l_pResource);

		ID3D11Texture2D* l_pTexture2D = nullptr;
		l_pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&l_pTexture2D);

		D3D11_TEXTURE2D_DESC l_texDesc;
		l_pTexture2D->GetDesc(&l_texDesc);
		//	テクスチャサイズを取得

		m_height = l_texDesc.Height;
		m_width = l_texDesc.Width;

		SAFE_RELEASE(l_pResource);
		SAFE_RELEASE(l_pTexture2D);
	}
	else {
		DEBUG_VALUE_CHECK(false, "テクスチャの読み込みができていません。");
	}

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	複製する
//
//-----------------------------------------------------------------------------------------
DX_2DObject* DX_2DObject::Clone()
{
	DX_2DObject* pObject = new DX_2DObject(*this);
	pObject->m_bClone = true;
	
	return pObject;
}

//-----------------------------------------------------------------------------------------
//
//	オブジェクトが複製したものかどうか
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsClone() const
{
	return m_bClone;
}

//-----------------------------------------------------------------------------------------
//
//	オブジェクトがオリジナルかどうか
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsOriginal() const
{
	return !m_bClone;
}

//-----------------------------------------------------------------------------------------
//
//  頂点情報を作成する
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::CreateVertex(ID3D11DeviceContext* pContext, const DX::tagRect& renderPos, const DX::tagRect& texturePos, bool isMirror)
{
	//	頂点情報
	DX::tagVertex2D pVertex[4];

	//	1 ~ 0の値に変換
	const float l_centerX = 1.0f / (CAST_F(DX_System::GetWindowWidth()) * 0.5f);
	const float l_centerY = 1.0f / (CAST_F(DX_System::GetWindowHeight()) * 0.5f);

	//	0~1の割合の落とし込む
	const float l_centerUv_X = (1.0f / m_width);
	const float l_centerUv_Y = (1.0f / m_height);

	// UVのほうこうを逆にする
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

	if (isMirror) {
		//	左の座標
		pVertex[1].uv.x = pVertex[0].uv.x = l_centerUv_X * texturePos.w;

		//	上の座標
		pVertex[3].uv.y = pVertex[1].uv.y = l_centerUv_Y * texturePos.y;

		//	右の座標
		pVertex[3].uv.x = pVertex[2].uv.x = l_centerUv_X * texturePos.x;

		//	下の座標
		pVertex[2].uv.y = pVertex[0].uv.y = l_centerUv_Y * texturePos.h;
	}
	else {
		//	左の座標
		pVertex[1].uv.x = pVertex[0].uv.x = l_centerUv_X * texturePos.x;

		//	上の座標
		pVertex[3].uv.y = pVertex[1].uv.y = l_centerUv_Y * texturePos.y;

		//	右の座標
		pVertex[3].uv.x = pVertex[2].uv.x = l_centerUv_X * texturePos.w;

		//	下の座標
		pVertex[2].uv.y = pVertex[0].uv.y = l_centerUv_Y * texturePos.h;
	}


	// バッファの上書き
	pContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, pVertex, 0, 0);
}