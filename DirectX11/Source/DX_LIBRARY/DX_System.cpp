#include	"DX_Library.h"
//#include	"../user_helper_class/ALManager.h"
//#include	"../user_helper_class/OGGManager.h"
#include	<stdio.h>

using namespace Microsoft::WRL;
//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_System* DX_System::m_pInstnace = nullptr;
D3D_FEATURE_LEVEL		DX_System::m_featureLevel;
D3D_DRIVER_TYPE			DX_System::m_driverType;
HWND					DX_System::m_windowHandle = NULL;

unsigned int DX_System::m_windowWidth	= 0;
unsigned int DX_System::m_windowHeight	= 0;


DX_System::DX_System()
{

}

DX_System* DX_System::GetInstance()
{
	if (m_pInstnace == nullptr) {
		m_pInstnace = new DX_System();
	}

	return m_pInstnace;
}

DX_System::~DX_System()
{
	m_swapChain->SetFullscreenState(FALSE, nullptr);
	m_deviceContext->ClearState();
	//	�V�F�[�_�[�̉�����s��
	DX_ShaderManager::Release();
	DX_Debug::ReportLiveDeviceObjects("DX_ShaderManager::Release");

	//	�f�o�b�O�̉�����s��
	DX_Debug::ReportLiveDeviceObjects("DX_System::Release");
	DX_Debug::Release();

	//	OGG�t�@�C�������
	//	OGGManager::Release();

	//	ALManager���������
	//ALManager::Release();
}
//-----------------------------------------------------------------------------------------
//
//  DirectX������������
//
//-----------------------------------------------------------------------------------------
bool DX_System::InitD3D(const HWND& hWnd)
{
	//	�E�B���h�E�n���h����ۑ�
	m_windowHandle = hWnd;

	try{
		//	DX_Graphics��������
		DX_Graphics::Initialize();

		//	SwapChain���쐬����
		CreateDeviceAndSwapChain(hWnd);
		
		//	�f�o�b�O�f�o�C�X���쐬����
		DX_Debug::Initialize();
		DX_Debug::ReportLiveDeviceObjects("DX_Debug::Initialize()");
		//	RenderTargetView���쐬����
		CreateRenderTargetView();

		//	DepthStencilBuffer���쐬����
		CreateDepthStencilBuffer();

		//	DepthStencilView���쐬����
		CreateDepthStencilView();

		//	�����_�[�X�e�[�g������������
		DX_RenderState::Initialize();

		//	�V�F�[�_�[��������
		DX_ShaderManager::Initialize();

		//	ALManager��������
		//ALManager::Initialize();

	}
	catch (char* pErrorMessage){
		MessageBox(NULL, pErrorMessage, "error", MB_OK);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//	�f�o�C�X�Ȃǂ��������
//
//-----------------------------------------------------------------------------------------
void DX_System::Release()
{
	if (m_pInstnace) {
		delete m_pInstnace;
		m_pInstnace = nullptr;
	}
}

//-----------------------------------------------------------------------------------------
//
//  �E�B���h�E�̃T�C�Y��ݒ肷��
//
//-----------------------------------------------------------------------------------------
void DX_System::SetWindowsSize(CREATE_WINDOW_SIZE windowSize)
{
	switch (windowSize){
	case CREATE_WINDOW_SIZE::SCREEN_480p:
		m_windowWidth	= 720;
		m_windowHeight	= 480;
		break;

	case CREATE_WINDOW_SIZE::SCREEN_720p:
		m_windowWidth	= 1280;
		m_windowHeight	= 720;
		break;

	case CREATE_WINDOW_SIZE::SCREEN_1080p:
		m_windowWidth	= 1920;
		m_windowHeight	= 1080;
		break;

	default:
		break;
	}

}

//-----------------------------------------------------------------------------------------
//
//  �E�B���h�E�̍������擾
//
//-----------------------------------------------------------------------------------------
unsigned int DX_System::GetWindowHeight()
{
	return m_windowHeight; 
}

//-----------------------------------------------------------------------------------------
//
//  �E�B���h�E�̕����擾
//
//-----------------------------------------------------------------------------------------
unsigned int DX_System::GetWindowWidth()
{
	return m_windowWidth; 
}

//-----------------------------------------------------------------------------------------
//
//  �f�o�C�X���擾
//
//-----------------------------------------------------------------------------------------
ID3D11Device* DX_System::GetDevice() const
{
	return m_device.Get();
}

//-----------------------------------------------------------------------------------------
//
//  �f�o�C�X�R���e�L�X�g���擾
//
//-----------------------------------------------------------------------------------------
ID3D11DeviceContext*	DX_System::GetDeviceContext() const
{
	return m_deviceContext.Get();
}

//-----------------------------------------------------------------------------------------
//
//  �X���b�v�`�F�C�����擾
//
//-----------------------------------------------------------------------------------------
IDXGISwapChain*		DX_System::GetSwapChain() const
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
ID3D11RenderTargetView*	DX_System::GetDefaultRenderTargetView() const
{
	return m_rtv.Get();
}

//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���r���[���擾
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilView*	DX_System::GetDefaultDepthStencilView() const
{
	return m_dsv.Get();
}


//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���o�b�t�@���擾
//
//-----------------------------------------------------------------------------------------
ID3D11Texture2D*		DX_System::GetDepthStencilBuffer() const
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
//  SwapChain,ID3D11Device,ID3D11DeviceContext���쐬
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDeviceAndSwapChain(const HWND& hWnd)
{
	PROFILE("DX_System::CreateDeviceAndSwapChain()");

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };

	//	�o�b�N�o�b�t�@��
	swapChainDesc.BufferCount = 1;

	//	�o�b�N�o�b�t�@�̕��ƍ���
	swapChainDesc.BufferDesc.Width	= m_windowWidth;
	swapChainDesc.BufferDesc.Height	= m_windowHeight;

	//	�t�H�[�}�b�g
	swapChainDesc.BufferDesc.Format = DX_Graphics::GetFortmat();

	//	���t���b�V�����[�g�̕���ƕ��q
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= DX_Graphics::GetRefreshRateN();
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= DX_Graphics::GetRefreshRateD();

	//	�X�L�������C���ƃX�P�[�����O
	swapChainDesc.BufferDesc.ScanlineOrdering	= DX_Graphics::GetScanLineOrder();
	swapChainDesc.BufferDesc.Scaling			= DX_Graphics::GetScaling();;

	// �o�b�N �o�b�t�@�̎g�p�@
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �֘A�t����E�C���h�E
	swapChainDesc.OutputWindow = hWnd;

	//	�}���`�T���v���̐��ƃN�I���e�B
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;

	//	�E�B���h�E���[�h
	swapChainDesc.Windowed = TRUE;

	// ���[�h�����؂�ւ�
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


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
		D3D_DRIVER_TYPE_HARDWARE,	D3D_DRIVER_TYPE_SOFTWARE, D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_WARP,		D3D_DRIVER_TYPE_NULL,
	};

#if defined(DEBUG) || defined(_DEBUG)
	UINT l_createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT l_createDeviceFlags = 0;
#endif

	HRESULT hr = S_FALSE;
	for (UINT i = 0; i < _countof(driveTypes); ++i){
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driveTypes[i],
			0,
			l_createDeviceFlags,
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

	if (!DX_Debug::IsHresultCheck(hr)){
		throw "D3D11CreateDeviceAndSwapChain() : failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//  �����f�[�^�[�Q�b�g�r���[���쐬
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateRenderTargetView()
{
	PROFILE("DX_System::CreateRenderTargetView()");

	ComPtr<ID3D11Texture2D> buffer;

	//	�o�b�t�@���擾
	if (!DX_Debug::IsHresultCheck(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer))){
		throw "IDXGISwapChain::GetBuffer() : faield";
	}

	//	�����_�[�^�[�Q�b�g�r���[���쐬����
	if (!DX_Debug::IsHresultCheck(m_device->CreateRenderTargetView(buffer.Get(), nullptr, &m_rtv))){
		throw "ID3D11Device::CreateRenderTargetView() : faield";
	}
}


//-----------------------------------------------------------------------------------------
//
//	�[�x��X�e���V���o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDepthStencilBuffer()
{
	PROFILE("DX_System::CreateDepthStencilBuffer()");

	D3D11_TEXTURE2D_DESC dsbDesc = { NULL };

	dsbDesc.MipLevels		= 1;
	dsbDesc.ArraySize		= 1;
	dsbDesc.SampleDesc.Count = 1;
	dsbDesc.Width			= m_windowWidth;
	dsbDesc.Height			= m_windowHeight;
	dsbDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
	dsbDesc.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;

	//	�[�x��X�e���V���o�b�t�@���쐬
	if (!DX_Debug::IsHresultCheck(m_device->CreateTexture2D(&dsbDesc, nullptr, &m_dsb))){
		throw "ID3D11Device::CreateTexture2D() : failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//	�[�x��X�e���V���r���[���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDepthStencilView()
{
	PROFILE("DX_System::CreateDepthStencilView()");

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format		 = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	//	�[�x��X�e���V���r���[���쐬
	if (!DX_Debug::IsHresultCheck(m_device->CreateDepthStencilView(m_dsb.Get(), &dsvDesc, &m_dsv))){
		throw "ID3D11Device::CreateDepthStencilView() : failed";
	}
}

