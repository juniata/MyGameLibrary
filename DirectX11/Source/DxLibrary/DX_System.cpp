#include	"DX_Library.h"
//#include	"../user_helper_class/ALManager.h"
//#include	"../user_helper_class/OGGManager.h"
#include	<stdio.h>

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

//-----------------------------------------------------------------------------------------
//
//  ������
//
//-----------------------------------------------------------------------------------------
DX_System::DX_System() :
	m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRtv(nullptr),
	m_pDsv(nullptr),
	m_pDsb(nullptr)
{
}

//-----------------------------------------------------------------------------------------
//
//  ���
//
//-----------------------------------------------------------------------------------------
DX_System::~DX_System()
{
	//	�V�F�[�_�[�̉�����s��
	DX_ShaderManager::Release();

	// �����_�[�X�e�[�g�̉�����s��
	DX_RenderState::Release();

	// �e�N�X�`���̉�����s��
	DX_TextureManager::Release();

	// �O���t�B�b�N�X�̊J�����s���B
	DX_Graphics::Release();

	m_pSwapChain->SetFullscreenState(FALSE, nullptr);
	m_pDeviceContext->ClearState();


	SAFE_RELEASE(m_pRtv);
	SAFE_RELEASE(m_pDsb);
	SAFE_RELEASE(m_pDsv);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);

	// DX_Debug�����
	DX_Debug::GetInstance()->ReportLiveDeviceObjects("DX_System::Release");
	DX_Debug::Release();

	//	OGG�t�@�C�������
	//	OGGManager::Release();

	//	ALManager���������
	//ALManager::Release();

}
//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���擾����
//
//-----------------------------------------------------------------------------------------
DX_System* DX_System::GetInstance()
{
	if (m_pInstnace == nullptr) {
		m_pInstnace = new DX_System();
	}

	return m_pInstnace;
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
		DX_Graphics::GetInstance()->Initialize();

		//	SwapChain���쐬����
		CreateDeviceAndSwapChain(hWnd);
	
		//	�f�o�b�O�f�o�C�X���쐬����
		DX_Debug::GetInstance()->Initialize();
	
		// �o�b�N�o�b�t�@���쐬����
		InitBuckBuffer();

		//	�����_�[�X�e�[�g������������
		DX_RenderState::GetInstance()->Initialize();

		//	�V�F�[�_�[��������
		DX_ShaderManager::GetInstance()->Initialize();

		// �e�N�X�`���̏��������s��(�C���X�^���X�����ł̃R���X�g���N�^�ŏ��������s����)
		DX_TextureManager::GetInstance();

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
	DELETE_OBJ(m_pInstnace);
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
	return m_pDevice;
}

//-----------------------------------------------------------------------------------------
//
//  �f�o�C�X�R���e�L�X�g���擾
//
//-----------------------------------------------------------------------------------------
ID3D11DeviceContext*	DX_System::GetDeviceContext() const
{
	return m_pDeviceContext;
}

//-----------------------------------------------------------------------------------------
//
//  �X���b�v�`�F�C�����擾
//
//-----------------------------------------------------------------------------------------
IDXGISwapChain*		DX_System::GetSwapChain() const
{
	return m_pSwapChain;
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
	return m_pRtv;
}

//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���r���[���擾
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilView*	DX_System::GetDefaultDepthStencilView() const
{
	return m_pDsv;
}


