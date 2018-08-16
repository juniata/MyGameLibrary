#include	"DX_Library.h"
//#include	"../user_helper_class/ALManager.h"
//#include	"../user_helper_class/OGGManager.h"
#include	<stdio.h>
#include	<wrl/client.h>
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

//-----------------------------------------------------------------------------------------
//
//  初期化
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
//  解放
//
//-----------------------------------------------------------------------------------------
DX_System::~DX_System()
{
	// DX_Graphicsを解放する
	DX_Graphics::Release();

	//	シェーダーの解放を行う
	DX_ShaderManager::Release();

	// レンダーステートを初期化する
	DX_RenderState::Release();

	m_pSwapChain->SetFullscreenState(FALSE, nullptr);
	m_pDeviceContext->ClearState();


	SAFE_RELEASE(m_pRtv);
	SAFE_RELEASE(m_pDsb);
	SAFE_RELEASE(m_pDsv);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);

	// DX_Debugを解放
	DX_Debug::GetInstance()->ReportLiveDeviceObjects("DX_System::Release");
	DX_Debug::Release();

	//	OGGファイルを解放
	//	OGGManager::Release();

	//	ALManagerを解放する
	//ALManager::Release();

}
//-----------------------------------------------------------------------------------------
//
//  インスタンスを取得する
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
		DX_Debug::GetInstance()->Initialize();
		DX_Debug::GetInstance()->ReportLiveDeviceObjects("dx_debug initialize");
	
		// バックバッファを作成する
		InitBuckBuffer();

		//	レンダーステートを初期化する
		DX_RenderState::Initialize(m_pDevice);

		//	シェーダーを初期化
		DX_ShaderManager::Initialize(m_pDevice);

		//	ALManagerを初期化
		//ALManager::Initialize();
	}
	catch (...){
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
	DELETE_OBJ(m_pInstnace);
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
	return m_pDevice;
}

//-----------------------------------------------------------------------------------------
//
//  デバイスコンテキストを取得
//
//-----------------------------------------------------------------------------------------
ID3D11DeviceContext*	DX_System::GetDeviceContext() const
{
	return m_pDeviceContext;
}

//-----------------------------------------------------------------------------------------
//
//  スワップチェインを取得
//
//-----------------------------------------------------------------------------------------
IDXGISwapChain*		DX_System::GetSwapChain() const
{
	return m_pSwapChain;
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
	return m_pRtv;
}

//-----------------------------------------------------------------------------------------
//
//  深度･ステンシルビューを取得
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilView*	DX_System::GetDefaultDepthStencilView() const
{
	return m_pDsv;
}


//-----------------------------------------------------------------------------------------
//
//  深度･ステンシルバッファを取得
//
//-----------------------------------------------------------------------------------------
ID3D11Texture2D*		DX_System::GetDepthStencilBuffer() const
{
	return m_pDsb;
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

void DX_System::InitBuckBuffer()
{
	SAFE_RELEASE(m_pRtv);
	SAFE_RELEASE(m_pDsv);
	SAFE_RELEASE(m_pDsb);

	//	RenderTargetViewを作成する
	CreateRenderTargetView();

	//	DepthStencilBufferを作成する
	CreateDepthStencilBuffer();

	//	DepthStencilViewを作成する
	CreateDepthStencilView();
}
//-----------------------------------------------------------------------------------------
//
//  SwapChain,ID3D11Device,ID3D11DeviceContextを作成
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDeviceAndSwapChain(const HWND& hWnd)
{
	PROFILE("DX_System::CreateDeviceAndSwapChain()");

	DX_Graphics* pGraphics = DX_Graphics::GetInstance();

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width	= rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };

	//	バックバッファ数
	swapChainDesc.BufferCount = 1;

	//	バックバッファの幅と高さ
	swapChainDesc.BufferDesc.Width	= width;
	swapChainDesc.BufferDesc.Height	= height;

	//	フォーマット
	swapChainDesc.BufferDesc.Format = pGraphics->GetFortmat();

	//	リフレッシュレートの分母と分子
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= pGraphics->GetRefreshRateN();
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= pGraphics->GetRefreshRateD();

	//	スキャンラインとスケーリング
	swapChainDesc.BufferDesc.ScanlineOrdering	= pGraphics->GetScanLineOrder();
	swapChainDesc.BufferDesc.Scaling			= pGraphics->GetScaling();

	// バック バッファの使用法
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 関連付けるウインドウ
	swapChainDesc.OutputWindow = hWnd;
	
	//	マルチサンプルの数とクオリティ(conut = 1 quality=0だとアンチエイリアス処理は行わない)
	// ※アンチエイリアス処理を行って描画する際は、描画ターゲットと深度バッファのマルチサンプル数とクオリティレベルが一致していなければならない。
	// また、アンチエイリアスを使用する場合、CheckMultisampleQualityLevels()で使用できるクオリティレベルをチェックすること。
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;

	//	ウィンドウモード
	swapChainDesc.Windowed = TRUE;

	// モード自動切り替え
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	// 画面を更新する際、バックバッファの内容はどうするか？
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

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

	DX_Debug::GetInstance()->ThrowIfFailed(hr, "D3D11CreateDeviceAndSwapChain() : failed");
}


//-----------------------------------------------------------------------------------------
//
//  レンデーターゲットビューを作成
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateRenderTargetView()
{
	PROFILE("DX_System::CreateRenderTargetView()");

	ComPtr<ID3D11Texture2D>  buffer;
	DX_Debug::GetInstance()->ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer), "IDXGISwapChain::GetBuffer() : faield");
	DX_Debug::GetInstance()->ThrowIfFailed(m_pDevice->CreateRenderTargetView(buffer.Get(), nullptr, &m_pRtv), "ID3D11Device::CreateRenderTargetView() : faield");
}


//-----------------------------------------------------------------------------------------
//
//	深度･ステンシルバッファを作成する
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDepthStencilBuffer()
{
	PROFILE("DX_System::CreateDepthStencilBuffer()");

	//	バックバッファを取得
	ComPtr<ID3D11Texture2D>  buffer;
	DX_Debug::GetInstance()->ThrowIfFailed(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer), "IDXGISwapChain::GetBuffer() : faield");

	// バック・バッファの情報
	D3D11_TEXTURE2D_DESC descBackBuffer;
	buffer->GetDesc(&descBackBuffer);

	D3D11_TEXTURE2D_DESC dsbDesc = descBackBuffer;
	dsbDesc.MipLevels			= 1;
	dsbDesc.ArraySize			= 1;
	dsbDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	dsbDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	dsbDesc.Usage				= D3D11_USAGE_DEFAULT;      // デフォルト使用法
	dsbDesc.CPUAccessFlags		= 0;   // CPUからはアクセスしない
	dsbDesc.MiscFlags			= 0;   // その他の設定なし

	//	深度･ステンシルバッファを作成
	DX_Debug::GetInstance()->ThrowIfFailed(m_pDevice->CreateTexture2D(&dsbDesc, nullptr, &m_pDsb), "ID3D11Device::CreateTexture2D() : faield");
}


//-----------------------------------------------------------------------------------------
//
//	深度･ステンシルビューを作成する
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateDepthStencilView()
{
	PROFILE("DX_System::CreateDepthStencilView()");

	D3D11_TEXTURE2D_DESC desc = { NULL };
	m_pDsb->GetDesc(&desc);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format			= desc.Format;
	dsvDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;

	DX_Debug::GetInstance()->ThrowIfFailed(m_pDevice->CreateDepthStencilView(m_pDsb, &dsvDesc, &m_pDsv), "ID3D11Device::CreateDepthStencilView() : faield");
}

