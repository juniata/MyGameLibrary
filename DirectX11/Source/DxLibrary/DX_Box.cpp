#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//	static定数
//
//-----------------------------------------------------------------------------------------
const float DX_Box::LEFT_POS	= -1.0f;
const float DX_Box::RIGHT_POS	= 1.0f;
const float DX_Box::UP_POS		= 1.0f;
const float DX_Box::DOWN_POS	= -1.0f;
const float DX_Box::FRONT_POS	= -1.0f;
const float DX_Box::BACK_POS	= 1.0f;

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
//	バッファを作成する
//
//-----------------------------------------------------------------------------------------
bool DX_Box::Initialize()
{
	return CreateBuffer();
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
bool DX_Box::CreateBuffer()
{
	const int VERTEX_NUM = 8;

	DX::tagObjectVertext pVertices[VERTEX_NUM];

	// 前
	// 左上
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_UP].pos.x = DX_Box::LEFT_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_UP].pos.y = DX_Box::UP_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_UP].pos.z = DX_Box::FRONT_POS;

	// 右上
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_UP].pos.x = DX_Box::RIGHT_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_UP].pos.y = DX_Box::UP_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_UP].pos.z = DX_Box::FRONT_POS;

	// 右下
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN].pos.x = DX_Box::RIGHT_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN].pos.y = DX_Box::DOWN_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN].pos.z = DX_Box::FRONT_POS;

	// 左下
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_DOWN].pos.x = DX_Box::LEFT_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_DOWN].pos.y = DX_Box::DOWN_POS;
	pVertices[DX_Box::BOX_INDEX::FRONT_LEFT_DOWN].pos.z = DX_Box::FRONT_POS;

	// 後
	// 左上
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_UP].pos.x = DX_Box::LEFT_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_UP].pos.y = DX_Box::UP_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_UP].pos.z = DX_Box::BACK_POS;

	// 右上
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_UP].pos.x = DX_Box::RIGHT_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_UP].pos.y = DX_Box::UP_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_UP].pos.z = DX_Box::BACK_POS;

	// 右下
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_DOWN].pos.x = DX_Box::RIGHT_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_DOWN].pos.y = DX_Box::DOWN_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_RIGHT_DOWN].pos.z = DX_Box::BACK_POS;

	// 左下
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_DOWN].pos.x = DX_Box::LEFT_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_DOWN].pos.y = DX_Box::DOWN_POS;
	pVertices[DX_Box::BOX_INDEX::BACK_LEFT_DOWN].pos.z = DX_Box::BACK_POS;

	// 白色
	for (int i = 0; i < VERTEX_NUM; ++i) {
		pVertices[i].color = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f);
	}
	
	// 面法線を計算
	CreateFaceNormal(pVertices);

	// 頂点法線を計算
	// 頂点法線を格納
	// ジオメトリで面法制を計算　ライティングを行う

	// インデックスを設定
	unsigned short pIndices[] =
	{
		// 前面
		DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN, DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,

		// 右面
		DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,	DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,

		// 後面
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,

		// 上面
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,
		DX_Box::BOX_INDEX::BACK_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_RIGHT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,

		// 左面
		DX_Box::BOX_INDEX::BACK_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_LEFT_UP,	DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,

		// 下面
		DX_Box::BOX_INDEX::FRONT_LEFT_DOWN,		DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,	DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
		DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN,	DX_Box::BOX_INDEX::BACK_RIGHT_DOWN,		DX_Box::BOX_INDEX::BACK_LEFT_DOWN,
	};

	ID3D11Device* pDevice = DX_System::GetInstance()->GetDevice();

	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(pVertices), pVertices);
	if (m_pVertexBuffer == nullptr) {
		return false;
	}

	m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(pDevice, sizeof(pIndices), pIndices);
	if (m_pIndexBuffer == nullptr) {
		return false;
	}

	m_pConstantBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(DirectX::XMFLOAT4X4));
	if (m_pConstantBuffer == nullptr) {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//	面法線を作成する
//
//-----------------------------------------------------------------------------------------
void DX_Box::CreateFaceNormal(DX::tagObjectVertext* pVertex)
{
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	p0 = DirectX::XMLoadFloat3(&pVertex[DX_Box::BOX_INDEX::FRONT_LEFT_UP].pos);
	p1 = DirectX::XMLoadFloat3(&pVertex[DX_Box::BOX_INDEX::FRONT_RIGHT_UP].pos);
	p2 = DirectX::XMLoadFloat3(&pVertex[DX_Box::BOX_INDEX::FRONT_RIGHT_DOWN].pos);

	CalcTraiangleNormal(p0, p1, p2);
}


//-----------------------------------------------------------------------------------------
//
//	三角形の面法線を算出する
//
//-----------------------------------------------------------------------------------------
DirectX::XMVECTOR DX_Box::CalcTraiangleNormal(const DirectX::XMVECTOR& p0, const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2)
{
	return DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(p1, p0), DirectX::XMVectorSubtract(p2, p0)));
}

//-----------------------------------------------------------------------------------------
//
//	頂点法線を作成する
//
//-----------------------------------------------------------------------------------------
void DX_Box::CreateVertexNormal()
{

}