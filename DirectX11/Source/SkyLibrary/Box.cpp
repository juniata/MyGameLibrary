#include "SkyLibrary.h"

/// <summary>
/// 変数等を初期化する
/// </summary>
Box::Box() :
	m_pos(0.0f, 0.0f, 0.0f),
	m_scale(1.0f, 1.0f, 1.0f),
	m_angle(0.0f, 0.0f, 0.0f),
	m_isChanged(true),
	m_isCloned(false),
	m_pastRaypickIndex(-1)
{
	unsigned short indices[] =
	{
		// 前面
		0,1,3,
		1,2,3,

		// 右面
		1,5,2,
		5,6,2,

		// 後面
		5,4,6,
		4,7,6,

		// 上面
		4,5,0,
		5,1,0,

		// 左面
		4,0,7,
		0,3,7,

		// 下面
		3,2,7,
		2,6,7
	};
	memcpy(m_indices, indices, sizeof(indices));

	Update();
}

/// <summary>
/// 解放処理
/// </summary>
Box::~Box()
{}

/// <summary>
/// バッファ等の作成を行う
/// </summary>
/// <returns>成否</returns>
bool Box::Initialize()
{
	return CreateBuffer();
}

/// <summary>
/// 座標を設定する
/// </summary>
/// <param name="pos">座標</param>
void Box::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_isChanged = true;
}

/// <summary>
/// 大きさを設定する
/// </summary>
/// <param name="scale">大きさ</param>
void Box::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_scale = scale;
	m_isChanged = true;
}

/// <summary>
/// 向きを設定する
/// </summary>
/// <param name="angle">向き</param>
void Box::SetAngle(const DirectX::XMFLOAT3& angle)
{
	m_angle = angle;
	m_isChanged = true;
}

/// <summary>
/// 座標を取得する
/// </summary>
/// <returns>座標</returns>
DirectX::XMFLOAT3 Box::GetPos() const
{
	return m_pos;
}

/// <summary>
/// 向きを取得する
/// </summary>
/// <returns>向き</returns>
DirectX::XMFLOAT3 Box::GetAngle() const
{
	return m_angle;
}

/// <summary>
/// 大きさを取得する
/// </summary>
/// <returns>大きさ</returns>
DirectX::XMFLOAT3 Box::GetScale() const
{
	return m_scale;
}

/// <summary>
/// 座標等を更新する
/// </summary>
void Box::Update()
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

/// <summary>
/// 描画する
/// </summary>
void Box::Render()
{
	DX_System* system = DX_System::GetInstance();
	ID3D11DeviceContext* deviceContext = system->GetDeviceContext();

	DX_ShaderManager* shaderManager = DX_ShaderManager::GetInstance();
	DX_Shader* vertexShader = shaderManager->GetShader(SHADER_BOX::VERTEX_SHADER);
	DX_Shader* pixelShader	= shaderManager->GetShader(SHADER_BOX::PIXEL_SHADER);

	vertexShader->Begin();
	pixelShader->Begin();

	unsigned int stride = sizeof(DX::tagObjectVertext);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetInputLayout(shaderManager->GetInputLayoutBox());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shaderManager->SetWorldMat(m_worldMat, SHADER_TYPE::VERTEX_SHADER);

	deviceContext->DrawIndexed(36, 0, 0);

	vertexShader->End();
	pixelShader->End();
}


/// <summary>
/// 複製する
/// </summary>
/// <returns>複製したオブジェクト</returns>
Box* Box::Clone()
{
	auto pBox = new Box(*this);
	pBox->m_isCloned = true;

	return pBox;
}

/// <summary>
/// オブジェクトが複製したものかどうかを取得する
/// </summary>
/// <returns>複製したものかどうか</returns>
bool Box::IsClone() const
{
	return m_isCloned;
}

/// オブジェクトがオリジナルかどうかを取得する
/// </summary>
/// <returns>オリジナルかどうか</returns>
bool Box::IsOriginal() const
{
	return !m_isCloned;
}

/// /// <summary>
/// レイキャスト判定
/// </summary>
/// <param name="pos">判定を取りたいオブジェクトの座標</param>
/// <param name="vec">判定を取りたいオブジェクトのベクトル</param>
/// <param name="distance">レイの長さ</param>
/// <returns>あたったかどうか</returns>
bool Box::RayCast(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& vec, const float distance)
{
	auto ret = false;

	UNREFERENCED_PARAMETER(pos);
	UNREFERENCED_PARAMETER(vec);
	UNREFERENCED_PARAMETER(distance);

	return ret;
}

