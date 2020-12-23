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
	m_pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
	m_scale(DirectX::XMFLOAT2(1.0f,1.0f)),
	m_angle(0.0f)
{
	ZeroMemory(&m_rect, sizeof(m_rect));
	ZeroMemory(&m_uv, sizeof(m_uv));
}

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

	SetRect(DX::tagRect(0.0f, 0.0f, DX::CAST::F(m_system->GetScreenWidth()), DX::CAST::F(m_system->GetScreenHeight())));
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

	SetRect(rectPos);
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

	SetRect(rectPos);
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

	ID3D11DeviceContext* deviceContext = m_system->GetDeviceContext();

	DX_Shader* vertexShader = m_shaderManager->GetShader(SHADER_OBJECT_2D::VERTEX_SHADER);
	DX_Shader* pixelShader = m_shaderManager->GetShader(SHADER_OBJECT_2D::PIXEL_SHADER);

	vertexShader->Begin();
	pixelShader->Begin();

	unsigned int stride = sizeof(DX::tagVertex2D);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetInputLayout(m_shaderManager->GetInputLayout2D());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	m_shaderManager->SetShaderResources(0, 1, m_srv.GetAddressOf(), SHADER_TYPE::PIXEL_SHADER);

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
		m_srv = m_textureManager->GetTexture(filepath);

		if (nullptr == m_srv.Get()) {
			TRACE("failed to DX_TextureManager::GetTexture(), filepath = %s", filepath);
			break;
		}

		m_textureManager->GetTextureSize(&m_width, &m_height, m_srv.Get());

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

	float height = DX::CAST::F(m_system->GetScreenHeight());
	float width = DX::CAST::F(m_system->GetScreenWidth());

	if (0.0f <= m_rect.y && m_rect.bottom <= height && 0.0f <= m_rect.x && m_rect.right <= width)
	{
		isInScreen = true;
	}

	return isInScreen;
}


/// <summary>
/// 描画するオブジェクトのサイズを設定する
/// </summary>
/// <param name="rect">描画座標</param>
void Object2D::SetRect(const DX::tagRect& rect)
{
	m_rect = rect;
	m_isChanged = true;
}

/// <summary>
/// 座標を設定
/// </summary>
/// <param name="pos">座標</param>
void Object2D::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_isChanged = true;
}

/// <summary>
/// 大きさを設定する(描画するオブジェクトの大きさに対して掛け算する)
/// </summary>
/// <param name="scale"></param>
void Object2D::SetScale(const DirectX::XMFLOAT2& scale)
{
	m_scale = scale;
	m_isChanged = true;
}

/// <summary>
/// 画像の向きの設定する
/// </summary>
/// <param name="val">向きを設定</param>
/// <param name="isAngle">falseならラジアン、trueなら度数で設定</param>
void Object2D::SetAngle(const float val, bool isAngle)
{
	if (isAngle)
	{
		m_angle = val * (DirectX::XM_PI / 180.0f);
	}
	else
	{
		m_angle = val;
	}
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
	return m_rect;
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
void Object2D::Update()
{
	if (m_isChanged)
	{
		UpdateVertexBuffer();

		m_isChanged = false;
	}
}

/// <summary>
/// 頂点バッファを更新する
/// </summary>
void Object2D::UpdateVertexBuffer()
{
	ID3D11DeviceContext* context = m_system->GetDeviceContext();

	float windowWidth	= DX::CAST::F(m_system->GetScreenWidth());
	float windowHeight	= DX::CAST::F(m_system->GetScreenHeight());

	//	-1.0f ~ 1.0fに座標を正規化する
	DirectX::XMFLOAT2 center(1.0f / (windowWidth * 0.5f), 1.0f / (windowHeight * 0.5f));

	// 正規化した座標
	DX::tagRect norRectPos = 
	{
		center.x * m_rect.x - 1.0f,
		1.0f - center.y * m_rect.y,
		center.x * m_rect.w - 1.0f,
		1.0f - center.y * m_rect.h
	};
	
	//	UV座標を0.0f ~ 1.0fに正規化する
	DirectX::XMFLOAT2 centerUV(1.0f / DX::CAST::F(m_width), 1.0f / DX::CAST::F(m_height));

	// 正規化したUV座標
	DX::tagRect norUV =
	{
		centerUV.x * m_uv.left,
		centerUV.y * m_uv.top,
		centerUV.x * m_uv.right,
		centerUV.y * m_uv.bottom
	};

	DX::tagVertex2D vertices[] =
	{
		// 左下
		{DirectX::XMFLOAT3(norRectPos.left, norRectPos.bottom, 0.0f), DirectX::XMFLOAT2(norUV.left, norUV.bottom) },

		// 左上
		{DirectX::XMFLOAT3(norRectPos.left, norRectPos.top, 0.0f), DirectX::XMFLOAT2(norUV.left, norUV.top) },
		
		// 右下
		{DirectX::XMFLOAT3(norRectPos.right, norRectPos.bottom, 0.0f), DirectX::XMFLOAT2(norUV.right, norUV.bottom) },
		
		// 右上
		{DirectX::XMFLOAT3(norRectPos.right, norRectPos.top, 0.0f), DirectX::XMFLOAT2(norUV.right, norUV.top) }
	};

	const float originX = 0.5f * (vertices[0].pos.x + vertices[3].pos.x);
	const float originY = 0.5f * (vertices[0].pos.y + vertices[1].pos.y);
	DirectX::XMVECTOR scalling		= DirectX::XMVectorSet(m_scale.x, m_scale.y, 1.0f, 0.0f);
	DirectX::XMVECTOR origin		= DirectX::XMVectorSet(originX, originY, 0.0f, 0.0f);
	DirectX::XMVECTOR translation	= DirectX::XMVectorSet(m_pos.x * (2.0f / windowWidth), -m_pos.y * (2.0f / windowHeight), m_pos.z, 1.0f);
	DirectX::XMMATRIX transform		= DirectX::XMMatrixAffineTransformation2D(scalling, origin, m_angle, translation);
	
	DirectX::XMVECTOR pos;
	for (auto& v : vertices)
	{
		pos = DirectX::XMLoadFloat3(&v.pos);
		pos = DirectX::XMVector3TransformCoord(pos, transform);
		DirectX::XMStoreFloat3(&v.pos, pos);
	}	
	
	// バッファの上書き
	context->UpdateSubresource(m_vertexBuffer.Get(), 0, nullptr, vertices, 0, 0);
}