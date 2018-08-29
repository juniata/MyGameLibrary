#include	"DX_Library.h"
using namespace DirectX;

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
{
	tagVertex2D l_pVertex[] = {
		/* 左下 */	XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
		/* 左上 */	XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f),
		/* 右下 */	XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),
		/* 右上 */	XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)
	};

	//	バッファを作成
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(tagVertex2D) * 4, l_pVertex);
}


//-----------------------------------------------------------------------------------------
//
//  メンバー変数を初期化し、2DObjectを作成する
//
//-----------------------------------------------------------------------------------------
DX_2DObject::DX_2DObject(const char* pFilepath) : DX_2DObject()
{
	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\2dobject\\", pFilepath);
	LoadTexture(texturePath);
}


//-----------------------------------------------------------------------------------------
//
//  m_pShaderResourceViewを解放
//
//-----------------------------------------------------------------------------------------
DX_2DObject::~DX_2DObject()
{
	if (IsOriginal()) {
		SAFE_RELEASE(m_pVertexBuffer);
		DX_TextureManager::Release(m_pShaderResourceView);
	}
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
	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* l_pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	l_pVertexShader->Begin(pContext);
	l_pPixelShader->Begin(pContext);

	//	描画
	DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	l_pVertexShader->End(pContext);
	l_pPixelShader->End(pContext);
}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(const tagRect& renderPos)
{
	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	頂点情報を作成
	CreateVertex(pContext, renderPos, tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* l_pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	l_pVertexShader->Begin(pContext);
	l_pPixelShader->Begin(pContext);

	//	描画
	DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	l_pVertexShader->End(pContext);
	l_pPixelShader->End(pContext);
}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(const DirectX::XMFLOAT2& renderPos, const DirectX::XMFLOAT2& renderSize)
{
	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	頂点情報を作成
	CreateVertex(pContext, tagRect(renderPos.x, renderPos.y, (renderSize.x + renderPos.x), (renderSize.y + renderPos.y)), tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* l_pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	l_pVertexShader->Begin(pContext);
	l_pPixelShader->Begin(pContext);

	//	描画
	DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	l_pVertexShader->End(pContext);
	l_pPixelShader->End(pContext);
}

//-----------------------------------------------------------------------------------------
//
//  指定した範囲に描画
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Render(const tagRect& renderPos, const tagRect& texturePos)
{
	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

	//	頂点情報を作成
	CreateVertex(pContext, renderPos, tagRect(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height)));

	//	シェーダーを取得
	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* l_pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
	DX_Shader* l_pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

	//	シェーダーを利用
	l_pVertexShader->Begin(pContext);
	l_pPixelShader->Begin(pContext);

	//	描画
	DX_Buffer::Render2D(pShaderManager, pContext, m_pVertexBuffer, m_pShaderResourceView);

	//	シェーダーを終了
	l_pVertexShader->End(pContext);
	l_pPixelShader->End(pContext);
}

//-----------------------------------------------------------------------------------------
//
//  テクスチャを読み込む
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::LoadTexture(const char* pFilepath)
{
	//	テクスチャを取得
	m_pShaderResourceView = DX_TextureManager::GetTexture(pFilepath);

	//	テクスチャがロードできてるかチェック
	DEBUG_VALUE_CHECK(m_pShaderResourceView, "テクスチャのファイルパスが間違っています");

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
void DX_2DObject::CreateVertex(ID3D11DeviceContext* pContext, const tagRect& renderPos, const tagRect& texturePos)
{
	//	頂点情報
	tagVertex2D pVertex[4];

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

	// バッファの上書き
	pContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, pVertex, 0, 0);
}