/// <summary>
/// バッファを作成する
/// </summary>
/// <returns>作成できたかどうか</returns>
bool Box::CreateBuffer()
{
	DX::tagObjectVertext pVertices[VERTEX_COUNT];

	// 前
	// 左上
	pVertices[0].pos.x = -0.5f;
	pVertices[0].pos.y = 0.5f;
	pVertices[0].pos.z = -0.5f;

	// 右上
	pVertices[1].pos.x = 0.5f;
	pVertices[1].pos.y = 0.5f;
	pVertices[1].pos.z = -0.5f;

	// 右下
	pVertices[2].pos.x = 0.5f;
	pVertices[2].pos.y = -0.5f;
	pVertices[2].pos.z = -0.5f;

	// 左下
	pVertices[3].pos.x = -0.5f;
	pVertices[3].pos.y = -0.5f;
	pVertices[3].pos.z = -0.5f;

	// 後
	// 左上
	pVertices[4].pos.x = -0.5f;
	pVertices[4].pos.y = 0.5f;
	pVertices[4].pos.z = 0.5f;

	// 右上
	pVertices[5].pos.x = 0.5f;
	pVertices[5].pos.y = 0.5f;
	pVertices[5].pos.z = 0.5f;

	// 右下
	pVertices[6].pos.x = 0.5f;
	pVertices[6].pos.y = -0.5f;
	pVertices[6].pos.z = 0.5f;

	// 左下
	pVertices[7].pos.x = -0.5f;
	pVertices[7].pos.y = -0.5f;
	pVertices[7].pos.z = 0.5f;

	// 白色
	for (int i = 0; i < VERTEX_COUNT; ++i) {
		pVertices[i].color = DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	}

	// 法線を計算
	CreateNormal(pVertices);

	m_vertexBuffer.Attach(DX_BufferCreater::VertexBuffer(sizeof(pVertices), pVertices));
	if (nullptr == m_vertexBuffer) {
		return false;
	}

	m_indexBuffer.Attach(DX_BufferCreater::IndexBuffer(sizeof(m_indices), m_indices));
	if (nullptr == m_indexBuffer) {
		return false;
	}

	m_constantBuffer.Attach(DX_BufferCreater::ConstantBuffer(sizeof(m_worldMat)));
	if (nullptr == m_constantBuffer) {
		return false;
	}

	return true;
}

/// <summary>
/// 法線を作成する
/// </summary>
/// <param name="pVertices">頂点情報</param>
void Box::CreateNormal(DX::tagObjectVertext* pVertices)
{
	DirectX::XMVECTOR p0, p1, p2;
	DirectX::XMFLOAT3 faceN;

	int index0 = 0;
	int index1 = 0;
	int index2 = 0;

	for (int i = 0; i < INDEX_COUNT; i += 3)
	{
		// 面法線を作成する
		index0 = m_indices[i];
		index1 = m_indices[i + 1];
		index2 = m_indices[i + 2];

		p0 = DirectX::XMLoadFloat3(&pVertices[index0].pos);
		p1 = DirectX::XMLoadFloat3(&pVertices[index1].pos);
		p2 = DirectX::XMLoadFloat3(&pVertices[index2].pos);

		CalcFaceNormal(faceN, p0, p1, p2);

		// 面法線を頂点法線に加算していく。
		AddFaceNormal(pVertices[index0].normal, pVertices[index0].normal, faceN);
		AddFaceNormal(pVertices[index1].normal, pVertices[index1].normal, faceN);
		AddFaceNormal(pVertices[index2].normal, pVertices[index2].normal, faceN);
	}

	for (int i = 0; i < 12; ++i)
	{
		DirectX::XMStoreFloat3(&pVertices[i].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&pVertices[i].normal)));
	}
}


/// <summary>
/// 面法線を算出する
/// </summary>
/// <param name="faceV">算出した面法線が格納される</param>
/// <param name="p0">頂点0の座標</param>
/// <param name="p1">頂点1の座標</param>
/// <param name="p2">頂点2の座標</param>
void Box::CalcFaceNormal(DirectX::XMFLOAT3& faceV, const DirectX::XMVECTOR& p0, const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2)
{
	DirectX::XMStoreFloat3(&faceV, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(p1, p0), DirectX::XMVectorSubtract(p2, p0))));
}

/// <summary>
/// 算出した面法線を、頂点の法線に加算していく
/// </summary>
/// <param name="outVf">面法線が格納される</param>
/// <param name="vN">頂点法線</param>
/// <param name="faceN">面法線</param>
void Box::AddFaceNormal(DirectX::XMFLOAT3& outVf, const DirectX::XMFLOAT3& vN, const DirectX::XMFLOAT3& faceN)
{
	DirectX::XMStoreFloat3(&outVf, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&vN), DirectX::XMLoadFloat3(&faceN)));
}
