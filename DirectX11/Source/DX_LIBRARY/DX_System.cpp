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


DX_System::System()
{

}

DX_System* DX_System::GetInstance()
{
	if (m_pInstnace == nullptr) {
		m_pInstnace = new DX_System();
	}

	return m_pInstnace;
}

DX_System::~System()
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
//  Device�Ȃǂ̐������s��
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
		CreateSwapChain(hWnd);
		
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
void DX_System::SetWindowsSize(dx_library::CREATE_WINDOW_SIZE windowSize)
{
	switch (windowSize){
	case dx_library::CREATE_WINDOW_SIZE::SCREEN_480p:
		m_windowWidth	= 720;
		m_windowHeight	= 480;
		break;

	case dx_library::CREATE_WINDOW_SIZE::SCREEN_720p:
		m_windowWidth	= 1280;
		m_windowHeight	= 720;
		break;

	case dx_library::CREATE_WINDOW_SIZE::SCREEN_1080p:
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

void DX_System::CreateDevice()
{
	PROFILE("DX_System::CreateDevice()");

	HRESULT l_hr = S_FALSE;

	//���̃t���O�́A�J���[ �`���l���̏����� API �̊���l�Ƃ͈قȂ�T�[�t�F�X�̃T�|�[�g��ǉ����܂��B
	// ����́ADirect2D �Ƃ̌݊�����ێ����邽�߂ɕK�v�ł��B
	UINT l_creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	// �f�o�b�O���C���[���g�p�ł��邩�`�F�b�N����(���C�u�����s�����Ƃł��Ȃ������肷��)
	l_hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // ���ۂ̃n�[�h�E�F�A �f�o�C�X���쐬����K�v�͂���܂���B
		0,
		D3D11_CREATE_DEVICE_DEBUG,  // SDK ���C���[���m�F���Ă��������B
		nullptr,                    // �ǂ̋@�\���x���ł��Ή��ł��܂��B
		0,
		D3D11_SDK_VERSION,          // Windows �X�g�A �A�v���ł́A����ɂ͏�� D3D11_SDK_VERSION ��ݒ肵�܂��B
		nullptr,                    // D3D �f�o�C�X�̎Q�Ƃ�ێ�����K�v�͂���܂���B
		nullptr,                    // �@�\���x���𒲂ׂ�K�v�͂���܂���B
		nullptr                     // D3D �f�o�C�X�̃R���e�L�X�g�Q�Ƃ�ێ�����K�v�͂���܂���B
	);
	if (SUCCEEDED(l_hr)) {
		l_creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif
	//	GPU���x��
	D3D_FEATURE_LEVEL l_featureLevels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	// Direct3D 11 API �f�o�C�X �I�u�W�F�N�g�ƁA�Ή�����R���e�L�X�g���쐬���܂��B
	ComPtr<ID3D11Device> l_device;
	ComPtr<ID3D11DeviceContext> l_context;

	l_hr = D3D11CreateDevice(
		nullptr,					// ����̃A�_�v�^�[���g�p���� nullptr ���w�肵�܂��B
		D3D_DRIVER_TYPE_HARDWARE,	// �n�[�h�E�F�A �O���t�B�b�N�X �h���C�o�[���g�p���ăf�o�C�X���쐬���܂��B
		0,							// �h���C�o�[�� D3D_DRIVER_TYPE_SOFTWARE �łȂ�����A0 ���g�p���Ă��������B
		l_creationFlags,			// �f�o�b�O �t���O�� Direct2D �݊����t���O��ݒ肵�܂��B
		l_featureLevels,			// ���̃A�v�����T�|�[�g�ł���@�\���x���̈ꗗ��\�����܂��B
		ARRAYSIZE(l_featureLevels),	// ��L���X�g�̃T�C�Y�B
		D3D11_SDK_VERSION,			// Windows �X�g�A �A�v���ł́A����ɂ͏�� D3D11_SDK_VERSION ��ݒ肵�܂��B
		&l_device,					// �쐬���ꂽ Direct3D �f�o�C�X��Ԃ��܂��B
		&m_featureLevel,			// �쐬���ꂽ�f�o�C�X�̋@�\���x����Ԃ��܂��B
		&l_context					// �f�o�C�X�̃C�~�f�B�G�C�g �R���e�L�X�g��Ԃ��܂��B
	);

	// �n�[�h�E�F�A �f�o�C�X�̑���� WARP �f�o�C�X���쐬���܂��B
	if (FAILED(l_hr)) {
		l_hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP,
			0,
			l_creationFlags,
			l_featureLevels,
			ARRAYSIZE(l_featureLevels),
			D3D11_SDK_VERSION,
			&l_device,
			&m_featureLevel,
			&l_context
		);
		if (!Debug::IsHresultCheck(l_hr)) {
			throw "D3D11CreateDevice() : failed";
		}
	}

	if (!Debug::IsHresultCheck(l_device.As(&m_device))) {
		throw "save device : failed";
	}
	if (!Debug::IsHresultCheck(l_context.As(&m_deviceContext))) {
		throw "save device context : failed";
	}
}
//-----------------------------------------------------------------------------------------
//
//  SwapChain,ID3D11Device,ID3D11DeviceContext���쐬
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateSwapChain(const HWND& hWnd)
{
	PROFILE("DX_System::CreateSwapChain()");

	DXGI_SWAP_CHAIN_DESC l_swapChainDesc;

	//	������
	ZeroMemory(&l_swapChainDesc, sizeof(l_swapChainDesc));

	//	�o�b�N�o�b�t�@��
	l_swapChainDesc.BufferCount = 1;

	//	�o�b�N�o�b�t�@�̕��ƍ���
	l_swapChainDesc.BufferDesc.Width	= m_windowWidth;
	l_swapChainDesc.BufferDesc.Height	= m_windowHeight;

	//	�t�H�[�}�b�g
	l_swapChainDesc.BufferDesc.Format = DX_Graphics::GetFortmat();

	//	���t���b�V�����[�g�̕���ƕ��q
	l_swapChainDesc.BufferDesc.RefreshRate.Numerator	= DX_Graphics::GetRefreshRateN();
	l_swapChainDesc.BufferDesc.RefreshRate.Denominator	= DX_Graphics::GetRefreshRateD();

	//	�X�L�������C���ƃX�P�[�����O
	l_swapChainDesc.BufferDesc.ScanlineOrdering = DX_Graphics::GetScanLineOrder();
	l_swapChainDesc.BufferDesc.Scaling			= DX_Graphics::GetScaling();;

	// �o�b�N �o�b�t�@�̎g�p�@
	l_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �֘A�t����E�C���h�E
	l_swapChainDesc.OutputWindow = hWnd;

	//	�}���`�T���v���̐��ƃN�I���e�B
	l_swapChainDesc.SampleDesc.Count	= 1;
	l_swapChainDesc.SampleDesc.Quality	= 0;

	//	�E�B���h�E���[�h
	l_swapChainDesc.Windowed = TRUE;

	// ���[�h�����؂�ւ�
	l_swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	//	GPU���x��
	D3D_FEATURE_LEVEL l_featureLevels[] = {
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
	D3D_DRIVER_TYPE l_pDriveTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,	D3D_DRIVER_TYPE_SOFTWARE, D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_WARP,		D3D_DRIVER_TYPE_NULL,
	};

#if defined(DEBUG) || defined(_DEBUG)
	UINT l_createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT l_createDeviceFlags = 0;
#endif

	HRESULT l_hr;
	for (UINT i = 0; i < _countof(l_pDriveTypes); ++i){
		l_hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			l_pDriveTypes[i],
			0,
			l_createDeviceFlags,
			l_featureLevels,
			ARRAYSIZE(l_featureLevels),
			D3D11_SDK_VERSION,
			&l_swapChainDesc,
			&m_swapChain,
			&m_device,
			&m_featureLevel,
			&m_deviceContext
			);

		if (SUCCEEDED(l_hr)){
			m_driverType = l_pDriveTypes[i];
			break;
		}
	}

	if (!DX_Debug::IsHresultCheck(l_hr)){
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

	ComPtr<ID3D11Texture2D> l_buffer;

	//	�o�b�t�@���擾
	if (!DX_Debug::IsHresultCheck(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&l_buffer))){
		throw "IDXGISwapChain::GetBuffer() : faield";
	}

	//	�����_�[�^�[�Q�b�g�r���[���쐬����
	if (!DX_Debug::IsHresultCheck(m_device->CreateRenderTargetView(l_buffer.Get(), nullptr, &m_rtv))){
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

	D3D11_TEXTURE2D_DESC l_desc;
	ZeroMemory(&l_desc, sizeof(l_desc));

	l_desc.MipLevels		= 1;
	l_desc.ArraySize		= 1;
	l_desc.SampleDesc.Count = 1;
	l_desc.Width			= m_windowWidth;
	l_desc.Height			= m_windowHeight;
	l_desc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
	l_desc.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;

	//	�[�x��X�e���V���o�b�t�@���쐬
	if (!DX_Debug::IsHresultCheck(m_device->CreateTexture2D(&l_desc, nullptr, &m_dsb))){
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

	D3D11_DEPTH_STENCIL_VIEW_DESC l_desc;
	ZeroMemory(&l_desc, sizeof(l_desc));
	l_desc.Format		 = DXGI_FORMAT_D24_UNORM_S8_UINT;
	l_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	//	�[�x��X�e���V���r���[���쐬
	if (!DX_Debug::IsHresultCheck(m_device->CreateDepthStencilView(m_dsb.Get(), &l_desc, &m_dsv))){
		throw "ID3D11Device::CreateDepthStencilView() : failed";
	}

}

