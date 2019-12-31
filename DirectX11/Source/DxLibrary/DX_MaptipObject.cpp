//#include	"DX_Library.h"
//#include	<stdio.h>
//
////-----------------------------------------------------------------------------------------
////
////  コンストラクタ
////
////-----------------------------------------------------------------------------------------
//DX_MaptipObject::DX_MaptipObject() :
//	m_pShaderResourceView(nullptr),
//	m_pVertexBuffer(nullptr),
//	m_width(0),
//	m_height(0),
//	m_splitCountX(0),
//	m_splitCountY(0),
//	m_instanceNum(0),
//	m_enabled(true),
//	m_pPosList(nullptr)
//{}
//
////-----------------------------------------------------------------------------------------
////
////  デストラクタ
////
////-----------------------------------------------------------------------------------------
//DX_MaptipObject::~DX_MaptipObject()
//{
//	DX_TextureManager::GetInstance()->Release(m_pShaderResourceView);
//	DELETE_OBJ_ARRAY(m_pPosList);
//	SAFE_RELEASE(m_pVertexBuffer);
//}
//
//bool DX_MaptipObject::Initialize(const char* pFilepath, const unsigned int splitCountX, const unsigned int splitCountY, const DirectX::XMFLOAT2& renderSize)
//{
//	m_splitCountX = splitCountX;
//	m_splitCountY = splitCountY;
//	m_instanceNum = m_splitCountX * m_splitCountY;
//	m_pPosList = new DirectX::XMFLOAT3[m_instanceNum];
//	ZeroMemory(m_pPosList, sizeof(m_pPosList[0]) * m_instanceNum);
//
//	if (false == LoadTexture(pFilepath)) {
//		return false;
//	}
//
//	//	1 ~ 0の値に変換
//	const float centerX = 1.0f / (DX::CAST::F(DX_System::GetWindowWidth()) * 0.5f);
//	const float centerY = 1.0f / (DX::CAST::F(DX_System::GetWindowHeight()) * 0.5f);
//
//
//	DX::tagVertex2D pVertex[] = {
//		/* 左下 */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
//		/* 左上 */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
//		/* 右下 */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
//		/* 右上 */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
//	};
//
//	DX::tagRect renderPos(0.0f, 0.0f, renderSize.x, renderSize.y);
//
//	//	左の座標
//	pVertex[1].pos.x = pVertex[0].pos.x = centerX * renderPos.x - 1.0f;
//
//	//	下の座標
//	pVertex[2].pos.y = pVertex[0].pos.y = 1.0f - centerY * renderPos.h;
//
//	//	上の座標
//	pVertex[3].pos.y = pVertex[1].pos.y = 1.0f - centerY * renderPos.y;
//
//	//	右の座標
//	pVertex[3].pos.x = pVertex[2].pos.x = centerX * renderPos.w - 1.0f;
//
//
//	if (nullptr == (m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetDevice(), sizeof(pVertex[0]) * 4, pVertex))) {
//		return false;
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  テクスチャを読み込む
////
////-----------------------------------------------------------------------------------------
//bool DX_MaptipObject::LoadTexture(const char* pFilepath)
//{
//	bool result = false;
//
//	char texturePath[MAX_PATH] = { '\n' };
//	sprintf_s(texturePath, "%s%s", "Resource\\2dobject\\", pFilepath);
//
//	//	テクスチャを取得
//	m_pShaderResourceView = DX_TextureManager::GetInstance()->GetTexture(texturePath);
//
//	//	テクスチャがロードできてるかチェック
//	if (m_pShaderResourceView) {
//		//	テクスチャを細かな情報を取得
//		ID3D11Resource* pResource = nullptr;
//		m_pShaderResourceView->GetResource(&pResource);
//
//		ID3D11Texture2D* pTex2D = nullptr;
//		if (FAILED(pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTex2D))) {
//			D3D11_TEXTURE2D_DESC texDesc;
//			pTex2D->GetDesc(&texDesc);
//
//			//	テクスチャサイズを取得
//			m_height = texDesc.Height;
//			m_width = texDesc.Width;
//		}
//		else {
//			result = true;
//		}
//
//		SAFE_RELEASE(pResource);
//		SAFE_RELEASE(pTex2D);
//	}
//
//	return result;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  座標一覧を取得する
////
////-----------------------------------------------------------------------------------------
//DirectX::XMFLOAT3* DX_MaptipObject::GetPosList()
//{
//	return m_pPosList;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  指定した箇所からの座標一覧を取得する
////
////-----------------------------------------------------------------------------------------
//DirectX::XMFLOAT3* DX_MaptipObject::GetPosList(const unsigned int y, const unsigned int x)
//{
//	return &m_pPosList[y * m_splitCountX + x];
//}
//
////-----------------------------------------------------------------------------------------
////
////  描画する
////
////-----------------------------------------------------------------------------------------
//bool DX_MaptipObject::Render()
//{
//	bool result = false;
//
//	DEBUG_VALUE_CHECK(m_pVertexBuffer, "頂点バッファの作成に失敗しました。");
//	DEBUG_VALUE_CHECK(m_pShaderResourceView, "テクスチャのファイルパスが間違っています");
//
//	if (m_enabled)
//	{
//		DX_System* pSystem = DX_System::GetInstance();
//		DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
//		ID3D11Device* pDevice = pSystem->GetDevice();
//		ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();
//
//
//		DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);
//		DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);
//
//		//	シェーダー利用を開始
//		pVertexShader->Begin(pDeviceContext);
//		pPixelShader->Begin(pDeviceContext);
//
//		unsigned int strides[] = { sizeof(DX::tagVertex2D) ,sizeof(m_pPosList[0]) };
//		unsigned int offsets[] = { 0,0 };
//
//		ID3D11Buffer* pInstanceBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pPosList[0]) * m_instanceNum, m_pPosList);
//		ID3D11Buffer* buffers[] = { m_pVertexBuffer, pInstanceBuffer };
//		DEBUG_VALUE_CHECK(pInstanceBuffer, "インスタンスバッファの作成に失敗しました。");
//
//
//		// 正規化したウィンドウサイズを送る
//		pShaderManager->SetVector(0, DirectX::XMFLOAT4(2.0f / DX::CAST::F(pSystem->GetWindowWidth()), 2.0f / DX::CAST::F(pSystem->GetWindowHeight()), 0.0f, 0.0f), pDevice, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);
//
//		//	VertexBufferを送る
//		pDeviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
//
//		//	InputLayoutの設定を送る
//		pDeviceContext->IASetInputLayout(pShaderManager->GetDefaultInputLayoutInstance2D());
//
//		//	Primitiveの設定を送る
//		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//		// テクスチャ情報を送る
//		result = DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 1, &m_pShaderResourceView, DX_SHADER_TYPE::PIXEL_SHADER);
//
//		// インスタンス描画を行う
//		pDeviceContext->DrawInstanced(4, m_instanceNum, 0, 0);
//
//		//	シェーダー利用を終了
//		pVertexShader->End(pDeviceContext);
//		pPixelShader->End(pDeviceContext);
//
//		SAFE_RELEASE(pInstanceBuffer);
//	}
//
//	return result;
//}
//
////-----------------------------------------------------------------------------------------
////
////  指定した番号のオブジェクトを無効化します(描画されなくなる)
////
////-----------------------------------------------------------------------------------------
//void DX_MaptipObject::Disable(const size_t index)
//{
//	// 描画されなくなるようz値を1より大きくする(これ以外やり方がわからん。。。)
//	m_pPosList[index].z = 1.1f;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  指定した番号のオブジェクトを無効化します(描画されなくなる)
////
////-----------------------------------------------------------------------------------------
//void DX_MaptipObject::Disable()
//{
//	m_enabled = false;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  指定した番号のオブジェクトの有効化する（描画されるようになる）
////
////-----------------------------------------------------------------------------------------
//void DX_MaptipObject::Enable(const size_t index)
//{
//	m_pPosList[index].z = 0.0f;
//}
//
////------------------------------------------------------------------------------
////
////  @brief		インタンス描画を行うようにする
////
////------------------------------------------------------------------------------
//void DX_MaptipObject::Enable()
//{
//	m_enabled = true;
//}
//
////------------------------------------------------------------------------------
////
////  @brief		指定したインスタンスが無効かどうか
////
////------------------------------------------------------------------------------
//bool DX_MaptipObject::IsDisable(const size_t index) const
//{
//	return (m_pPosList[index].z >= 1.0f);
//}
//
////------------------------------------------------------------------------------
////
////  @brief		指定したインスタンスが有効かどうか
////
////------------------------------------------------------------------------------
//bool DX_MaptipObject::IsEnable(const size_t index) const
//{
//	return (m_pPosList[index].z < 1.0f);
//}
//
////------------------------------------------------------------------------------
////
////  @brief		指定したインスタンスが無効かどうか
////
////------------------------------------------------------------------------------
//bool DX_MaptipObject::IsDisable() const
//{
//	return !m_enabled;
//}
//
////------------------------------------------------------------------------------
////
////  @brief		指定したインスタンスが有効かどうか
////
////------------------------------------------------------------------------------
//bool DX_MaptipObject::IsEnable() const
//{
//	return m_enabled;
//}