#include	"SkyLibrary.h"

#define VIEW_DEFAULT_ASPECT  DX::CAST::F(PI * 0.25f)
#define VIEW_DEFAULT_ZNEAR  DX::CAST::F(1.0f)
#define VIEW_DEFAULT_ZFAR	DX::CAST::F(1000.0f)

//-----------------------------------------------------------------------------------------
//
//	メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
View::View() :
	m_pos(0.0f, 20, -50.0f),
	m_target(0.0f, 20.0f, 0.0f),
	m_upDirection(0.0f, 1.0f, 0.0f),
	m_bChanged(true),
	m_updateFrameNum(0)
{
	//	メンバ変数初期化
	ZeroMemory(&m_viewPort, sizeof(m_viewPort));
	ZeroMemory(&m_matView, sizeof(m_matView));
	ZeroMemory(&m_matProj, sizeof(m_matProj));
	ZeroMemory(&m_matViewProj, sizeof(m_matViewProj));
	ZeroMemory(&m_fov, sizeof(m_fov));
	ZeroMemory(m_plane, sizeof(m_plane));

	//	ビューポートとプロジェクションを設定する
	SetViewPort();
	SetProjection(VIEW_DEFAULT_ASPECT, VIEW_DEFAULT_ZNEAR, VIEW_DEFAULT_ZFAR);

	//	行列を先に計算
	Active();

	//	視錐台の面を作成
	CreateFrustum();

	m_constantBuffer.Attach(DX_BufferCreater::ConstantBuffer(sizeof(DirectX::XMFLOAT4X4) * 3));
	m_constantBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_constantBuffer") - 1, "m_constantBuffer");

}


//-----------------------------------------------------------------------------------------
//
//	解放
//
//-----------------------------------------------------------------------------------------
View::~View()
{
}

//-----------------------------------------------------------------------------------------
//
//	行列を計算し、RSにViewportを設定する
//
//-----------------------------------------------------------------------------------------
void View::Active()
{
	//	行列作成に関わる変数が変化した場合trueになり、行列が再計算される
	if (m_bChanged) {

		//	ビュー行列設定
		DirectX::XMVECTOR pos = XMLoadFloat3(&m_pos);
		DirectX::XMVECTOR target = XMLoadFloat3(&m_target);
		DirectX::XMVECTOR up = XMLoadFloat3(&m_upDirection);

		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);
		XMStoreFloat4x4(&m_matView, view);

		//	投影行列変換
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(m_fov.fovY, m_fov.aspect, m_fov.znear, m_fov.zfar);
		XMStoreFloat4x4(&m_matProj, proj);


		//	viewProj行列を作成
		DirectX::XMMATRIX viwProj = view * proj;
		XMStoreFloat4x4(&m_matViewProj, viwProj);

		//	RSにビューポートを設定
		DX_System::GetInstance()->GetDeviceContext()->RSSetViewports(1, GetViewPort());

		//	フラグをfalseにする
		m_bChanged = false;
	}
}

//-----------------------------------------------------------------------------------------
//
//	RenderTargetViewとDepthStencileViewをクリアする
//
//-----------------------------------------------------------------------------------------
void View::Clear(
	const bool bZClear,
	const bool bStencilClear,
	ID3D11RenderTargetView* pRtv,
	ID3D11DepthStencilView* pDsv
)
{
	DX_System* pSystem = DX_System::GetInstance();
	ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();
	ID3D11RenderTargetView* pClearRtv = pRtv ? pRtv : pSystem->GetRenderTargetView();
	ID3D11DepthStencilView* pClearDsv = pDsv ? pDsv : pSystem->GetDepthStencilView();
	ID3D11RenderTargetView* const targets[1] = { pClearRtv };

	//	DepthStencilViewの何をクリアするか
	UINT flags = 0;

	//	フラグによってクリアするものを変更する
	if (bZClear) { flags = D3D11_CLEAR_DEPTH; }
	if (bStencilClear) { flags |= D3D11_CLEAR_STENCIL; }

	//	RenderTargetViewをクリアする
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pDeviceContext->ClearRenderTargetView(pClearRtv, clearColor);

	//	DepthStencilViewをクリアする
	pDeviceContext->ClearDepthStencilView(pClearDsv, flags, 1.0f, 0);

	pDeviceContext->OMSetRenderTargets(1, targets, pClearDsv);

}

