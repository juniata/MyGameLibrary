#include	"DX_Library.h"
using namespace DirectX;

DX_Instance2DObject::DX_Instance2DObject() : m_pVertexBuffer(nullptr), m_pShaderResourceView(nullptr), m_pPosList(nullptr), m_instanceNum(0), m_height(0),m_width(0)
{
}

DX_Instance2DObject::DX_Instance2DObject(const char* pFilepath, const size_t num) : m_pPosList(new DirectX::XMFLOAT3[num]), m_instanceNum(num)
{
	LoadTexture(pFilepath);
	ZeroMemory(m_pPosList, sizeof(m_pPosList[0]) * num);

	//	1 ~ 0の値に変換
	const float l_centerX = 1.0f / (CAST_F(DX_System::GetWindowWidth()) * 0.5f);
	const float l_centerY = 1.0f / (CAST_F(DX_System::GetWindowHeight()) * 0.5f);


	tagVertex2D pVertex[] = {
		/* 左下 */	XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
		/* 左上 */	XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f),
		/* 右下 */	XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),
		/* 右上 */	XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)
	};

	tagRect renderPos(0.0f, 0.0f, 32.0f, 32.0f);

	//	左の座標
	pVertex[1].pos.x = pVertex[0].pos.x = l_centerX * renderPos.x - 1.0f;

	//	下の座標
	pVertex[2].pos.y = pVertex[0].pos.y = 1.0f - l_centerY * renderPos.h;

	//	上の座標
	pVertex[3].pos.y = pVertex[1].pos.y = 1.0f - l_centerY * renderPos.y;

	//	右の座標
	pVertex[3].pos.x = pVertex[2].pos.x = l_centerX * renderPos.w - 1.0f;


	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetInstance()->GetDevice(), sizeof(tagVertex2D) * 4, pVertex);
}

DX_Instance2DObject::~DX_Instance2DObject()
{
	DX_TextureManager::Release(m_pShaderResourceView);
	DELETE_OBJ_ARRAY(m_pPosList);
	SAFE_RELEASE(m_pVertexBuffer);
}
//-----------------------------------------------------------------------------------------
//
//  テクスチャを読み込む
//
//-----------------------------------------------------------------------------------------
void DX_Instance2DObject::LoadTexture(
	const char* pFilepath
)
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

DirectX::XMFLOAT3* DX_Instance2DObject::GetPosList()
{
	return m_pPosList;
}
DirectX::XMFLOAT3* DX_Instance2DObject::GetPosList(const unsigned int index)
{
	return &m_pPosList[index];
}

void DX_Instance2DObject::Render()
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

	unsigned int strides[] = { sizeof(tagVertex2D) ,sizeof(m_pPosList[0]) };
	unsigned int offsets[] = { 0,0 };

	ID3D11Buffer* pInstanceBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pPosList[0]) * m_instanceNum, m_pPosList);
	ID3D11Buffer* buffers[] = { m_pVertexBuffer, pInstanceBuffer };
	
	
	// 正規化したウィンドウサイズを送る
	pShaderManager->SetVector(0, XMFLOAT4(2.0f / CAST_F(pSystem->GetWindowWidth()), 2.0f / CAST_F(pSystem->GetWindowHeight()), 0.0f, 0.0f), pDevice, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

	//	VertexBufferを送る
	pDeviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);

	//	InputLayoutの設定を送る
	pDeviceContext->IASetInputLayout(pShaderManager->GetDefaultInputLayoutInstance2D());

	//	Primitiveの設定を送る
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ情報を送る
	DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 1, &m_pShaderResourceView, DX_SHADER_TYPE::PIXEL_SHADER);
	
	// インスタンス描画を行う
	pDeviceContext->DrawInstanced(4, m_instanceNum, 0, 0);

	//	シェーダー利用を終了
	pVertexShader->End(pDeviceContext);
	pPixelShader->End(pDeviceContext);

	SAFE_RELEASE(pInstanceBuffer);

}