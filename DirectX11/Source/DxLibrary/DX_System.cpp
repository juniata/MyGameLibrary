#include	"DX_Library.h"
//#include	"../user_helper_class/ALManager.h"
//#include	"../user_helper_class/OGGManager.h"
#include	<stdio.h>

/// <summary>
/// メンバ変数を初期化する
/// </summary>
DX_System::DX_System() :
	m_featureLevel(D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1),
	m_driverType(D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN),
	m_windowHandle(NULL),
	m_screenHeight(0),
	m_screenWidth(0)
{}

/// <summary>
/// DXにかかわるすべてのデバイス等の解放を行う
/// </summary>
DX_System::~DX_System()
{
	m_swapChain->SetFullscreenState(FALSE, nullptr);
	
	m_deviceContext->ClearState();
	m_deviceContext->Flush();

	//DX_SceneManager::Release();
	DX_Input::Destroy();
	DX_Text::Destroy();
	DX_TextureManager::Destroy();
	DX_RenderState::Destroy();
	DX_ShaderManager::Destroy();
	DX_Graphics::Destroy();
	//	OGGファイルを解放
	//	OGGManager::Release();

	//	ALManagerを解放する
	//ALManager::Release();
	//DX_Debug::Destroy();
}

/// <summary>
/// DirectXのデバイス等を初期化する
/// </summary>
/// <param name="hWnd">ウィンドウのインスタンスハンドル</param>
/// <param name="hInst">ウィンドウのインスタンス</param>
/// <param name="width">描画する幅</param>
/// <param name="height">描画する高さ</param>
/// <returns>成否</returns>
bool DX_System::Initialize(HWND hWnd, HINSTANCE hInst, const UINT width, const UINT height)
{
	bool succeed = false;

	m_windowHandle	= hWnd;
	m_screenWidth	= width;
	m_screenHeight	= height;

	do
	{
		DX_Graphics::Create();
		DX_Graphics* graphics = DX_Graphics::GetInstance();
		if (graphics->Initialize() == false)
		{
			break;
		}
		
		if (CreateDeviceAndSwapChain(hWnd) == false)
		{
			break;
		}

		DX_Debug::Create();
		DX_Debug::GetInstance()->Initialize();

		if (InitBuckBuffer() == false)
		{
			break;
		}

		DX_RenderState::Create();
		if (DX_RenderState::GetInstance()->Initialize() == false)
		{
			break;
		}

		DX_ShaderManager::Create();
		if (DX_ShaderManager::GetInstance()->Initialize() == false)
		{
			break;
		}
	
		DX_Input::Create();
		if (DX_Input::GetInstance()->Initialize(hWnd, hInst) == false)
		{
			break;
		}

		DX_TextureManager::Create();
		DX_Text::Create();

		succeed = true;
	} while (false);

	return succeed;
}

/// <summary>
/// スクリーンの高さを取得する
/// </summary>
/// <returns>スクリーンの高さ</returns>
UINT DX_System::GetScreenHeight()
{
	return m_screenHeight; 
}

/// <summary>
/// スクリーンの幅を取得する
/// </summary>
/// <returns>スクリーンの幅</returns>
UINT DX_System::GetScreenWidth()
{
	return m_screenWidth; 
}

/// <summary>
/// DirectXのデバイスを取得する
/// </summary>
/// <returns>デバイス</returns>
ID3D11Device* DX_System::GetDevice()
{
	return m_device.Get();
}

/// <summary>
/// DIrectXのデバイスコンテキストを取得する
/// </summary>
/// <returns>デバイスコンテキスト</returns>
ID3D11DeviceContext*	DX_System::GetDeviceContext()
{
	return m_deviceContext.Get();
}

/// <summary>
/// スワップチェインを取得する
/// </summary>
/// <returns>スワップチェイン</returns>
IDXGISwapChain*		DX_System::GetSwapChain()
{
	return m_swapChain.Get();
}

/// <summary>
/// FeatureLevelを取得する
/// </summary>
/// <returns>FeatureLevel</returns>
D3D_FEATURE_LEVEL DX_System::GetFeatureLevel()
{
	return m_featureLevel;
}