//-----------------------------------------------------------------------------------------
//
//	ビューポートを設定する
//
//-----------------------------------------------------------------------------------------
void View::SetViewPort()
{
	D3D11_TEXTURE2D_DESC desc;
	DX_System::GetInstance()->GetDepthStencilBuffer()->GetDesc(&desc);
	m_viewPort.TopLeftX = 0;			//	ビューポート領域の左上X座標
	m_viewPort.TopLeftY = 0;			//	ビューポート領域の左上Y座標				
	m_viewPort.Width = static_cast<FLOAT>(desc.Width);	//	ビューポート領域の幅
	m_viewPort.Height = static_cast<FLOAT>(desc.Height);	//	ビューポート領域の高さ
	m_viewPort.MinDepth = 0.0f;			//	ビューポート領域の深度値の最小値
	m_viewPort.MaxDepth = 1.0f;			//	ビューポート領域の深度値の最大値
}

//-----------------------------------------------------------------------------------------
//
//	投影に必要な情報を設定する
//
//-----------------------------------------------------------------------------------------
void View::SetProjection(
	const float fov,
	const float znear,
	const float zFar
)
{
	m_fov.fovY = fov;
	m_fov.znear = znear;
	m_fov.zfar = zFar;
	m_fov.aspect = (m_viewPort.Width / m_viewPort.Height);

	m_bChanged = true;
}


//-----------------------------------------------------------------------------------------
//
//	viewに関する行列を送る
//
//-----------------------------------------------------------------------------------------
void View::SetMatrixForTheView()
{
	DirectX::XMFLOAT4X4 mat[] = {
		m_matView,
		m_matProj,
		m_matViewProj
	};

	//	行列を送る
	DX_ShaderManager::GetInstance()->SetMatrix(m_constantBuffer.GetAddressOf(), 0, mat, 1, SHADER_TYPE::VERTEX_SHADER);
}

//-----------------------------------------------------------------------------------------
//
//	viewportを取得sうる
//
//-----------------------------------------------------------------------------------------
D3D11_VIEWPORT* View::GetViewPort()
{
	return &m_viewPort;
}

//-----------------------------------------------------------------------------------------
//
//	座標を設定
//
//-----------------------------------------------------------------------------------------
void View::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_bChanged = true;
}


//-----------------------------------------------------------------------------------------
//
//	座標を設定
//
//-----------------------------------------------------------------------------------------
void View::SetTarget(const DirectX::XMFLOAT3& target)
{
	m_target = target;
	m_bChanged = true;

}


//-----------------------------------------------------------------------------------------
//
//	座標を取得
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 View::GetPos()const
{
	return m_pos;
}

//-----------------------------------------------------------------------------------------
//
//	ターゲットを取得
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 View::GetTarget()const
{
	return m_target;
}

