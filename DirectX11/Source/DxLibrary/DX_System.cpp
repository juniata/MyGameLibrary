#include	"DX_Library.h"
//#include	"../user_helper_class/ALManager.h"
//#include	"../user_helper_class/OGGManager.h"
#include	<stdio.h>

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
//
//	メンバ変数を初期化する
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
//  DirectXを初期化する
//
//-----------------------------------------------------------------------------------------
bool DX_System::Initialize(HWND hWnd, HINSTANCE hInst)
{
	//	ウィンドウハンドルを保存
	m_windowHandle = hWnd;

	//	DX_Graphicsを初期
	if (!DX_Graphics::Initialize()) {
		return false;
	}


	//	SwapChainを作成する
	if (!CreateDeviceAndSwapChain(hWnd)) {
		return false;
	}

	//	デバッグデバイスを作成する
	DX_Debug::Create();
	DX_Debug::GetInstance()->Initialize();

	// バックバッファを作成する
	if (!InitBuckBuffer()) {
		return false;
	}

	//	レンダーステートを初期化する
	DX_RenderState::Create();
	if (!DX_RenderState::GetInstance()->Initialize()) {
		return false;
	}

	//	シェーダーを初期化
	DX_ShaderManager::Initialize();

	// DX_TextureManagerを初期化する
	DX_TextureManager::Initialize();

	// DirectInputを初期化
	if (!DX_Input::Initialize(hWnd, hInst)) {
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------------
//
//	DirectX関連　解放
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
	
	//	OGGファイルを解放
	//	OGGManager::Release();

	//	ALManagerを解放する
	//ALManager::Release();
	//DX_Debug::Destroy();
}


//-----------------------------------------------------------------------------------------
//
//  スクリーンのサイズを設定する
//
//-----------------------------------------------------------------------------------------
void DX_System::SetScreenSize(const unsigned int width, const unsigned int height)
{
	m_screenWidth	= width;
	m_screenHeight	= height;
}

//-----------------------------------------------------------------------------------------
//
//  スクリーンの高さを取得
//
//-----------------------------------------------------------------------------------------
unsigned int DX_System::GetScreenHeight()
{
	return m_screenHeight; 
}

//-----------------------------------------------------------------------------------------
//
//  スクリーンの幅を取得
//
//-----------------------------------------------------------------------------------------
unsigned int DX_System::GetScreenWidth()
{
	return m_screenWidth; 
}

//-----------------------------------------------------------------------------------------
//
//  デバイスを取得
//
//-----------------------------------------------------------------------------------------
ID3D11Device* DX_System::GetDevice()
{
	return m_device.Get();
}

//-----------------------------------------------------------------------------------------
//
//  デバイスコンテキストを取得
//
//-----------------------------------------------------------------------------------------
ID3D11DeviceContext*	DX_System::GetDeviceContext()
{
	return m_deviceContext.Get();
}

//-----------------------------------------------------------------------------------------
//
//  スワップチェインを取得
//
//-----------------------------------------------------------------------------------------
IDXGISwapChain*		DX_System::GetSwapChain()
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
ID3D11RenderTargetView*	DX_System::GetRenderTargetView()
{
	return m_rtv.Get();
}

//-----------------------------------------------------------------------------------------
//
//  深度･ステンシルビューを取得
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilView*	DX_System::GetDepthStencilView()
{
	return m_dsv.Get();
}


//-----------------------------------------------------------------------------------------
//
//  深度･ステンシルバッファを取得
//
//-----------------------------------------------------------------------------------------
ID3D11Texture2D*		DX_System::GetDepthStencilBuffer()
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
//  バックバッファの初期化を行う
//
//-----------------------------------------------------------------------------------------
bool DX_System::InitBuckBuffer()
{
	//	RenderTargetViewを作成する
	if (!CreateRenderTargetView()) {
		return false;
	}

	//	DepthStencilBufferを作成する
	if (!CreateDepthStencilBuffer()) {
		return false;
	}

	//	DepthStencilViewを作成する
	if (!CreateDepthStencilView()) {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//
//  リサイズ処理
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
//  SwapChain,ID3D11Device,ID3D11DeviceContextを作成
//
//-----------------------------------------------------------------------------------------
bool DX_System::CreateDeviceAndSwapChain(const HWND& hWnd)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };

	//	バックバッファ数
	swapChainDesc.BufferCount = 2;

	//	バックバッファの幅と高さ
	swapChainDesc.BufferDesc.Width	= m_screenWidth;
	swapChainDesc.BufferDesc.Height	= m_screenHeight;

	//	フォーマット
	swapChainDesc.BufferDesc.Format = DX_Graphics::GetFortmat();

	//	リフレッシュレートの分母と分子
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= DX_Graphics::GetRefreshRateN();
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= DX_Graphics::GetRefreshRateD();

	//	スキャンラインとスケーリング
	swapChainDesc.BufferDesc.ScanlineOrdering	= DX_Graphics::GetScanLineOrder();
	swapChainDesc.BufferDesc.Scaling			= DX_Graphics::GetScaling();

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
	// DXGI_SWAP_EFFECT_DISCARDとDXGI_SWAP_EFFECT_SEQUENTIALは使用しないほうがいいっぽい。
	// https://devblogs.microsoft.com/directx/dxgi-flip-model/ 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	
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
//  レンデーターゲットビューを作成
//
//-----------------------------------------------------------------------------------------
bool DX_System::CreateRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buckBuffer;

	//	バックバッファを取得
	if (!DX_Debug::GetInstance()->CheckHresult(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buckBuffer))){
		TRACE("failed to GetBuffer()");
		return false;
	}

	//	レンダーターゲットビューを作成する
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
//	深度･ステンシルバッファを作成する
//
//-----------------------------------------------------------------------------------------
bool DX_System::CreateDepthStencilBuffer()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buckBuffer;

	//	バックバッファを取得
	if (!DX_Debug::GetInstance()->CheckHresult(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buckBuffer))) {
		TRACE("faield to GetBuffer()");
		return false;
	}

	// バック・バッファの情報
	D3D11_TEXTURE2D_DESC descBackBuffer;
	buckBuffer->GetDesc(&descBackBuffer);

	D3D11_TEXTURE2D_DESC dsbDesc = descBackBuffer;
	dsbDesc.MipLevels			= 1;
	dsbDesc.ArraySize			= 1;
	dsbDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	dsbDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	dsbDesc.Usage				= D3D11_USAGE_DEFAULT;      // デフォルト使用法
	dsbDesc.CPUAccessFlags		= 0;   // CPUからはアクセスしない
	dsbDesc.MiscFlags			= 0;   // その他の設定なし

	//	深度･ステンシルバッファを作成
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
//	深度･ステンシルビューを作成する
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

	//	深度･ステンシルビューを作成
	if (!DX_Debug::GetInstance()->CheckHresult(m_device->CreateDepthStencilView(m_dsb.Get(), &dsvDesc, &m_dsv))){
		TRACE("failed to CreateDepthStencilView()");
		return false;
	}

	// TODO:SetPrivateData
	m_dsv->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_dsv") - 1, "m_dsv");

	return true;
}