/// <summary>
/// DriverTypeを取得する
/// </summary>
/// <returns>DriverType</returns>
D3D_DRIVER_TYPE DX_System::GetDriverType()
{
	return m_driverType;
}

/// <summary>
/// レンダーターゲットを取得する
/// </summary>
/// <returns>レンダーターゲット</returns>
ID3D11RenderTargetView*	DX_System::GetRenderTargetView()
{
	return m_rtv.Get();
}

/// <summary>
/// 深度・ステンシルビューを取得する
/// </summary>
/// <returns>深度・ステンシルビュー</returns>
ID3D11DepthStencilView*	DX_System::GetDepthStencilView()
{
	return m_dsv.Get();
}

/// <summary>
/// 深度・ステンシルバッファを取得する
/// </summary>
/// <returns>深度・ステンシルバッファ</returns>
ID3D11Texture2D*		DX_System::GetDepthStencilBuffer()
{
	return m_dsb.Get();
}


/// <summary>
/// ウィンドウのインスタンスハンドルを取得する
/// </summary>
/// <returns>ウィンドウのインスタンスハンドル</returns>
HWND	DX_System::GetWindowHandle()
{
	return m_windowHandle;
}

/// <summary>
/// バックバッファを初期化する
/// </summary>
/// <returns>成否</returns>
bool DX_System::InitBuckBuffer()
{
	bool succeed = false;

	do
	{
		if (CreateRenderTargetView() == false)
		{
			break;
		}

		if (CreateDepthStencilBuffer() == false)
		{
			break;
		}

		if (CreateDepthStencilView() == false)
		{
			break;
		}

		succeed = true;
	} while (false);


	return succeed;
}

/// <summary>
/// 画面変更に伴うバッファのリサイズを行う
/// </summary>
/// <param name="width">スクリーンの幅</param>
/// <param name="height">スクリーンの高さ</param>
/// <returns>成否</returns>
bool DX_System::BufferResize(const WORD width, const WORD height)
{
	bool succeed = false;

	m_screenWidth	= static_cast<UINT>(width);
	m_screenHeight	= static_cast<UINT>(height);
	
	ID3D11RTV* const nullRtv[]	= { nullptr };
	ID3D11DSV* const nullDsv	= nullptr;
	
	m_deviceContext->OMSetRenderTargets(1, nullRtv, nullDsv);

	m_rtv.Reset();
	m_dsv.Reset();
	m_dsb.Reset();
	
	do
	{
		// TODO:失敗した時、元のスクリーンサイズで対応する処理が必要。
		DX_Debug* debug = DX_Debug::GetInstance();
		DXGI_SWAP_CHAIN_DESC buckBufferDesc;

		if (debug->IsFailedHresult(m_swapChain->GetDesc(&buckBufferDesc)))
		{
			break;
		}

		if (debug->IsFailedHresult(m_swapChain->ResizeBuffers(buckBufferDesc.BufferCount, static_cast<UINT>(width), static_cast<UINT>(height), DX_Graphics::GetInstance()->GetFortmat(), 0)))
		{
			break;
		}

		if (InitBuckBuffer())
		{
			break;
		}

		succeed = true;
	} while (false);

	ID3D11RTV* const rtv[]	= { m_rtv.Get() };
	ID3D11DSV* const dsv	= m_dsv.Get();

	m_deviceContext->OMSetRenderTargets(1, rtv, dsv);

	return succeed;
}

