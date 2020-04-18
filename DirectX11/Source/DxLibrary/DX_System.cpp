#include	"DX_Library.h"
//#include	"../user_helper_class/ALManager.h"
//#include	"../user_helper_class/OGGManager.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ�������������
//
//-----------------------------------------------------------------------------------------
DX_System::DX_System() :
	m_featureLevel(D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1),
	m_driverType(D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN),
	m_windowHandle(NULL),
	m_screenHeight(0),
	m_screenWidth(0)
{}

//-----------------------------------------------------------------------------------------
//
//  DirectX������������
//
//-----------------------------------------------------------------------------------------
bool DX_System::Initialize(HWND hWnd, HINSTANCE hInst)
{
	//	�E�B���h�E�n���h����ۑ�
	m_windowHandle = hWnd;

	//	DX_Graphics������
	if (!DX_Graphics::Initialize()) {
		return false;
	}


	//	SwapChain���쐬����
	if (!CreateDeviceAndSwapChain(hWnd)) {
		return false;
	}

	//	�f�o�b�O�f�o�C�X���쐬����
	DX_Debug::Create();
	DX_Debug::GetInstance()->Initialize();

	// �o�b�N�o�b�t�@���쐬����
	if (!InitBuckBuffer()) {
		return false;
	}

	//	�����_�[�X�e�[�g������������
	DX_RenderState::Create();
	if (!DX_RenderState::GetInstance()->Initialize()) {
		return false;
	}

	//	�V�F�[�_�[��������
	DX_ShaderManager::Initialize();

	// DX_TextureManager������������
	DX_TextureManager::Initialize();

	// DirectInput��������
	if (!DX_Input::Initialize(hWnd, hInst)) {
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	DirectX�֘A�@���
//
//-----------------------------------------------------------------------------------------
DX_System::~DX_System()
{
	m_swapChain->SetFullscreenState(FALSE, nullptr);
	m_deviceContext->ClearState();
	m_deviceContext->Flush();

	//DX_SceneManager::Release();
	DX_Input::Release();
	DX_RenderState::Destroy();
	DX_ShaderManager::Release();
	DX_TextureManager::AllRelease();
	
	//	OGG�t�@�C�������
	//	OGGManager::Release();

	//	ALManager���������
	//ALManager::Release();
	//DX_Debug::Destroy();
}


//-----------------------------------------------------------------------------------------
//
//  �X�N���[���̃T�C�Y��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_System::SetScreenSize(const unsigned int width, const unsigned int height)
{
	m_screenWidth	= width;
	m_screenHeight	= height;
}

//-----------------------------------------------------------------------------------------
//
//  �X�N���[���̍������擾
//
//-----------------------------------------------------------------------------------------
unsigned int DX_System::GetScreenHeight()
{
	return m_screenHeight; 
}

//-----------------------------------------------------------------------------------------
//
//  �X�N���[���̕����擾
//
//-----------------------------------------------------------------------------------------
unsigned int DX_System::GetScreenWidth()
{
	return m_screenWidth; 
}

//-----------------------------------------------------------------------------------------
//
//  �f�o�C�X���擾
//
//-----------------------------------------------------------------------------------------
ID3D11Device* DX_System::GetDevice()
{
	return m_device.Get();
}

//-----------------------------------------------------------------------------------------
//
//  �f�o�C�X�R���e�L�X�g���擾
//
//-----------------------------------------------------------------------------------------
ID3D11DeviceContext*	DX_System::GetDeviceContext()
{
	return m_deviceContext.Get();
}

//-----------------------------------------------------------------------------------------
//
//  �X���b�v�`�F�C�����擾
//
//-----------------------------------------------------------------------------------------
IDXGISwapChain*		DX_System::GetSwapChain()
{
	return m_swapChain.Get();
}

//-----------------------------------------------------------------------------------------
//
//  FeatureLevel���擾
//
//-----------------------------------------------------------------------------------------
D3D_FEATURE_LEVEL DX_System::GetFeatureLevel()
{
	return m_featureLevel;
}

//-----------------------------------------------------------------------------------------
//
//  DriverType���擾
//
//-----------------------------------------------------------------------------------------
D3D_DRIVER_TYPE DX_System::GetDriverType()
{
	return m_driverType;
}

//-----------------------------------------------------------------------------------------
//
//  �����_�[�^�[�Q�b�g���擾
//
//-----------------------------------------------------------------------------------------
ID3D11RenderTargetView*	DX_System::GetRenderTargetView()
{
	return m_rtv.Get();
}

//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���r���[���擾
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilView*	DX_System::GetDepthStencilView()
{
	return m_dsv.Get();
}


//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���o�b�t�@���擾
//
//-----------------------------------------------------------------------------------------
ID3D11Texture2D*		DX_System::GetDepthStencilBuffer()
{
	return m_dsb.Get();
}

//-----------------------------------------------------------------------------------------
//
//  �E�B���h�E�n���h�����擾
//
//-----------------------------------------------------------------------------------------
HWND	DX_System::GetWindowHandle()
{
	return m_windowHandle;
}

//-----------------------------------------------------------------------------------------
//
//  �o�b�N�o�b�t�@�̏��������s��
//
//-----------------------------------------------------------------------------------------
bool DX_System::InitBuckBuffer()
{
	//	RenderTargetView���쐬����
	if (!CreateRenderTargetView()) {
		return false;
	}

	//	DepthStencilBuffer���쐬����
	if (!CreateDepthStencilBuffer()) {
		return false;
	}

	//	DepthStencilView���쐬����
	if (!CreateDepthStencilView()) {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  ���T�C�Y����
//
//-----------------------------------------------------------------------------------------
bool DX_System::BufferResize(const WORD width, const WORD height)
{
	m_screenWidth	= DX::CAST::UI(width);
	m_screenHeight	= DX::CAST::UI(height);
	ID3D11RTV* const pNullRTV[] = { nullptr };
	ID3D11DSV* const pNullDSV = nullptr;
	m_deviceContext->OMSetRenderTargets(1, pNullRTV, pNullDSV);

	m_rtv.Reset();
	m_dsv.Reset();
	m_dsb.Reset();

	DX_Debug* pDebug = DX_Debug::GetInstance();
	DXGI_SWAP_CHAIN_DESC desc;
	if (!pDebug->CheckHresult(m_swapChain->GetDesc(&desc))) {
		return false;
	}
	if (!pDebug->CheckHresult(m_swapChain->ResizeBuffers(desc.BufferCount, DX::CAST::UI(width), DX::CAST::UI(height), DX_Graphics::GetFortmat(), 0))) {
		return false;
	}
	
	if (!InitBuckBuffer()) {
		return false;
	}

	ID3D11RTV* const pRrv[] = { m_rtv.Get() };
	ID3D11DSV* const pDsv = m_dsv.Get();
	m_deviceContext->OMSetRenderTargets(1, pRrv, pDsv);

	return true;
}
//-----------------------------------------------------------------------------------------
//
//  SwapChain,ID3D11Device,ID3D11DeviceContext���쐬
//
//-----------------------------------------------------------------------------------------
bool DX_System::CreateDeviceAndSwapChain(const HWND& hWnd)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };

	//	�o�b�N�o�b�t�@��
	swapChainDesc.BufferCount = 2;

	//	�o�b�N�o�b�t�@�̕��ƍ���
	swapChainDesc.BufferDesc.Width	= m_screenWidth;
	swapChainDesc.BufferDesc.Height	= m_screenHeight;

	//	�t�H�[�}�b�g
	swapChainDesc.BufferDesc.Format = DX_Graphics::GetFortmat();

	//	���t���b�V�����[�g�̕���ƕ��q
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= DX_Graphics::GetRefreshRateN();
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= DX_Graphics::GetRefreshRateD();

	//	�X�L�������C���ƃX�P�[�����O
	swapChainDesc.BufferDesc.ScanlineOrdering	= DX_Graphics::GetScanLineOrder();
	swapChainDesc.BufferDesc.Scaling			= DX_Graphics::GetScaling();

	// �o�b�N �o�b�t�@�̎g�p�@
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �֘A�t����E�C���h�E
	swapChainDesc.OutputWindow = hWnd;
	
	//	�}���`�T���v���̐��ƃN�I���e�B(conut = 1 quality=0���ƃA���`�G�C���A�X�����͍s��Ȃ�)
	// ���A���`�G�C���A�X�������s���ĕ`�悷��ۂ́A�`��^�[�Q�b�g�Ɛ[�x�o�b�t�@�̃}���`�T���v�����ƃN�I���e�B���x������v���Ă��Ȃ���΂Ȃ�Ȃ��B
	// �܂��A�A���`�G�C���A�X���g�p����ꍇ�ACheckMultisampleQualityLevels()�Ŏg�p�ł���N�I���e�B���x�����`�F�b�N���邱�ƁB
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;

	//	�E�B���h�E���[�h
	swapChainDesc.Windowed = TRUE;

	// ���[�h�����؂�ւ�
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	// ��ʂ��X�V����ہA�o�b�N�o�b�t�@�̓��e�͂ǂ����邩�H
	// DXGI_SWAP_EFFECT_DISCARD��DXGI_SWAP_EFFECT_SEQUENTIAL�͎g�p���Ȃ��ق����������ۂ��B
	// https://devblogs.microsoft.com/directx/dxgi-flip-model/ 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	
	//	GPU���x��
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	//	�n�[�h�^�C�v
	D3D_DRIVER_TYPE driveTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_SOFTWARE, 
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_NULL,
	};

#if defined(DEBUG) || defined(_DEBUG)
	UINT createDeviecFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createDeviecFlags = 0;
#endif

	HRESULT hr = S_FALSE;
	for (auto i = 0; i < _countof(driveTypes); ++i){
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driveTypes[i],
			0,
			createDeviecFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&m_swapChain,
			&m_device,
			&m_featureLevel,
			&m_deviceContext
			);

		if (SUCCEEDED(hr)){
			m_driverType = driveTypes[i];
			break;
		}
	}

	if (!DX_Debug::GetInstance()->CheckHresult(hr)){
		TRACE("failed to D3D11CreateDeviceAndSwapChain()");
		return false;
	}

	// TODO:SetPrivateData
	m_device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_device") - 1, "m_device");
	m_deviceContext->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_deviceContext") - 1, "m_deviceContext");
	m_swapChain->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_swapChain") - 1, "m_swapChain");

	return true;
}


