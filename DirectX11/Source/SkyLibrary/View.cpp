#include	"SkyLibrary.h"

#define VIEW_DEFAULT_ASPECT  DX::CAST::F(PI * 0.25f)
#define VIEW_DEFAULT_ZNEAR  DX::CAST::F(1.0f)
#define VIEW_DEFAULT_ZFAR	DX::CAST::F(1000.0f)

//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
View::View() :
	m_pos(0.0f, 20, -50.0f),
	m_target(0.0f, 20.0f, 0.0f),
	m_upDirection(0.0f, 1.0f, 0.0f),
	m_bChanged(true),
	m_updateFrameNum(0)
{
	//	�����o�ϐ�������
	ZeroMemory(&m_viewPort, sizeof(m_viewPort));
	ZeroMemory(&m_matView, sizeof(m_matView));
	ZeroMemory(&m_matProj, sizeof(m_matProj));
	ZeroMemory(&m_matViewProj, sizeof(m_matViewProj));
	ZeroMemory(&m_fov, sizeof(m_fov));
	ZeroMemory(m_plane, sizeof(m_plane));

	//	�r���[�|�[�g�ƃv���W�F�N�V������ݒ肷��
	SetViewPort();
	SetProjection(VIEW_DEFAULT_ASPECT, VIEW_DEFAULT_ZNEAR, VIEW_DEFAULT_ZFAR);

	//	�s����Ɍv�Z
	Active();

	//	������̖ʂ��쐬
	CreateFrustum();

	m_constantBuffer.Attach(DX_BufferCreater::ConstantBuffer(sizeof(DirectX::XMFLOAT4X4) * 3));
	m_constantBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_constantBuffer") - 1, "m_constantBuffer");

}


//-----------------------------------------------------------------------------------------
//
//	���
//
//-----------------------------------------------------------------------------------------
View::~View()
{
}

//-----------------------------------------------------------------------------------------
//
//	�s����v�Z���ARS��Viewport��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void View::Active()
{
	//	�s��쐬�Ɋւ��ϐ����ω������ꍇtrue�ɂȂ�A�s�񂪍Čv�Z�����
	if (m_bChanged) {

		//	�r���[�s��ݒ�
		DirectX::XMVECTOR pos = XMLoadFloat3(&m_pos);
		DirectX::XMVECTOR target = XMLoadFloat3(&m_target);
		DirectX::XMVECTOR up = XMLoadFloat3(&m_upDirection);

		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);
		XMStoreFloat4x4(&m_matView, view);

		//	���e�s��ϊ�
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(m_fov.fovY, m_fov.aspect, m_fov.znear, m_fov.zfar);
		XMStoreFloat4x4(&m_matProj, proj);


		//	viewProj�s����쐬
		DirectX::XMMATRIX viwProj = view * proj;
		XMStoreFloat4x4(&m_matViewProj, viwProj);

		//	RS�Ƀr���[�|�[�g��ݒ�
		DX_System::GetInstance()->GetDeviceContext()->RSSetViewports(1, GetViewPort());

		//	�t���O��false�ɂ���
		m_bChanged = false;
	}
}

//-----------------------------------------------------------------------------------------
//
//	RenderTargetView��DepthStencileView���N���A����
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

	//	DepthStencilView�̉����N���A���邩
	UINT flags = 0;

	//	�t���O�ɂ���ăN���A������̂�ύX����
	if (bZClear) { flags = D3D11_CLEAR_DEPTH; }
	if (bStencilClear) { flags |= D3D11_CLEAR_STENCIL; }

	//	RenderTargetView���N���A����
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pDeviceContext->ClearRenderTargetView(pClearRtv, clearColor);

	//	DepthStencilView���N���A����
	pDeviceContext->ClearDepthStencilView(pClearDsv, flags, 1.0f, 0);

	pDeviceContext->OMSetRenderTargets(1, targets, pClearDsv);

}

