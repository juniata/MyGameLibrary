#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//	変数の初期化とバッファの作成
//
//-----------------------------------------------------------------------------------------
DX_Box::DX_Box() : 
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pConstantBuffer(nullptr),
	m_pos(0.0f, 0.0f, 0.0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_angle(0.0f, 0.0f, 0.0f),
	m_isChanged(true),
	m_isCloned(false)
{
	CreateBuffer();
	Update();
}

//-----------------------------------------------------------------------------------------
//
//	解放処理
//
//-----------------------------------------------------------------------------------------
DX_Box::~DX_Box()
{
	if (m_isCloned) {
		SAFE_RELEASE(m_pVertexBuffer);
		SAFE_RELEASE(m_pIndexBuffer);
		SAFE_RELEASE(m_pConstantBuffer);
	}
}

//-----------------------------------------------------------------------------------------
//
//	座標を設定
//
//-----------------------------------------------------------------------------------------
void DX_Box::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	大きさを設定
//
//-----------------------------------------------------------------------------------------
void DX_Box::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_scale = scale;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	向きを設定
//
//-----------------------------------------------------------------------------------------
void DX_Box::SetAngle(const DirectX::XMFLOAT3& angle)
{
	m_angle = angle;
	m_isChanged = true;
}

//-----------------------------------------------------------------------------------------
//
//	座標を取得
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 DX_Box::GetPos() const
{
	return m_pos;
}

//-----------------------------------------------------------------------------------------
//
//	向きを取得
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 DX_Box::GetAngle() const
{
	return m_angle;
}

//-----------------------------------------------------------------------------------------
//
//	大きさを取得
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 DX_Box::GetScale() const
{
	return m_scale;
}

//-----------------------------------------------------------------------------------------
//
//	座標等を更新する
//
//-----------------------------------------------------------------------------------------
void DX_Box::Update()
{
	if (m_isChanged)
	{
		//	回転行列 * 拡大行列 * 平行行列
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		DirectX::XMMATRIX angle = DirectX::XMMatrixRotationRollPitchYaw(m_angle.x, m_angle.y, m_angle.z);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

		XMStoreFloat4x4(&m_worldMat, angle * scale * translation);
		m_isChanged = false;
	}
}

//-----------------------------------------------------------------------------------------
//
//	描画する
//
//-----------------------------------------------------------------------------------------
void DX_Box::Render()
{
	DX_System*	pSystem = DX_System::GetInstance();

	ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();

	DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();

	DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_OBJECT_SHADER::VERTEX_SHADER);
	DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_OBJECT_SHADER::PIXEL_SHADER);

	//	シェーダー利用を開始
	pVertexShader->Begin(pDeviceContext);
	pPixelShader->Begin(pDeviceContext);

	unsigned int stride = sizeof(DX::tagObjectVertext);
	unsigned int offset = 0;

	//	VertexBufferを送る
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// IndexBufferを送る
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);

	//	InputLayoutの設定を送る
	pDeviceContext->IASetInputLayout(pShaderManager->GetDefaultInputLayoutObject());

	//	Primitiveの設定を送る
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	ワールド行列を送る
	pShaderManager->SetWorldMat(m_pConstantBuffer, m_worldMat, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

	// 描画を行う
	pDeviceContext->DrawIndexed(36, 0, 0);

	//	シェーダー利用を終了
	pVertexShader->End(pDeviceContext);
	pPixelShader->End(pDeviceContext);
}


//-----------------------------------------------------------------------------------------
//
//	複製する
//
//-----------------------------------------------------------------------------------------
DX_Box* DX_Box::Clone()
{
	auto pBox = new DX_Box(*this);
	pBox->m_isCloned = true;

	return pBox;
}


//------------------------------------------------------------------------------
//
//  オブジェクトが複製したものかどうか
//
//------------------------------------------------------------------------------
bool DX_Box::IsClone() const
{
	return m_isCloned;
}

//------------------------------------------------------------------------------
//
//  オブジェクトがオリジナルかどうか
//
//------------------------------------------------------------------------------
bool DX_Box::IsOriginal() const
{
	return !m_isCloned;
}

