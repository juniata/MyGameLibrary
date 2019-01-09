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
	m_isCloned(false),
	m_isChanged(false),
	m_isLRMirror(false),
	m_isUDMirror(false)
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
bool DX_2DObject::CommonInitialize(const char* pFilepath)
{
	//	頂点バッファを作成
	DX::tagVertex2D vertices[] = {
		/* 左下 */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* 左上 */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* 右下 */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* 右上 */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(vertices), vertices);

	if (false == DebugValueCheck(m_pVertexBuffer, "バッファの作成に失敗しています。")) {
		return false;
	}

	// テクスチャを読み込む
	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\", pFilepath);
	bool isSucceed = LoadTexture(texturePath);

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//	テクスチャ読み込みと頂点バッファの作成を行う。
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Initialize(const char* pFilepath)
{
	bool isSucceed = CommonInitialize(pFilepath);

	DX_System* pSystem = DX_System::GetInstance();

	SetRectPos(0.0f, 0.0f, CAST_F(pSystem->GetWindowWidth()), CAST_F(pSystem->GetWindowHeight()));
	SetUV(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height));
	Update();

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//	テクスチャ読み込みと頂点バッファの作成を行う。
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Initialize(const char* pFilepath, const DX::tagRect& rectPos)
{
	bool isSucceed = CommonInitialize(pFilepath);

	SetRectPos(rectPos);
	SetUV(0.0f, 0.0f, CAST_F(m_width), CAST_F(m_height));
	Update();

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//	テクスチャ読み込みと頂点バッファの作成を行う。
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::Initialize(const char* pFilepath, const DX::tagRect& rectPos, const DX::tagRect& uv)
{
	bool isSucceed = CommonInitialize(pFilepath);

	SetRectPos(rectPos);
	SetUV(uv);
	Update();

	return isSucceed;
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
	auto result = false;


	DX_System* pSystem = DX_System::GetInstance();
	//	デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = pSystem->GetDeviceContext();

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

	if (false == DebugValueCheck(m_pShaderResourceView, "テクスチャの読み込みができていません。")) {
		return false;
	}

	//	テクスチャを細かな情報を取得
	ID3D11Resource* pResource = nullptr;
	m_pShaderResourceView->GetResource(&pResource);

	ID3D11Texture2D* pTex2D = nullptr;
	pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTex2D);

		D3D11_TEXTURE2D_DESC texDesc;
	pTex2D->GetDesc(&texDesc);
	//	テクスチャサイズを取得

	m_height = texDesc.Height;
	m_width = texDesc.Width;

	SAFE_RELEASE(pResource);
	SAFE_RELEASE(pTex2D);

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	複製する
//
//-----------------------------------------------------------------------------------------
DX_2DObject* DX_2DObject::Clone()
{
	auto pObject = new DX_2DObject(*this);
	pObject->m_isCloned = true;
	
	return pObject;
}

//-----------------------------------------------------------------------------------------
//
//	オブジェクトが複製したものかどうか
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsClone() const
{
	return m_isCloned;
}

//-----------------------------------------------------------------------------------------
//
//	オブジェクトがオリジナルかどうか
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsOriginal() const
{
	return !m_isCloned;
}

//-----------------------------------------------------------------------------------------
//
//	画面内に描画されているかどうか
//
//-----------------------------------------------------------------------------------------
bool DX_2DObject::IsInScreen() const
{
	auto isInScreen = false;

	DX_System* pSystem = DX_System::GetInstance();
	float height = CAST_F(pSystem->GetWindowHeight());
	float width = CAST_F(pSystem->GetWindowWidth());

	if (0.0f <= m_rectPos.y && m_rectPos.bottom <= height &&
		0.0f <= m_rectPos.x && m_rectPos.right <= width)
	{
		isInScreen = true;
	}

	return isInScreen;
}

//-----------------------------------------------------------------------------------------
//
//	描画座標を設定する
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::SetRectPos(const DX::tagRect& rect)
{
	m_rectPos = rect;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	描画座標を設定する
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::SetRectPos(const float left, const float top, const float right, const float bottom)
{
	m_rectPos.left = left;
	m_rectPos.top = top;
	m_rectPos.right = right;
	m_rectPos.bottom = bottom;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	UV座標を設定する
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::SetUV(const DX::tagRect& uv)
{
	m_uv = uv;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	UV座標を設定する
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::SetUV(const float left, const float top, const float right, const float bottom)
{
	m_uv.left = left;
	m_uv.top = top;
	m_uv.right = right;
	m_uv.bottom = bottom;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	描画座標を取得する
//
//-----------------------------------------------------------------------------------------
DX::tagRect  DX_2DObject::GetRectPos() const
{
	return m_rectPos;
}

//-----------------------------------------------------------------------------------------
//
//	UV座標を取得する
//
//-----------------------------------------------------------------------------------------
DX::tagRect  DX_2DObject::GetUV() const
{
	return m_uv;
}

//-----------------------------------------------------------------------------------------
//
//	頂点情報を更新する
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::Update(const bool isLRMirror, const bool isUDMirror)
{
	if (m_isChanged || m_isLRMirror != isLRMirror || m_isUDMirror != isUDMirror)
	{
		DX_System* pSystem = DX_System::GetInstance();
		ID3D11DeviceContext* pContext = pSystem->GetDeviceContext();

		//	頂点情報を作成
		CreateVertex(pContext, m_rectPos, m_uv, isLRMirror, isUDMirror);

		m_isChanged = false;
		m_isLRMirror = isLRMirror;
		m_isUDMirror = isUDMirror;
	}
}

//-----------------------------------------------------------------------------------------
//
//  頂点情報を作成する
//
//-----------------------------------------------------------------------------------------
void DX_2DObject::CreateVertex(ID3D11DeviceContext* pContext, const DX::tagRect& rectPos, const DX::tagRect& uv, const bool isLRMirror, const bool isUDMirror)
{
	DX_System* pSystem = DX_System::GetInstance();

	float windowWidth = CAST_F(pSystem->GetWindowWidth());
	float windowHeight = CAST_F(pSystem->GetWindowHeight());

	//	-1.0f ~ 1.0fに座標を正規化する
	DirectX::XMFLOAT2 center(1.0f / (windowWidth * 0.5f), 1.0f / (windowHeight * 0.5f));

	DX::tagRect norRectPos;
	norRectPos.left		= center.x * rectPos.x - 1.0f;
	norRectPos.right	= center.x * rectPos.w - 1.0f;
	norRectPos.bottom	= 1.0f - center.y * rectPos.h;
	norRectPos.top		= 1.0f - center.y * rectPos.y;



	//	UV座標を0.0f ~ 1.0fに正規化する
	DirectX::XMFLOAT2 centerUV(1.0f / CAST_F(m_width), 1.0f / CAST_F(m_height));

	DX::tagRect norUV;

	//	上下左右反転するかどうか
	if (isLRMirror && isUDMirror) {
		norUV.left		= centerUV.x * uv.w;
		norUV.top		= centerUV.y * uv.h;
		norUV.right		= centerUV.x * uv.x;
		norUV.bottom	= centerUV.y * uv.y;
	}
	else {

		// 左右反転するかどうか
		if (isLRMirror) {
			norUV.left		= centerUV.x * uv.w;
			norUV.top		= centerUV.y * uv.y;
			norUV.right		= centerUV.x * uv.x;
			norUV.bottom	= centerUV.y * uv.h;
		}
		//	上下反転するかどうか
		else if (isUDMirror) {
			norUV.left		= centerUV.x * uv.x;
			norUV.top		= centerUV.y * uv.h;
			norUV.right		= centerUV.x * uv.w;
			norUV.bottom	= centerUV.y * uv.y;
		}
		//	なんもしない場合
		else {
			norUV.left		= centerUV.x * uv.x;
			norUV.top		= centerUV.y * uv.y;
			norUV.right		= centerUV.x * uv.w;
			norUV.bottom	= centerUV.y * uv.h;
		}
	}
	
	//	頂点情報
	DX::tagVertex2D pVertices[4];

	//	描画座標を設定する
	//	左の座標
	pVertices[0].pos.x = norRectPos.left;
	pVertices[1].pos.x = norRectPos.left;

	//	下の座標
	pVertices[0].pos.y = norRectPos.bottom;
	pVertices[2].pos.y = norRectPos.bottom;

	//	上の座標
	pVertices[1].pos.y = norRectPos.top;
	pVertices[3].pos.y = norRectPos.top;

	//	右の座標
	pVertices[2].pos.x = norRectPos.right;
	pVertices[3].pos.x = norRectPos.right;

	//	z値
	pVertices[0].pos.z = 0.0f;
	pVertices[1].pos.z = 0.0f;
	pVertices[2].pos.z = 0.0f;
	pVertices[3].pos.z = 0.0f;

	//	UV座標を設定する
	//	左の座標
	pVertices[0].uv.x = norUV.left;
	pVertices[1].uv.x = norUV.left;

	//	下の座標
	pVertices[0].uv.y = norUV.bottom;
	pVertices[2].uv.y = norUV.bottom;

	//	上の座標
	pVertices[1].uv.y = norUV.top;
	pVertices[3].uv.y = norUV.top;

	//	右の座標
	pVertices[2].uv.x = norUV.right;
	pVertices[3].uv.x = norUV.right;

	// バッファの上書き
	pContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, pVertices, 0, 0);
}