//-----------------------------------------------------------------------------------------
//
//  �����f�[�^�[�Q�b�g�r���[���쐬
//
//-----------------------------------------------------------------------------------------
bool DX_System::CreateRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buckBuffer;

	//	�o�b�N�o�b�t�@���擾
	if (!DX_Debug::GetInstance()->CheckHresult(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buckBuffer))){
		TRACE("failed to GetBuffer()");
		return false;
	}

	//	�����_�[�^�[�Q�b�g�r���[���쐬����
	if (!DX_Debug::GetInstance()->CheckHresult(m_device->CreateRenderTargetView(buckBuffer.Get(), nullptr, &m_rtv))){
		TRACE("failed to CreateRenderTargetView()");
		return false;
	}

	// TODO:SetPrivateData
	m_rtv->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_rtv") - 1, "m_rtv");

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	�[�x��X�e���V���o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
bool DX_System::CreateDepthStencilBuffer()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buckBuffer;

	//	�o�b�N�o�b�t�@���擾
	if (!DX_Debug::GetInstance()->CheckHresult(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buckBuffer))) {
		TRACE("faield to GetBuffer()");
		return false;
	}

	// �o�b�N�E�o�b�t�@�̏��
	D3D11_TEXTURE2D_DESC descBackBuffer;
	buckBuffer->GetDesc(&descBackBuffer);

	D3D11_TEXTURE2D_DESC dsbDesc = descBackBuffer;
	dsbDesc.MipLevels			= 1;
	dsbDesc.ArraySize			= 1;
	dsbDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	dsbDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	dsbDesc.Usage				= D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	dsbDesc.CPUAccessFlags		= 0;   // CPU����̓A�N�Z�X���Ȃ�
	dsbDesc.MiscFlags			= 0;   // ���̑��̐ݒ�Ȃ�

	//	�[�x��X�e���V���o�b�t�@���쐬
	if (!DX_Debug::GetInstance()->CheckHresult(m_device->CreateTexture2D(&dsbDesc, nullptr, &m_dsb))){
		TRACE("failed to CreateTexture2D()");
		return false;
	}

	// TODO:SetPrivateData
	m_dsb->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_dsb") - 1, "m_dsb");

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	�[�x��X�e���V���r���[���쐬����
//
//-----------------------------------------------------------------------------------------
bool DX_System::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC desc = { NULL };
	m_dsb->GetDesc(&desc);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format			= desc.Format;
	dsvDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;

	//	�[�x��X�e���V���r���[���쐬
	if (!DX_Debug::GetInstance()->CheckHresult(m_device->CreateDepthStencilView(m_dsb.Get(), &dsvDesc, &m_dsv))){
		TRACE("failed to CreateDepthStencilView()");
		return false;
	}

	// TODO:SetPrivateData
	m_dsv->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_dsv") - 1, "m_dsv");

	return true;
}