/// <summary>
/// DirectXのデバイス及びデバイスコンテキスト並びにスワップチェインを作成する
/// </summary>
/// <param name="hWnd">ウィンドウのインスタンスハンドル</param>
bool DX_System::CreateDeviceAndSwapChain(const HWND hWnd)
{
	bool succeed = false;

	DX_Graphics* graphics = DX_Graphics::GetInstance();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };

	//	バックバッファ数
	swapChainDesc.BufferCount = 2;

	//	バックバッファの幅と高さ
	swapChainDesc.BufferDesc.Width	= m_screenWidth;
	swapChainDesc.BufferDesc.Height	= m_screenHeight;

	//	フォーマット
	swapChainDesc.BufferDesc.Format = graphics->GetFortmat();

	//	リフレッシュレートの分母と分子
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= graphics->GetRefreshRateN();
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= graphics->GetRefreshRateD();

	//	スキャンラインとスケーリング
	swapChainDesc.BufferDesc.ScanlineOrdering	= graphics->GetScanLineOrder();
	swapChainDesc.BufferDesc.Scaling			= graphics->GetScaling();

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
	D3D_FEATURE_LEVEL featureLevels[] =
	{
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
	D3D_DRIVER_TYPE driveTypes[] = 
	{
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
	for (auto i = 0; i < _countof(driveTypes); ++i)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driveTypes[i],
			nullptr,
			createDeviecFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&m_swapChain,
			&m_device,
			&m_featureLevel,
			&m_deviceContext);

		if (SUCCEEDED(hr))
		{
			m_driverType = driveTypes[i];
			succeed = true;
			
			m_device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_device") - 1, "m_device");
			m_deviceContext->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_deviceContext") - 1, "m_deviceContext");
			m_swapChain->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_swapChain") - 1, "m_swapChain");
			
			break;
		}
	}

	return succeed;
}

/// <summary>
/// レンダーターゲットビューを作成する
/// </summary>
/// <returns>成否</returns>
bool DX_System::CreateRenderTargetView()
{
	bool succeed = false;

	do
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> buckBuffer;
		DX_Debug* debug = DX_Debug::GetInstance();

		if (debug->IsFailedHresult(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buckBuffer)))
		{
			TRACE("failed to GetBuffer()");
			break;
		}

		if (debug->IsFailedHresult(m_device->CreateRenderTargetView(buckBuffer.Get(), nullptr, &m_rtv)))
		{
			TRACE("failed to CreateRenderTargetView()");
			break;
		}

		m_rtv->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_rtv") - 1, "m_rtv");

		succeed = true;
	} while (false);

	return succeed;
}

/// <summary>
/// 深度・ステンシルバッファを作成する
/// </summary>
/// <returns>成否<</returns>
bool DX_System::CreateDepthStencilBuffer()
{
	bool succeed = false;

	do
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> buckBuffer;
		DX_Debug* debug = DX_Debug::GetInstance();

		if (debug->IsFailedHresult(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buckBuffer)))
		{
			TRACE("faield to GetBuffer()");
			break;
		}

		D3D11_TEXTURE2D_DESC descBackBuffer;
		buckBuffer->GetDesc(&descBackBuffer);

		D3D11_TEXTURE2D_DESC dsbDesc = descBackBuffer;
		dsbDesc.MipLevels		= 1;
		dsbDesc.ArraySize		= 1;
		dsbDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
		dsbDesc.Format			= DXGI_FORMAT_D32_FLOAT;
		dsbDesc.Usage			= D3D11_USAGE_DEFAULT;      // デフォルト使用法
		dsbDesc.CPUAccessFlags	= 0;   // CPUからはアクセスしない
		dsbDesc.MiscFlags		= 0;   // その他の設定なし

		if (debug->IsFailedHresult(m_device->CreateTexture2D(&dsbDesc, nullptr, &m_dsb)))
		{
			TRACE("failed to CreateTexture2D()");
			break;
		}

		m_dsb->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_dsb") - 1, "m_dsb");

		succeed = true;
	} while (false);

	return succeed;
}

/// <summary>
/// 深度・ステンシルビューを作成
/// </summary>
/// <returns>成否</returns>
bool DX_System::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC desc = { NULL };
	m_dsb->GetDesc(&desc);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format			= desc.Format;
	dsvDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;


	bool succeed = false;

	if (DX_Debug::GetInstance()->CheckHresult(m_device->CreateDepthStencilView(m_dsb.Get(), &dsvDesc, &m_dsv)))
	{
		succeed = true;
	} 
	else
	{
		TRACE("failed to CreateDepthStencilView()");
	}

	m_dsv->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("m_dsv") - 1, "m_dsv");

	return succeed;
}