//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���o�b�t�@���擾
//
//-----------------------------------------------------------------------------------------
ID3D11Texture2D*		DX_System::GetDepthStencilBuffer() const
{
	return m_pDsb;
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

void DX_System::InitBuckBuffer()
{
	SAFE_RELEASE(m_pRtv);
	SAFE_RELEASE(m_pDsv);
	SAFE_RELEASE(m_pDsb);

	//	RenderTargetView���쐬����
	CreateRenderTargetView();

	//	DepthStencilBuffer���쐬����
	CreateDepthStencilBuffer();

	//	DepthStencilView���쐬����
	CreateDepthStencilView();
}
//-----------------------------------------------------------------------------------------
//
//  SwapChain,ID3D11Device,ID3D11DeviceContext���쐬
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDeviceAndSwapChain(const HWND& hWnd)
{
	DX_Graphics* pGrapchics = DX_Graphics::GetInstance();

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width	= rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };

	//	�o�b�N�o�b�t�@��
	swapChainDesc.BufferCount = 1;

	//	�o�b�N�o�b�t�@�̕��ƍ���
	swapChainDesc.BufferDesc.Width	= width;
	swapChainDesc.BufferDesc.Height	= height;

	//	�t�H�[�}�b�g
	swapChainDesc.BufferDesc.Format = pGrapchics->GetFortmat();

	//	���t���b�V�����[�g�̕���ƕ��q
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= pGrapchics->GetRefreshRateN();
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= pGrapchics->GetRefreshRateD();

	//	�X�L�������C���ƃX�P�[�����O
	swapChainDesc.BufferDesc.ScanlineOrdering	= pGrapchics->GetScanLineOrder();
	swapChainDesc.BufferDesc.Scaling			= pGrapchics->GetScaling();

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
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

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
			&m_pSwapChain,
			&m_pDevice,
			&m_featureLevel,
			&m_pDeviceContext
			);

		if (SUCCEEDED(hr)){
			m_driverType = driveTypes[i];
			break;
		}
	}

	if (!DX_Debug::GetInstance()->IsHresultCheck(hr)){
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
	ID3D11Texture2D* pBuffer = nullptr;
	//	�o�b�N�o�b�t�@���擾
	if (!DX_Debug::GetInstance()->IsHresultCheck(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer))){
		throw "IDXGISwapChain::GetBuffer() : faield";
	}

	//	�����_�[�^�[�Q�b�g�r���[���쐬����
	if (!DX_Debug::GetInstance()->IsHresultCheck(m_pDevice->CreateRenderTargetView(pBuffer, nullptr, &m_pRtv))){
		throw "ID3D11Device::CreateRenderTargetView() : faield";
	}

	SAFE_RELEASE(pBuffer);
}


//-----------------------------------------------------------------------------------------
//
//	�[�x��X�e���V���o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDepthStencilBuffer()
{
	ID3D11Texture2D* pBuffer = nullptr;
	//	�o�b�N�o�b�t�@���擾
	if (!DX_Debug::GetInstance()->IsHresultCheck(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer))) {
		throw "IDXGISwapChain::GetBuffer() : faield";
	}

	// �o�b�N�E�o�b�t�@�̏��
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBuffer->GetDesc(&descBackBuffer);

	D3D11_TEXTURE2D_DESC dsbDesc = descBackBuffer;
	dsbDesc.MipLevels			= 1;
	dsbDesc.ArraySize			= 1;
	dsbDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	dsbDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	dsbDesc.Usage				= D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	dsbDesc.CPUAccessFlags		= 0;   // CPU����̓A�N�Z�X���Ȃ�
	dsbDesc.MiscFlags			= 0;   // ���̑��̐ݒ�Ȃ�

	//	�[�x��X�e���V���o�b�t�@���쐬
	if (!DX_Debug::GetInstance()->IsHresultCheck(m_pDevice->CreateTexture2D(&dsbDesc, nullptr, &m_pDsb))){
		throw "ID3D11Device::CreateTexture2D() : failed";
	}
	SAFE_RELEASE(pBuffer);
}


//-----------------------------------------------------------------------------------------
//
//	�[�x��X�e���V���r���[���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC desc = { NULL };
	m_pDsb->GetDesc(&desc);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format			= desc.Format;
	dsvDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;

	//	�[�x��X�e���V���r���[���쐬
	if (!DX_Debug::GetInstance()->IsHresultCheck(m_pDevice->CreateDepthStencilView(m_pDsb, &dsvDesc, &m_pDsv))){
		throw "ID3D11Device::CreateDepthStencilView() : failed";
	}
}