//-----------------------------------------------------------------------------------------
//
//	視錐台の6つの面を作成
//
//-----------------------------------------------------------------------------------------
void View::CreateFrustum(const int updateFrameNum)
{
	//	引数と同じ数になったら初期化し、return
	if (m_updateFrameNum == updateFrameNum) {
		m_updateFrameNum = 0;
		return;
	}
	++m_updateFrameNum;

	//	near place of frustum
	m_plane[0].x = m_matViewProj._14 + m_matViewProj._13;
	m_plane[0].y = m_matViewProj._24 + m_matViewProj._23;
	m_plane[0].z = m_matViewProj._34 + m_matViewProj._33;
	m_plane[0].w = m_matViewProj._44 + m_matViewProj._43;

	//	far plane of frustum
	m_plane[1].x = m_matViewProj._14 - m_matViewProj._13;
	m_plane[1].y = m_matViewProj._24 - m_matViewProj._23;
	m_plane[1].z = m_matViewProj._34 - m_matViewProj._33;
	m_plane[1].w = m_matViewProj._44 - m_matViewProj._43;

	//	right plane of frustum
	m_plane[2].x = m_matViewProj._14 - m_matViewProj._11;
	m_plane[2].y = m_matViewProj._24 - m_matViewProj._21;
	m_plane[2].z = m_matViewProj._34 - m_matViewProj._31;
	m_plane[2].w = m_matViewProj._44 - m_matViewProj._41;

	//	left plane of frustum
	m_plane[3].x = m_matViewProj._14 + m_matViewProj._11;
	m_plane[3].y = m_matViewProj._24 + m_matViewProj._21;
	m_plane[3].z = m_matViewProj._34 + m_matViewProj._31;
	m_plane[3].w = m_matViewProj._44 + m_matViewProj._41;

	// top plane of frustum
	m_plane[4].x = m_matViewProj._14 - m_matViewProj._12;
	m_plane[4].y = m_matViewProj._24 - m_matViewProj._22;
	m_plane[4].z = m_matViewProj._34 - m_matViewProj._32;
	m_plane[4].w = m_matViewProj._44 - m_matViewProj._42;

	// top bottom of frustum
	m_plane[5].x = m_matViewProj._14 + m_matViewProj._12;
	m_plane[5].y = m_matViewProj._24 + m_matViewProj._22;
	m_plane[5].z = m_matViewProj._34 + m_matViewProj._32;
	m_plane[5].w = m_matViewProj._44 + m_matViewProj._42;



	//	全面正規化
	DirectX::XMVECTOR pos;
	for (int i = 0; i < 6; ++i) {
		pos = XMLoadFloat4(&m_plane[i]);
		XMStoreFloat4(&m_plane[i], DirectX::XMVector4Normalize(pos));
	}
}
//-----------------------------------------------------------------------------------------
//
//	pointが視錐台に中にいるかをチェック
//
//-----------------------------------------------------------------------------------------
bool View::IsCheckPointInFrustum(const DirectX::XMFLOAT3& argPos)
{

	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR vec;
	DirectX::XMVECTOR result;
	for (int i = 0; i < 6; ++i) {
		pos = XMLoadFloat4(&m_plane[i]);
		vec = XMLoadFloat3(&argPos);
		result = DirectX::XMPlaneDotCoord(pos, vec);

		/*	if (PlaneDotCoord(m_plane[i], pos) < 0.0f){
				return false;
			}*/
	}
	return true;
}

//-----------------------------------------------------------------------------------------
//
//	ボックスが視錐台にいるかをチェック
//
//-----------------------------------------------------------------------------------------
bool View::IsCheckCubeInFrustum(const DirectX::XMFLOAT3& center, const float radius)
{
	for (int i = 0; i < 6; ++i) {

		////	左下手前の座標
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x - radius, center.y - radius, center.z - radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	右下手前の座標
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x + radius, center.y - radius, center.z - radius)) >= 0.0f)
		//{1>c:\users\junik\documents\github\__friends_dg\directx11\source\dxlibrary\View.cpp(345): warning C4702: 制御が渡らないコードです。

		//	continue;
		//}

		////	左上手前の座標
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x - radius, center.y + radius, center.z - radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	右上手前の座標
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x + radius, center.y + radius, center.z - radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	左下奥の座標
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x - radius, center.y - radius, center.z + radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	右下奥の座標
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x + radius, center.y - radius, center.z + radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	左上奥の座標
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x - radius, center.y + radius, center.z + radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	右上奥の座標
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x + radius, center.y + radius, center.z + radius)) >= 0.0f)
		//{
		//	continue;
		//}

		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	球体が視錐台にいるかををチェック
//
//-----------------------------------------------------------------------------------------
bool View::IsCheckSphereInFrustum(const DirectX::XMFLOAT3& center, const float radius)
{
	
	//for (int i = 0; i < 6; ++i){
	//	if (PlaneDotCoord(m_plane[i], center) < -radius){
	//		return false;
	//	}
	//}  
	return true;
}


