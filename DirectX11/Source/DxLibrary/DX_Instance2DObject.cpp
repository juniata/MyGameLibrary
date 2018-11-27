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
	m_width(0),
	m_height(0),
	m_pInstance2DList(nullptr),
	m_instanceNum(0),
	m_enabled(true)
{}
//-----------------------------------------------------------------------------------------
//
//  デストラクタ
//
//-----------------------------------------------------------------------------------------
DX_Instance2DObject::~DX_Instance2DObject()
{
	DX_TextureManager::GetInstance()->Release(m_pShaderResourceView);
	DELETE_OBJ_ARRAY(m_pInstance2DList);
	SAFE_RELEASE(m_pVertexBuffer);
}

//-----------------------------------------------------------------------------------------
//
// メンバー変数を初期化し、インスタンス描画オブジェクトを作成する
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize)
{
	m_pInstance2DList = new DX::tagInstance2D[num];
	for (UINT i = 0; i < num; ++i)
	{
		m_pInstance2DList[i].x = 0.0f;
		m_pInstance2DList[i].y = 0.0f;
		m_pInstance2DList[i].z = 0.0f;
	}
	m_instanceNum = num;

	LoadTexture(pFilepath);

	//	1 ~ 0の値に変換
	const float centerX = 1.0f / (CAST_F(DX_System::GetWindowWidth()) * 0.5f);
	const float centerY = 1.0f / (CAST_F(DX_System::GetWindowHeight()) * 0.5f);


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

	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(vertices), vertices);

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
	m_pShaderResourceView = DX_TextureManager::GetInstance()->GetTexture(texturePath);

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


//-----------------------------------------------------------------------------------------
//
//  インスタンスリストを取得する
//
//-----------------------------------------------------------------------------------------
DX::tagInstance2D* DX_Instance2DObject::GetInstanceList()
{
	return m_pInstance2DList;
}

//-----------------------------------------------------------------------------------------
//
//  描画する
//
//-----------------------------------------------------------------------------------------
bool DX_Instance2DObject::Render()
{
	bool result = false;

	DEBUG_VALUE_CHECK(m_pVertexBuffer, "頂点バッファの作成に失敗しました。");
	DEBUG_VALUE_CHECK(m_pShaderResourceView, "テクスチャのファイルパスが間違っています");
	
	if (m_enabled) 
	{
		DX_System* pSystem = DX_System::GetInstance();
		DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
		ID3D11Device* pDevice = pSystem->GetDevice();
		ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();


		DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);
		DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

		//	シェーダー利用を開始
		pVertexShader->Begin(pDeviceContext);
		pPixelShader->Begin(pDeviceContext);

		unsigned int strides[] = { sizeof(DX::tagVertex2D) ,sizeof(m_pInstance2DList[0]) };
		unsigned int offsets[] = { 0,0 };

		ID3D11Buffer* pInstanceBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pInstance2DList[0]) * m_instanceNum, m_pInstance2DList);
		ID3D11Buffer* buffers[] = { m_pVertexBuffer, pInstanceBuffer };
		DEBUG_VALUE_CHECK(pInstanceBuffer, "インスタンスバッファの作成に失敗しました。");


		// 正規化したウィンドウサイズを送る
		pShaderManager->SetVector(0, DirectX::XMFLOAT4(2.0f / CAST_F(pSystem->GetWindowWidth()), 2.0f / CAST_F(pSystem->GetWindowHeight()), 0.0f, 0.0f), pDevice, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

		//	VertexBufferを送る
		pDeviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);

		//	InputLayoutの設定を送る
		pDeviceContext->IASetInputLayout(pShaderManager->GetDefaultInputLayoutInstance2D());

		//	Primitiveの設定を送る
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// テクスチャ情報を送る
		result = DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 1, &m_pShaderResourceView, DX_SHADER_TYPE::PIXEL_SHADER);

		// インスタンス描画を行う
		pDeviceContext->DrawInstanced(4, m_instanceNum, 0, 0);

		//	シェーダー利用を終了
		pVertexShader->End(pDeviceContext);
		pPixelShader->End(pDeviceContext);

		SAFE_RELEASE(pInstanceBuffer);
	}

	return result;
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