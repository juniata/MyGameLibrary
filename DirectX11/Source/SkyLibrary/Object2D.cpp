#include <DxLibrary/DX_Library.h>
#include "GameObject.h"
#include "Object2D.h"
#include	<stdio.h>

/// <summary>
/// メンバ変数等を初期化
/// </summary>
Object2D::Object2D() :
	m_height(0),
	m_width(0),
	m_isCloned(false),
	m_isChanged(false),
	m_isLRMirror(false),
	m_isUDMirror(false)
{}

/// <summary>
/// メンバ変数等を解放
/// </summary>
Object2D::~Object2D()
{
}

/// <summary>
/// テクスチャの読み込みと頂点バッファの作成を行う 
/// </summary>
/// <param name="filepath">テクスチャのファイルパス(/Resource/以下から)</param>
/// <returns>成否</returns>
bool Object2D::CommonInitialize(const char* fFilepath)
{
	bool isSucceed = false;

	do
	{
		//	頂点バッファを作成
		DX::tagVertex2D vertices[] = {
			/* 左下 */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
			/* 左上 */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
			/* 右下 */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
			/* 右上 */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
		};

		m_vertexBuffer.Attach(DX_BufferCreater::VertexBuffer(sizeof(vertices), vertices));
		if (nullptr == m_vertexBuffer.Get()) {
			TRACE("failed to DX_Buffer::CreateVertexBuffer()");
			break;
		}

		// テクスチャを読み込む
		char texturePath[MAX_PATH] = { '\n' };
		sprintf_s(texturePath, "%s%s", "Resource\\", fFilepath);

		if (LoadTexture(texturePath) == false)
		{
			TRACE("failed to LoadTexture()")
			break;
		}

		isSucceed = true;
	} while (false);

	return isSucceed;
}

/// <summary>
/// テクスチャの読み込みと頂点バッファの作成を行う 
/// </summary>
/// <param name="filepath">テクスチャのファイルパス(/Resource/以下から)</param>
/// <returns>成否</returns>
bool Object2D::Initialize(const char* filepath)
{
	bool isSucceed = CommonInitialize(filepath);

	DX_System* system = DX_System::GetInstance();
	SetRectPos(DX::tagRect(0.0f, 0.0f, DX::CAST::F(system->GetScreenWidth()), DX::CAST::F(system->GetScreenHeight())));
	SetUV(DX::tagRect(0.0f, 0.0f, DX::CAST::F(m_width), DX::CAST::F(m_height)));
	Update();

	return isSucceed;
}

/// <summary>
/// 指定したサイズでテクスチャ及び頂点バッファを作成する
/// </summary>
/// <param name="filepath">テクスチャのファイルパス(/Resource/以下から)</param>
/// <param name="rectPos">描画するサイズ</param>
/// <returns>成否</returns>
bool Object2D::Initialize(const char* filepath, const DX::tagRect& rectPos)
{
	bool isSucceed = CommonInitialize(filepath);

	SetRectPos(rectPos);
	SetUV(DX::tagRect(0.0f, 0.0f, DX::CAST::F(m_width), DX::CAST::F(m_height)));
	Update();

	return isSucceed;
}

/// <summary>
///  指定したサイズとUVでテクスチャ及び頂点バッファを作成する
/// </summary>
/// <param name="filepath">テクスチャのファイルパス(/Resource/以下から)</param>
/// <param name="rectPos">描画するサイズ</param>
/// <param name="uv">描画するテクスチャ座標</param>
/// <returns>成否</returns>
bool Object2D::Initialize(const char* filepath, const DX::tagRect& rectPos, const DX::tagRect& uv)
{
	bool isSucceed = CommonInitialize(filepath);

	SetRectPos(rectPos);
	SetUV(uv);
	Update();

	return isSucceed;
}


/// <summary>
/// テクスチャの高さを取得する
/// </summary>
/// <returns>テクスチャの高さ</returns>
unsigned int Object2D::GetHeight()const
{
	return m_height;
}

/// <summary>
/// テクスチャの幅を取得する
/// </summary>
/// <returns>テクスチャの幅</returns>
unsigned int Object2D::GetWidth()const
{
	return m_width;
}

/// <summary>
/// 描画する
/// </summary>
/// <returns>成否</returns>
bool Object2D::Render()
{
	auto result = true;

	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	DX_ShaderManager* shaderManager = DX_ShaderManager::GetInstance();

	DX_Shader* vertexShader = shaderManager->GetShader(SHADER_OBJECT_2D::VERTEX_SHADER);
	DX_Shader* pixelShader = shaderManager->GetShader(SHADER_OBJECT_2D::PIXEL_SHADER);

	vertexShader->Begin();
	pixelShader->Begin();

	unsigned int stride = sizeof(DX::tagVertex2D);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetInputLayout(shaderManager->GetInputLayout2D());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	shaderManager->SetShaderResources(0, 1, m_srv.GetAddressOf(), SHADER_TYPE::PIXEL_SHADER);

	deviceContext->Draw(4, 0);

	vertexShader->End();
	pixelShader->End();

	return result;
}

/// <summary>
/// テクスチャを読み込む
/// </summary>
/// <param name="filepath"></param>
/// <returns>成否</returns>
bool Object2D::LoadTexture(const char* filepath)
{
	auto isSucceed = false;

	do
	{
		m_srv = DX_TextureManager::GetInstance()->GetTexture(filepath);

		if (nullptr == m_srv.Get()) {
			TRACE("failed to DX_TextureManager::GetTexture(), filepath = %s", filepath);
			break;
		}

		DX_TextureManager::GetInstance()->GetTextureSize(&m_height, &m_width, m_srv.Get());

		isSucceed = true;
	} while (false);

	return isSucceed;
}