//-----------------------------------------------------------------------------------------
//
//	�r���[�|�[�g��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void View::SetViewPort()
{
	D3D11_TEXTURE2D_DESC desc;
	DX_System::GetInstance()->GetDepthStencilBuffer()->GetDesc(&desc);
	m_viewPort.TopLeftX = 0;			//	�r���[�|�[�g�̈�̍���X���W
	m_viewPort.TopLeftY = 0;			//	�r���[�|�[�g�̈�̍���Y���W				
	m_viewPort.Width = static_cast<FLOAT>(desc.Width);	//	�r���[�|�[�g�̈�̕�
	m_viewPort.Height = static_cast<FLOAT>(desc.Height);	//	�r���[�|�[�g�̈�̍���
	m_viewPort.MinDepth = 0.0f;			//	�r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	m_viewPort.MaxDepth = 1.0f;			//	�r���[�|�[�g�̈�̐[�x�l�̍ő�l
}

//-----------------------------------------------------------------------------------------
//
//	���e�ɕK�v�ȏ���ݒ肷��
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
//	view�Ɋւ���s��𑗂�
//
//-----------------------------------------------------------------------------------------
void View::SetMatrixForTheView()
{
	DirectX::XMFLOAT4X4 mat[] = {
		m_matView,
		m_matProj,
		m_matViewProj
	};

	//	�s��𑗂�
	DX_ShaderManager::GetInstance()->SetMatrix(m_constantBuffer.GetAddressOf(), 0, mat, 1, SHADER_TYPE::VERTEX_SHADER);
}

//-----------------------------------------------------------------------------------------
//
//	viewport���擾s����
//
//-----------------------------------------------------------------------------------------
D3D11_VIEWPORT* View::GetViewPort()
{
	return &m_viewPort;
}

//-----------------------------------------------------------------------------------------
//
//	���W��ݒ�
//
//-----------------------------------------------------------------------------------------
void View::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos = pos;
	m_bChanged = true;
}


//-----------------------------------------------------------------------------------------
//
//	���W��ݒ�
//
//-----------------------------------------------------------------------------------------
void View::SetTarget(const DirectX::XMFLOAT3& target)
{
	m_target = target;
	m_bChanged = true;

}


//-----------------------------------------------------------------------------------------
//
//	���W���擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 View::GetPos()const
{
	return m_pos;
}

//-----------------------------------------------------------------------------------------
//
//	�^�[�Q�b�g���擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 View::GetTarget()const
{
	return m_target;
}

//-----------------------------------------------------------------------------------------
//
//	�������6�̖ʂ��쐬
//
//-----------------------------------------------------------------------------------------
void View::CreateFrustum(const int updateFrameNum)
{
	//	�����Ɠ������ɂȂ����珉�������Areturn
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



	//	�S�ʐ��K��
	DirectX::XMVECTOR pos;
	for (int i = 0; i < 6; ++i) {
		pos = XMLoadFloat4(&m_plane[i]);
		XMStoreFloat4(&m_plane[i], DirectX::XMVector4Normalize(pos));
	}
}
//-----------------------------------------------------------------------------------------
//
//	point��������ɒ��ɂ��邩���`�F�b�N
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
//	�{�b�N�X��������ɂ��邩���`�F�b�N
//
//-----------------------------------------------------------------------------------------
bool View::IsCheckCubeInFrustum(const DirectX::XMFLOAT3& center, const float radius)
{
	for (int i = 0; i < 6; ++i) {

		////	������O�̍��W
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x - radius, center.y - radius, center.z - radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	�E����O�̍��W
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x + radius, center.y - radius, center.z - radius)) >= 0.0f)
		//{1>c:\users\junik\documents\github\__friends_dg\directx11\source\dxlibrary\View.cpp(345): warning C4702: ���䂪�n��Ȃ��R�[�h�ł��B

		//	continue;
		//}

		////	�����O�̍��W
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x - radius, center.y + radius, center.z - radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	�E���O�̍��W
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x + radius, center.y + radius, center.z - radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	�������̍��W
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x - radius, center.y - radius, center.z + radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	�E�����̍��W
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x + radius, center.y - radius, center.z + radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	���㉜�̍��W
		//if (PlaneDotCoord(
		//	m_plane[i],
		//	Vector3(center.x - radius, center.y + radius, center.z + radius)) >= 0.0f)
		//{
		//	continue;
		//}

		////	�E�㉜�̍��W
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
//	���̂�������ɂ��邩�����`�F�b�N
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


