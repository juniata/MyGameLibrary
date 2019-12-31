#include	"DX_Library.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_Instance2DObject::DX_Instance2DObject() :
	m_pShaderResourceView(nullptr),
	m_pVertexBuffer(nullptr),
	m_pPosBuffer(nullptr),
	m_pUVBuffer(nullptr),
	m_pVertices(nullptr),
	m_pUvs(nullptr),
	m_width(0),
	m_height(0),
	m_instanceNum(0),
	m_enabled(true),
	m_changedUV(false),
	m_changedPos(false),
	m_chipSize(DirectX::XMFLOAT2(1.0f,1.0f))
{}
//-----------------------------------------------------------------------------------------
//
//  デストラクタ
//
//-----------------------------------------------------------------------------------------
DX_Instance2DObject::~DX_Instance2DObject()
{
	DX_TextureManager::Release(m_pShaderResourceView);
	DELETE_OBJ_ARRAY(m_pVertices);
	DELETE_OBJ_ARRAY(m_pUvs);


	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pUVBuffer);
	SAFE_RELEASE(m_pPosBuffer);
}

//-----------------------------------------------------------------------------------------
//
// メンバー変数を初期化し、インスタンス描画オブジェクトを作成する
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize, const DirectX::XMFLOAT2& mapChipSize)
{
	m_pVertices = new DirectX::XMFLOAT3[num];
	m_pUvs = new DirectX::XMFLOAT2[num];
	for (UINT i = 0; i < num; ++i)
	{
		m_pVertices[i].x = 0.0f;
		m_pVertices[i].y = 0.0f;
		m_pVertices[i].z = 0.0f;
		m_pUvs[i].x = 0.0f;
		m_pUvs[i].y = 0.0f;
	}
	m_instanceNum = num;

	LoadTexture(pFilepath);

	m_chipSize.x = mapChipSize.x / DX::CAST::F(m_width);
	m_chipSize.y = mapChipSize.y / DX::CAST::F(m_height);

	DX_System* pSystem = DX_System::GetInstance();

	//	1 ~ 0の値に変換
	auto centerX = 1.0f / (DX::CAST::F(pSystem->GetScreenWidth()) * 0.5f);
	auto centerY = 1.0f / (DX::CAST::F(pSystem->GetScreenHeight()) * 0.5f);


	DX::tagVertex2D vertices[] = {
		/* 左下 */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, m_chipSize.y),
		/* 左上 */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* 右下 */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(m_chipSize.x, m_chipSize.y),
		/* 右上 */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(m_chipSize.x, 0.0f)
	};

	DX::tagRect renderPos(0.0f, 0.0f, renderSize.x, renderSize.y);

	//	左の座標
	vertices[1].pos.x = vertices[0].pos.x = centerX * renderPos.x - 1.0f;

	//	下の座標
	vertices[2].pos.y = vertices[0].pos.y = 1.0f - centerY * renderPos.h;

	//	上の座標
	vertices[3].pos.y = vertices[1].pos.y = 1.0f - centerY * renderPos.y;

	//	右の座標
	vertices[3].pos.x = vertices[2].pos.x = centerX * renderPos.w - 1.0f;

	ID3D11Device* pDevice = pSystem->GetDevice();

	// 頂点バッファを作成
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(vertices), vertices);
	if (DebugValueCheck(m_pVertexBuffer, "頂点バッファの作成に失敗しました。")) {
		return false;
	}

	// 頂点バッファ(座標を作成)
	m_pPosBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pVertices[0]) * m_instanceNum, m_pVertices);
	if (DebugValueCheck(m_pPosBuffer, "座標配列バッファの作成に失敗しました。")) {
		return false;
	}

	// 頂点バッファ(UVを作成)
	m_pUVBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pUvs[0]) * m_instanceNum, m_pUvs);
	if (DebugValueCheck(m_pUVBuffer, "UVバッファの作成に失敗しました。")) {
		return false;
	}

	return true;
}
bool DX_Instance2DObject::Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize)
{
	m_pVertices = new DirectX::XMFLOAT3[num];
	m_pUvs = new DirectX::XMFLOAT2[num];
	for (UINT i = 0; i < num; ++i)
	{
		m_pVertices[i].x = 0.0f;
		m_pVertices[i].y = 0.0f;
		m_pVertices[i].z = 0.0f;
		m_pUvs[i].x = 0.0f;
		m_pUvs[i].y = 0.0f;
	}
	m_instanceNum = num;

	LoadTexture(pFilepath);

	DX_System* pSystem = DX_System::GetInstance();

	//	1 ~ 0の値に変換
	auto centerX = 1.0f / (DX::CAST::F(pSystem->GetScreenWidth()) * 0.5f);
	auto centerY = 1.0f / (DX::CAST::F(pSystem->GetScreenHeight()) * 0.5f);


	DX::tagVertex2D vertices[] = {
		/* 左下 */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* 左上 */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* 右下 */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* 右上 */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};

	DX::tagRect renderPos(0.0f, 0.0f, renderSize.x, renderSize.y);

	//	左の座標
	vertices[1].pos.x = vertices[0].pos.x = centerX * renderPos.x - 1.0f;

	//	下の座標
	vertices[2].pos.y = vertices[0].pos.y = 1.0f - centerY * renderPos.h;

	//	上の座標
	vertices[3].pos.y = vertices[1].pos.y = 1.0f - centerY * renderPos.y;

	//	右の座標
	vertices[3].pos.x = vertices[2].pos.x = centerX * renderPos.w - 1.0f;

	ID3D11Device* pDevice = pSystem->GetDevice();

	// 頂点バッファを作成
	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(vertices), vertices);

	// 頂点バッファ(座標を作成)
	m_pPosBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pVertices[0]) * m_instanceNum, m_pVertices);

	// 頂点バッファ(UVを作成)
	m_pUVBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pUvs[0]) * m_instanceNum, m_pUvs);

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  テクスチャを読み込む
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::LoadTexture(const char* pFilepath)
{
	bool  result = false;

	char texturePath[MAX_PATH] = { '\n' };
	sprintf_s(texturePath, "%s%s", "Resource\\2dobject\\", pFilepath);

	//	テクスチャを取得
	m_pShaderResourceView = DX_TextureManager::GetTexture(texturePath);

	//	テクスチャがロードできてるかチェック
	if (m_pShaderResourceView) {
		//	テクスチャを細かな情報を取得
		ID3D11Resource* pResource = nullptr;
		m_pShaderResourceView->GetResource(&pResource);

		ID3D11Texture2D* pTexture2D = nullptr;
		if (SUCCEEDED(pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture2D))) {

			D3D11_TEXTURE2D_DESC texDesc;
			pTexture2D->GetDesc(&texDesc);

			//	テクスチャサイズを取得
			m_height = texDesc.Height;
			m_width = texDesc.Width;

			result = true;
		}

		SAFE_RELEASE(pResource);
		SAFE_RELEASE(pTexture2D);
	}

	return result;
}

