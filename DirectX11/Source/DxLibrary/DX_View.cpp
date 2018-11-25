#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_View* DX_View::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_View::DX_View() :
m_pos(0.0f,20,-50.0f),
m_target(0.0f,20.0f, 0.0f),
m_upDirection(0.0f, 1.0f, 0.0f),
m_bChanged(true),
m_updateFrameNum(0),
m_pConstantBuffer(nullptr)
{
	//	�����o�ϐ�������
	ZeroMemory(&m_viewPort, sizeof(m_viewPort));
	ZeroMemory(&m_matView, sizeof(m_matView));
	ZeroMemory(&m_matProj, sizeof(m_matProj));
	ZeroMemory(&m_matViewProj, sizeof(m_matViewProj));
	ZeroMemory(&m_viewInfo, sizeof(m_viewInfo));
	ZeroMemory(m_plane, sizeof(m_plane));
	
	//	�r���[�|�[�g�ƃv���W�F�N�V������ݒ肷��
	SetViewPort();
	SetProjection(VIEW_DEFAULT_ASPECT, VIEW_DEFAULT_ZNEAR, VIEW_DEFAULT_ZFAR);

	//	�s����Ɍv�Z
	Active();

	//	������̖ʂ��쐬
	CreateFrustum();

	m_pConstantBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(DirectX::XMFLOAT4X4) * 3);
}


//-----------------------------------------------------------------------------------------
//
//	���
//
//-----------------------------------------------------------------------------------------
DX_View::~DX_View()
{
	SAFE_RELEASE(m_pConstantBuffer);
}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���擾����
//
//-----------------------------------------------------------------------------------------
DX_View* DX_View::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_View();
	}

	return m_pInstance;
}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���������
//
//-----------------------------------------------------------------------------------------
void DX_View::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//	�s����v�Z���ARS��Viewport��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_View::Active()
{
	//	�s��쐬�Ɋւ��ϐ����ω������ꍇtrue�ɂȂ�A�s�񂪍Čv�Z�����
	if (m_bChanged){

		//	�r���[�s��ݒ�
		DirectX::XMVECTOR pos		= XMLoadFloat3(&m_pos);
		DirectX::XMVECTOR target	= XMLoadFloat3(&m_target);
		DirectX::XMVECTOR up		= XMLoadFloat3(&m_upDirection);

		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);
		XMStoreFloat4x4(&m_matView, view);

		//	���e�s��ϊ�
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(m_viewInfo.fovY, m_viewInfo.aspect, m_viewInfo.znear, m_viewInfo.zfar);
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
void DX_View::Clear(
	const bool bZClear,
	const bool bStencilClear,
	ID3D11RenderTargetView* pRtv,
	ID3D11DepthStencilView* pDsv
	)
{
	DX_System* l_pSystem = DX_System::GetInstance();
	ID3D11DeviceContext* l_pDeviceContext = l_pSystem->GetDeviceContext();
	ID3D11RenderTargetView* pClearRtv = pRtv ? pRtv : l_pSystem->GetDefaultRenderTargetView();
	ID3D11DepthStencilView* pClearDsv = pDsv ? pDsv : l_pSystem->GetDefaultDepthStencilView();

	//	DepthStencilView�̉����N���A���邩
	UINT flags = 0;

	//	�t���O�ɂ���ăN���A������̂�ύX����
	if (bZClear){ flags = D3D11_CLEAR_DEPTH; }
	if (bStencilClear){ flags |= D3D11_CLEAR_STENCIL; }
	

	//	RenderTargetView���N���A����
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	l_pDeviceContext->ClearRenderTargetView(pClearRtv, clearColor);

	//	DepthStencilView���N���A����
	l_pDeviceContext->ClearDepthStencilView(pClearDsv, flags, 1.0f,0);
}

//-----------------------------------------------------------------------------------------
//
//	�r���[�|�[�g��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_View::SetViewPort()
{
	D3D11_TEXTURE2D_DESC desc;
	DX_System::GetInstance()->GetDepthStencilBuffer()->GetDesc(&desc);
	m_viewPort.TopLeftX = 0;			//	�r���[�|�[�g�̈�̍���X���W
	m_viewPort.TopLeftY = 0;			//	�r���[�|�[�g�̈�̍���Y���W				
	m_viewPort.Width	= static_cast<FLOAT>(desc.Width);	//	�r���[�|�[�g�̈�̕�
	m_viewPort.Height	= static_cast<FLOAT>(desc.Height);	//	�r���[�|�[�g�̈�̍���
	m_viewPort.MinDepth = 0.0f;			//	�r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	m_viewPort.MaxDepth = 1.0f;			//	�r���[�|�[�g�̈�̐[�x�l�̍ő�l
}

//-----------------------------------------------------------------------------------------
//
//	���e�ɕK�v�ȏ���ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_View::SetProjection(
	const float fov,
	const float znear,
	const float zFar
	)
{
	m_viewInfo.fovY		= fov;
	m_viewInfo.znear	= znear;
	m_viewInfo.zfar		= zFar;
	m_viewInfo.aspect	= (m_viewPort.Width / m_viewPort.Height);

	m_bChanged = true;
}


