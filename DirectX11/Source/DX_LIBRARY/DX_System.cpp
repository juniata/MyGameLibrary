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
//  Deviceなどの生成を行う
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
		CreateSwapChain(hWnd);
		
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

void DX_System::CreateDevice()
{
	PROFILE("DX_System::CreateDevice()");

	HRESULT l_hr = S_FALSE;

	//このフラグは、カラー チャネルの順序が API の既定値とは異なるサーフェスのサポートを追加します。
	// これは、Direct2D との互換性を保持するために必要です。
	UINT l_creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	// デバッグレイヤーを使用できるかチェックする(ライブラリ不足だとできなかったりする)
	l_hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // 実際のハードウェア デバイスを作成する必要はありません。
		0,
		D3D11_CREATE_DEVICE_DEBUG,  // SDK レイヤーを確認してください。
		nullptr,                    // どの機能レベルでも対応できます。
		0,
		D3D11_SDK_VERSION,          // Windows ストア アプリでは、これには常に D3D11_SDK_VERSION を設定します。
		nullptr,                    // D3D デバイスの参照を保持する必要はありません。
		nullptr,                    // 機能レベルを調べる必要はありません。
		nullptr                     // D3D デバイスのコンテキスト参照を保持する必要はありません。
	);
	if (SUCCEEDED(l_hr)) {
		l_creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif
	//	GPUレベル
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
	// Direct3D 11 API デバイス オブジェクトと、対応するコンテキストを作成します。
	ComPtr<ID3D11Device> l_device;
	ComPtr<ID3D11DeviceContext> l_context;

	l_hr = D3D11CreateDevice(
		nullptr,					// 既定のアダプターを使用する nullptr を指定します。
		D3D_DRIVER_TYPE_HARDWARE,	// ハードウェア グラフィックス ドライバーを使用してデバイスを作成します。
		0,							// ドライバーが D3D_DRIVER_TYPE_SOFTWARE でない限り、0 を使用してください。
		l_creationFlags,			// デバッグ フラグと Direct2D 互換性フラグを設定します。
		l_featureLevels,			// このアプリがサポートできる機能レベルの一覧を表示します。
		ARRAYSIZE(l_featureLevels),	// 上記リストのサイズ。
		D3D11_SDK_VERSION,			// Windows ストア アプリでは、これには常に D3D11_SDK_VERSION を設定します。
		&l_device,					// 作成された Direct3D デバイスを返します。
		&m_featureLevel,			// 作成されたデバイスの機能レベルを返します。
		&l_context					// デバイスのイミディエイト コンテキストを返します。
	);

	// ハードウェア デバイスの代わりに WARP デバイスを作成します。
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
//  SwapChain,ID3D11Device,ID3D11DeviceContextを作成
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateSwapChain(const HWND& hWnd)
{
	PROFILE("DX_System::CreateSwapChain()");

	DXGI_SWAP_CHAIN_DESC l_swapChainDesc;

	//	初期化
	ZeroMemory(&l_swapChainDesc, sizeof(l_swapChainDesc));

	//	バックバッファ数
	l_swapChainDesc.BufferCount = 1;

	//	バックバッファの幅と高さ
	l_swapChainDesc.BufferDesc.Width	= m_windowWidth;
	l_swapChainDesc.BufferDesc.Height	= m_windowHeight;

	//	フォーマット
	l_swapChainDesc.BufferDesc.Format = DX_Graphics::GetFortmat();

	//	リフレッシュレートの分母と分子
	l_swapChainDesc.BufferDesc.RefreshRate.Numerator	= DX_Graphics::GetRefreshRateN();
	l_swapChainDesc.BufferDesc.RefreshRate.Denominator	= DX_Graphics::GetRefreshRateD();

	//	スキャンラインとスケーリング
	l_swapChainDesc.BufferDesc.ScanlineOrdering = DX_Graphics::GetScanLineOrder();
	l_swapChainDesc.BufferDesc.Scaling			= DX_Graphics::GetScaling();;

	// バック バッファの使用法
	l_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 関連付けるウインドウ
	l_swapChainDesc.OutputWindow = hWnd;

	//	マルチサンプルの数とクオリティ
	l_swapChainDesc.SampleDesc.Count	= 1;
	l_swapChainDesc.SampleDesc.Quality	= 0;

	//	ウィンドウモード
	l_swapChainDesc.Windowed = TRUE;

	// モード自動切り替え
	l_swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	//	GPUレベル
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

	//	ハードタイプ
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
//  レンデーターゲットビューを作成
//
//-----------------------------------------------------------------------------------------
void DX_System::CreateRenderTargetView()
{
	PROFILE("DX_System::CreateRenderTargetView()");

	ComPtr<ID3D11Texture2D> l_buffer;

	//	バッファを取得
	if (!DX_Debug::IsHresultCheck(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&l_buffer))){
		throw "IDXGISwapChain::GetBuffer() : faield";
	}

	//	レンダーターゲットビューを作成する
	if (!DX_Debug::IsHresultCheck(m_device->CreateRenderTargetView(l_buffer.Get(), nullptr, &m_rtv))){
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

	D3D11_TEXTURE2D_DESC l_desc;
	ZeroMemory(&l_desc, sizeof(l_desc));

	l_desc.MipLevels		= 1;
	l_desc.ArraySize		= 1;
	l_desc.SampleDesc.Count = 1;
	l_desc.Width			= m_windowWidth;
	l_desc.Height			= m_windowHeight;
	l_desc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
	l_desc.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;

	//	深度･ステンシルバッファを作成
	if (!DX_Debug::IsHresultCheck(m_device->CreateTexture2D(&l_desc, nullptr, &m_dsb))){
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

	D3D11_DEPTH_STENCIL_VIEW_DESC l_desc;
	ZeroMemory(&l_desc, sizeof(l_desc));
	l_desc.Format		 = DXGI_FORMAT_D24_UNORM_S8_UINT;
	l_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	//	深度･ステンシルビューを作成
	if (!DX_Debug::IsHresultCheck(m_device->CreateDepthStencilView(m_dsb.Get(), &l_desc, &m_dsv))){
		throw "ID3D11Device::CreateDepthStencilView() : failed";
	}

}