void DX_Instance2DObject::BufferUpdate()
{
	ID3D11Device* pDevice = DX_System::GetInstance()->GetDevice();

	// 頂点バッファ(座標を作成)
	if (m_changedPos) {
		SAFE_RELEASE(m_pPosBuffer);
		m_pPosBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pVertices[0]) * m_instanceNum, m_pVertices);
		m_changedPos = false;
	}

	// 頂点バッファ(UVを作成)
	if (m_changedUV) {
		SAFE_RELEASE(m_pUVBuffer);
		m_pUVBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pUvs[0]) * m_instanceNum, m_pUvs);
		m_changedUV = false;
	}
}


//-----------------------------------------------------------------------------------------
//
//  描画する
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::Render()
{
	auto isSucceed = true;

	if (m_enabled) 
	{
		DX_System* pSystem = DX_System::GetInstance();

		ID3D11Device* pDevice = pSystem->GetDevice();
		ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();


		DX_Shader* pVertexShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);
		DX_Shader* pPixelShader = DX_ShaderManager::GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

		//	シェーダー利用を開始
		pVertexShader->Begin(pDeviceContext);
		pPixelShader->Begin(pDeviceContext);

		unsigned int strides[] = { sizeof(DX::tagVertex2D) ,sizeof(m_pVertices[0]), sizeof(m_pUvs[0]) };
		unsigned int offsets[] = { 0, 0, 0 };

		// バッファを更新する
		BufferUpdate();

		ID3D11Buffer* buffers[] = { m_pVertexBuffer, m_pPosBuffer, m_pUVBuffer };

		// 正規化したウィンドウサイズを送る
		DX_ShaderManager::SetVector(0, DirectX::XMFLOAT4(2.0f / DX::CAST::F(pSystem->GetScreenWidth()), 2.0f / DX::CAST::F(pSystem->GetScreenHeight()), 0.0f, 0.0f), pDevice, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

		//	VertexBufferを送る
		pDeviceContext->IASetVertexBuffers(0, 3, buffers, strides, offsets);

		//	InputLayoutの設定を送る
		pDeviceContext->IASetInputLayout(DX_ShaderManager::GetDefaultInputLayoutInstance2D());

		//	Primitiveの設定を送る
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// テクスチャ情報を送る
		isSucceed = DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 1, &m_pShaderResourceView, DX_SHADER_TYPE::PIXEL_SHADER);

		// インスタンス描画を行う
		pDeviceContext->DrawInstanced(4, m_instanceNum, 0, 0);

		//	シェーダー利用を終了
		pVertexShader->End(pDeviceContext);
		pPixelShader->End(pDeviceContext);
	}

	return isSucceed;
}

//-----------------------------------------------------------------------------------------
//
//  指定した番号のオブジェクトを無効化します(描画されなくなる)
//
//-----------------------------------------------------------------------------------------
void DX_Instance2DObject::Disable()
{
	m_enabled = false;
}

//------------------------------------------------------------------------------
//
//  @brief		インタンス描画を行うようにする
//
//------------------------------------------------------------------------------
void DX_Instance2DObject::Enable()
{
	m_enabled = true;
}

//------------------------------------------------------------------------------
//
//  @brief		指定したインスタンスが無効かどうか
//
//------------------------------------------------------------------------------
bool DX_Instance2DObject::IsDisable() const
{
	return !m_enabled;
}

//------------------------------------------------------------------------------
//
//  @brief		指定したインスタンスが有効かどうか
//
//------------------------------------------------------------------------------
bool DX_Instance2DObject::IsEnable() const
{
	return m_enabled;
}