#include	"DX_Library.h"
//#include	"../user_helper_class/ALManager.h"
//#include	"../user_helper_class/OGGManager.h"
#include	<stdio.h>

using namespace Microsoft::WRL;
//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
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
	//	シェーダーの解放を行う
	DX_ShaderManager::Release();
	DX_Debug::ReportLiveDeviceObjects("DX_ShaderManager::Release");

	//	デバッグの解放を行う
	DX_Debug::ReportLiveDeviceObjects("DX_System::Release");
	DX_Debug::Release();

	//	OGGファイルを解放
	//	OGGManager::Release();

	//	ALManagerを解放する
	//ALManager::Release();
}
//-----------------------------------------------------------------------------------------
//
//  DirectXを初期化する
//
//-----------------------------------------------------------------------------------------
bool DX_System::InitD3D(const HWND& hWnd)
{
	//	ウィンドウハンドルを保存
	m_windowHandle = hWnd;

	try{
		//	DX_Graphicsを初期化
		DX_Graphics::Initialize();

		//	SwapChainを作成する
		CreateDeviceAndSwapChain(hWnd);
		
		//	デバッグデバイスを作成する
		DX_Debug::Initialize();
		DX_Debug::ReportLiveDeviceObjects("DX_Debug::Initialize()");
		//	RenderTargetViewを作成する
		CreateRenderTargetView();

		//	DepthStencilBufferを作成する
		CreateDepthStencilBuffer();

		//	DepthStencilViewを作成する
		CreateDepthStencilView();

		//	レンダーステートを初期化する
		DX_RenderState::Initialize();

		//	シェーダーを初期化
		DX_ShaderManager::Initialize();

		//	ALManagerを初期化
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
//	デバイスなどを解放する
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
//  ウィンドウのサイズを設定する
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
//  ウィンドウの高さを取得
//
//-----------------------------------------------------------------------------------------
unsigned int DX_System::GetWindowHeight()
{
	return m_windowHeight; 
}

//-----------------------------------------------------------------------------------------
//
//  ウィンドウの幅を取得
//
//-----------------------------------------------------------------------------------------
unsigned int DX_System::GetWindowWidth()
{
	return m_windowWidth; 
}

//-----------------------------------------------------------------------------------------
//
//  デバイスを取得
//
//-----------------------------------------------------------------------------------------
ID3D11Device* DX_System::GetDevice() const
{
	return m_device.Get();
}

//-----------------------------------------------------------------------------------------
//
//  デバイスコンテキストを取得
//
//-----------------------------------------------------------------------------------------
ID3D11DeviceContext*	DX_System::GetDeviceContext() const
{
	return m_deviceContext.Get();
}

//-----------------------------------------------------------------------------------------
//
//  スワップチェインを取得
//
//-----------------------------------------------------------------------------------------
IDXGISwapChain*		DX_System::GetSwapChain() const
{
	return m_swapChain.Get();
}

//-----------------------------------------------------------------------------------------
//
//  FeatureLevelを取得
//
//-----------------------------------------------------------------------------------------
D3D_FEATURE_LEVEL DX_System::GetFeatureLevel()
{
	return m_featureLevel;
}

//-----------------------------------------------------------------------------------------
//
//  DriverTypeを取得
//
//-----------------------------------------------------------------------------------------
D3D_DRIVER_TYPE DX_System::GetDriverType()
{
	return m_driverType;
}

//-----------------------------------------------------------------------------------------
//
//  レンダーターゲットを取得
//
//-----------------------------------------------------------------------------------------
ID3D11RenderTargetView*	DX_System::GetDefaultRenderTargetView() const
{
	return m_rtv.Get();
}

//-----------------------------------------------------------------------------------------
//
//  深度･ステンシルビューを取得
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilView*	DX_System::GetDefaultDepthStencilView() const
{
	return m_dsv.Get();
}


//-----------------------------------------------------------------------------------------
//
//  深度･ステンシルバッファを取得
//
//-----------------------------------------------------------------------------------------
ID3D11Texture2D*		DX_System::GetDepthStencilBuffer() const
{
	return m_dsb.Get();
}

//-----------------------------------------------------------------------------------------
//
//  ウィンドウハンドルを取得
//
//-----------------------------------------------------------------------------------------
HWND	DX_System::GetWindowHandle()
{
	return m_windowHandle;
}

//-----------------------------------------------------------------------------------------
//
//  SwapChain,ID3D11Device,ID3D11DeviceContextを作成
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDeviceAndSwapChain(const HWND& hWnd)
{
	PROFILE("DX_System::CreateDeviceAndSwapChain()");

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };

	//	バックバッファ数
	swapChainDesc.BufferCount = 1;

	//	バックバッファの幅と高さ
	swapChainDesc.BufferDesc.Width	= m_windowWidth;
	swapChainDesc.BufferDesc.Height	= m_windowHeight;

	//	フォーマット
	swapChainDesc.BufferDesc.Format = DX_Graphics::GetFortmat();

	//	リフレッシュレートの分母と分子
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= DX_Graphics::GetRefreshRateN();
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= DX_Graphics::GetRefreshRateD();

	//	スキャンラインとスケーリング
	swapChainDesc.BufferDesc.ScanlineOrdering	= DX_Graphics::GetScanLineOrder();
	swapChainDesc.BufferDesc.Scaling			= DX_Graphics::GetScaling();;

	// バック バッファの使用法
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 関連付けるウインドウ
	swapChainDesc.OutputWindow = hWnd;

	//	マルチサンプルの数とクオリティ
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;

	//	ウィンドウモード
	swapChainDesc.Windowed = TRUE;

	// モード自動切り替え
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	//	GPUレベル
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

	//	ハードタイプ
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
//  レンデーターゲットビューを作成
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateRenderTargetView()
{
	PROFILE("DX_System::CreateRenderTargetView()");

	ComPtr<ID3D11Texture2D> buffer;

	//	バッファを取得
	if (!DX_Debug::IsHresultCheck(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer))){
		throw "IDXGISwapChain::GetBuffer() : faield";
	}

	//	レンダーターゲットビューを作成する
	if (!DX_Debug::IsHresultCheck(m_device->CreateRenderTargetView(buffer.Get(), nullptr, &m_rtv))){
		throw "ID3D11Device::CreateRenderTargetView() : faield";
	}
}


//-----------------------------------------------------------------------------------------
//
//	深度･ステンシルバッファを作成する
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

	//	深度･ステンシルバッファを作成
	if (!DX_Debug::IsHresultCheck(m_device->CreateTexture2D(&dsbDesc, nullptr, &m_dsb))){
		throw "ID3D11Device::CreateTexture2D() : failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//	深度･ステンシルビューを作成する
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDepthStencilView()
{
	PROFILE("DX_System::CreateDepthStencilView()");

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format		 = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	//	深度･ステンシルビューを作成
	if (!DX_Debug::IsHresultCheck(m_device->CreateDepthStencilView(m_dsb.Get(), &dsvDesc, &m_dsv))){
		throw "ID3D11Device::CreateDepthStencilView() : failed";
	}
}