//------------------------------------------------------------------------------
//
//  レイキャスト判定
//
//------------------------------------------------------------------------------
bool DX_Box::RayCast(const DirectX::XMFLOAT3 pos)
{
	auto ret = false;

	return ret;
}
//-----------------------------------------------------------------------------------------
//
//	頂点バッファ等を作成する
//
//-----------------------------------------------------------------------------------------
void DX_Box::CreateBuffer()
{
	const int VERTEX_NUM = 8;

	DX::tagObjectVertext pVertices[VERTEX_NUM];

	const float LEFT_POS = -1.0f;
	const float RIGHT_POS = 1.0f;
	const float UP_POS = 1.0f;
	const float DOWN_POS = -1.0f;
	const float FRONT_POS = -1.0f;
	const float BACK_POS = 1.0f;

	const int FRONT_LEFT_UP_INDEX = 0;
	const int FRONT_RIGHT_UP_INDEX = 1;
	const int FRONT_RIGHT_DOWN_INDEX = 2;
	const int FRONT_LEFT_DOWN_INDEX = 3;
	const int BACK_LEFT_UP_INDEX = 4;
	const int BACK_RIGHT_UP_INDEX = 5;
	const int BACK_RIGHT_DOWN_INDEX = 6;
	const int BACK_LEFT_DOWN_INDEX = 7;

	// 前
	// 左上
	pVertices[FRONT_LEFT_UP_INDEX].pos.x = LEFT_POS;
	pVertices[FRONT_LEFT_UP_INDEX].pos.y = UP_POS;
	pVertices[FRONT_LEFT_UP_INDEX].pos.z = FRONT_POS;

	// 右上
	pVertices[FRONT_RIGHT_UP_INDEX].pos.x = RIGHT_POS;
	pVertices[FRONT_RIGHT_UP_INDEX].pos.y = UP_POS;
	pVertices[FRONT_RIGHT_UP_INDEX].pos.z = FRONT_POS;

	// 右下
	pVertices[FRONT_RIGHT_DOWN_INDEX].pos.x = RIGHT_POS;
	pVertices[FRONT_RIGHT_DOWN_INDEX].pos.y = DOWN_POS;
	pVertices[FRONT_RIGHT_DOWN_INDEX].pos.z = FRONT_POS;

	// 左下
	pVertices[FRONT_LEFT_DOWN_INDEX].pos.x = LEFT_POS;
	pVertices[FRONT_LEFT_DOWN_INDEX].pos.y = DOWN_POS;
	pVertices[FRONT_LEFT_DOWN_INDEX].pos.z = FRONT_POS;

	// 後
	// 左上
	pVertices[BACK_LEFT_UP_INDEX].pos.x = LEFT_POS;
	pVertices[BACK_LEFT_UP_INDEX].pos.y = UP_POS;
	pVertices[BACK_LEFT_UP_INDEX].pos.z = BACK_POS;

	// 右上
	pVertices[BACK_RIGHT_UP_INDEX].pos.x = RIGHT_POS;
	pVertices[BACK_RIGHT_UP_INDEX].pos.y = UP_POS;
	pVertices[BACK_RIGHT_UP_INDEX].pos.z = BACK_POS;

	// 右下
	pVertices[BACK_RIGHT_DOWN_INDEX].pos.x = RIGHT_POS;
	pVertices[BACK_RIGHT_DOWN_INDEX].pos.y = DOWN_POS;
	pVertices[BACK_RIGHT_DOWN_INDEX].pos.z = BACK_POS;

	// 左下
	pVertices[BACK_LEFT_DOWN_INDEX].pos.x = LEFT_POS;
	pVertices[BACK_LEFT_DOWN_INDEX].pos.y = DOWN_POS;
	pVertices[BACK_LEFT_DOWN_INDEX].pos.z = BACK_POS;

	// 白色
	for (int i = 0; i < VERTEX_NUM; ++i) {
		pVertices[i].color = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f);
	}
	
	// 面法線を計算
	// 頂点法線を計算
	// 頂点法線を格納
	// ジオメトリで面法制を計算　ライティングを行う

	// インデックスを設定
	unsigned short pIndices[] =
	{
		// 前面
		FRONT_LEFT_UP_INDEX,	FRONT_RIGHT_UP_INDEX,	FRONT_LEFT_DOWN_INDEX,
		FRONT_RIGHT_UP_INDEX,	FRONT_RIGHT_DOWN_INDEX, FRONT_LEFT_DOWN_INDEX,

		// 右面
		FRONT_RIGHT_UP_INDEX,	BACK_RIGHT_UP_INDEX,	FRONT_RIGHT_DOWN_INDEX,
		BACK_RIGHT_UP_INDEX,	BACK_RIGHT_DOWN_INDEX,	FRONT_RIGHT_DOWN_INDEX,

		// 後面
		BACK_RIGHT_UP_INDEX,	BACK_LEFT_UP_INDEX,		BACK_RIGHT_DOWN_INDEX,
		BACK_LEFT_UP_INDEX,		BACK_LEFT_DOWN_INDEX,	BACK_RIGHT_DOWN_INDEX,

		// 上面
		BACK_LEFT_UP_INDEX,		BACK_RIGHT_UP_INDEX,	FRONT_LEFT_UP_INDEX,
		BACK_RIGHT_UP_INDEX,	FRONT_RIGHT_UP_INDEX,	FRONT_LEFT_UP_INDEX,

		// 左面
		BACK_LEFT_UP_INDEX,		FRONT_LEFT_UP_INDEX,	BACK_LEFT_DOWN_INDEX,
		FRONT_LEFT_UP_INDEX,	FRONT_LEFT_DOWN_INDEX,	BACK_LEFT_DOWN_INDEX,

		// 下面
		FRONT_LEFT_DOWN_INDEX,	FRONT_RIGHT_DOWN_INDEX, BACK_LEFT_DOWN_INDEX,
		FRONT_RIGHT_DOWN_INDEX,	BACK_RIGHT_DOWN_INDEX,	BACK_LEFT_DOWN_INDEX
	};

	ID3D11Device* pDevice = DX_System::GetInstance()->GetDevice();

	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(pVertices), pVertices);
	m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(pDevice, sizeof(pIndices), pIndices);
	m_pConstantBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(DirectX::XMFLOAT4X4));
}