//-----------------------------------------------------------------------------------------
//
//	view�Ɋւ���s��𑗂�
//
//-----------------------------------------------------------------------------------------
void DX_View::SetMatrixForTheView()
{
	DirectX::XMFLOAT4X4 l_mat[] = {
		m_matView,
		m_matProj,
		m_matViewProj
	};

	//	�s��𑗂�
	DX_ShaderManager::GetInstance()->SetMatrix(m_pConstantBuffer, 0, l_mat, 3, DX_System::GetInstance()->DX_System::GetDeviceContext(), DX_SHADER_TYPE::VERTEX_SHADER);
}

//-----------------------------------------------------------------------------------------
//
//	viewport���擾s����
//
//-----------------------------------------------------------------------------------------
D3D11_VIEWPORT* DX_View::GetViewPort()
{ 
	return &m_viewPort;
}

//-----------------------------------------------------------------------------------------
//
//	���W��ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_View::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_pos		= pos;
	m_bChanged	= true;
}


//-----------------------------------------------------------------------------------------
//
//	���W��ݒ�
//
//-----------------------------------------------------------------------------------------
void DX_View::SetTarget(const DirectX::XMFLOAT3& target)
{
	m_target	= target;
	m_bChanged	= true;

}


//-----------------------------------------------------------------------------------------
//
//	���W���擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 DX_View::GetPos()const
{
	return m_pos;
}

//-----------------------------------------------------------------------------------------
//
//	�^�[�Q�b�g���擾
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT3 DX_View::GetTarget()const
{
	return m_target;
}

//-----------------------------------------------------------------------------------------
//
//	�������6�̖ʂ��쐬
//
//-----------------------------------------------------------------------------------------
void DX_View::CreateFrustum(const int updateFrameNum)
{
	//	�����Ɠ������ɂȂ����珉�������Areturn
	if (m_updateFrameNum == updateFrameNum){
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
	for (int i = 0; i < 6; ++i){
		pos = XMLoadFloat4(&m_plane[i]);
		XMStoreFloat4(&m_plane[i], DirectX::XMVector4Normalize(pos));
	}
}
//-----------------------------------------------------------------------------------------
//
//	point��������ɒ��ɂ��邩���`�F�b�N
//
//-----------------------------------------------------------------------------------------
bool DX_View::IsCheckPointInFrustum(const DirectX::XMFLOAT3& argPos)
{
	
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR vec;
	DirectX::XMVECTOR result;
	for (int i = 0; i < 6; ++i){
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
bool DX_View::IsCheckCubeInFrustum(const DirectX::XMFLOAT3& center, const float radius)
{
	for (int i = 0; i < 6; ++i){

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
		//{
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
bool DX_View::IsCheckSphereInFrustum(const DirectX::XMFLOAT3& center, const float radius)
{
	//for (int i = 0; i < 6; ++i){
	//	if (PlaneDotCoord(m_plane[i], center) < -radius){
	//		return false;
	//	}
	//}  
	return true;
}


//-----------------------------------------------------------------------------------------
//
//	���L�[�ŃJ�������ړ�
//
//-----------------------------------------------------------------------------------------
void	DX_View::FreeCamera(const float moveSpeed)
{
	DirectX::XMFLOAT3 l_move(0.0f, 0.0f, 0.0f);

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_UP))	{ l_move.z += 1.0f; }
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_DOWN))	{ l_move.z -= 1.0f; }
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_LEFT))	{ l_move.x -= 1.0f; }
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RIGHT)){ l_move.x += 1.0f; }

	
	//l_move *= moveSpeed;

	//m_pos		+= l_move;
	//m_target	+= l_move;

	m_bChanged = true;

}