/// <summary>
/// テクスチャを複製する
/// </summary>
/// <returns>複製したオブジェクト</returns>
Object2D* Object2D::Clone()
{
	auto pObject = new Object2D(*this);
	pObject->m_isCloned = true;

	return pObject;
}

/// <summary>
	/// オブジェクトが複製したものかどうかを取得する
	/// </summary>
	/// <returns>複製したものならtrue</returns>
bool Object2D::IsClone() const
{
	return m_isCloned;
}

/// <summary>
/// オブジェクトがオリジナルかどうかを取得する
/// </summary>
/// <returns>オリジナルならtrue</returns>
bool Object2D::IsOriginal() const
{
	return (m_isCloned == false);
}

/// <summary>
/// スクリーン内に描画されているかどうかを取得する
/// </summary>
/// <returns>描画されているならtrue</returns>
bool Object2D::IsInScreen() const
{
	auto isInScreen = false;

	DX_System* system = DX_System::GetInstance();

	float height = DX::CAST::F(system->GetScreenHeight());
	float width = DX::CAST::F(system->GetScreenWidth());

	if (0.0f <= m_rectPos.y && m_rectPos.bottom <= height &&
		0.0f <= m_rectPos.x && m_rectPos.right <= width)
	{
		isInScreen = true;
	}

	return isInScreen;
}


/// <summary>
/// 描画座標を設定する
/// </summary>
/// <param name="rect">描画座標</param>
void Object2D::SetRectPos(const DX::tagRect& rect)
{
	m_rectPos = rect;
	m_isChanged = true;
}

/// <summary>
/// UV座標を設定する
/// </summary>
/// <param name="uv">UV座標</param>
void Object2D::SetUV(const DX::tagRect& uv)
{
	m_uv = uv;
	m_isChanged = true;
}


/// <summary>
/// 描画座標を取得する
/// </summary>
/// <returns>描画座標</returns>
const DX::tagRect& Object2D::GetRectPos()
{
	return m_rectPos;
}

/// <summary>
/// UV座標を取得する
/// </summary>
/// <returns>UV座標</returns>
const DX::tagRect& Object2D::GetUV()
{
	return m_uv;
}

/// <summary>
/// 座標等を更新する
/// </summary>
/// <param name="isLRMirror">左右反転描画するかどうか</param>
/// <param name="isUDMirror">上下反転描画するかどうか</param>
void Object2D::Update(const bool isLRMirror, const bool isUDMirror)
{
	if (m_isChanged || m_isLRMirror != isLRMirror || m_isUDMirror != isUDMirror)
	{
		ID3D11DeviceContext* pContext = DX_System::GetInstance()->GetDeviceContext();

		//	頂点情報を作成
		CreateVertex(m_rectPos, m_uv, isLRMirror, isUDMirror);

		m_isChanged = false;
		m_isLRMirror = isLRMirror;
		m_isUDMirror = isUDMirror;
	}
}

/// <summary>
/// 頂点座標を作成する
/// </summary>
/// <param name="rectPos">画面に描画する範囲</param>
/// <param name="uv">描画する画像の範囲</param>
/// <param name="isLRMirror">左右反転するかどうか</param>
/// <param name="isUDMirror">上下反転するかどうか</param>
void Object2D::CreateVertex(const DX::tagRect& rectPos, const DX::tagRect& uv, const bool isLRMirror, const bool isUDMirror)
{
	DX_System* system = DX_System::GetInstance();
	ID3D11DeviceContext* context = system->GetDeviceContext();

	float windowWidth = DX::CAST::F(system->GetScreenWidth());
	float windowHeight = DX::CAST::F(system->GetScreenHeight());

	//	-1.0f ~ 1.0fに座標を正規化する
	DirectX::XMFLOAT2 center(1.0f / (windowWidth * 0.5f), 1.0f / (windowHeight * 0.5f));

	DX::tagRect norRectPos;
	norRectPos.left = center.x * rectPos.x - 1.0f;
	norRectPos.right = center.x * rectPos.w - 1.0f;
	norRectPos.bottom = 1.0f - center.y * rectPos.h;
	norRectPos.top = 1.0f - center.y * rectPos.y;



	//	UV座標を0.0f ~ 1.0fに正規化する
	DirectX::XMFLOAT2 centerUV(1.0f / DX::CAST::F(m_width), 1.0f / DX::CAST::F(m_height));

	DX::tagRect norUV;

	//	上下左右反転するかどうか
	if (isLRMirror && isUDMirror) {
		norUV.left = centerUV.x * uv.w;
		norUV.top = centerUV.y * uv.h;
		norUV.right = centerUV.x * uv.x;
		norUV.bottom = centerUV.y * uv.y;
	}
	else {

		// 左右反転するかどうか
		if (isLRMirror) {
			norUV.left = centerUV.x * uv.w;
			norUV.top = centerUV.y * uv.y;
			norUV.right = centerUV.x * uv.x;
			norUV.bottom = centerUV.y * uv.h;
		}
		//	上下反転するかどうか
		else if (isUDMirror) {
			norUV.left = centerUV.x * uv.x;
			norUV.top = centerUV.y * uv.h;
			norUV.right = centerUV.x * uv.w;
			norUV.bottom = centerUV.y * uv.y;
		}
		//	なんもしない場合
		else {
			norUV.left = centerUV.x * uv.x;
			norUV.top = centerUV.y * uv.y;
			norUV.right = centerUV.x * uv.w;
			norUV.bottom = centerUV.y * uv.h;
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
	context->UpdateSubresource(m_vertexBuffer.Get(), 0, nullptr, pVertices, 0, 0);
}