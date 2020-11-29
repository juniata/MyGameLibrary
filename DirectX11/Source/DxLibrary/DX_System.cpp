#include	"DX_Library.h"
//#include	"../user_helper_class/ALManager.h"
//#include	"../user_helper_class/OGGManager.h"
#include	<stdio.h>

/// <summary>
/// �����o�ϐ�������������
/// </summary>
DX_System::DX_System() :
	m_featureLevel(D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1),
	m_driverType(D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN),
	m_windowHandle(NULL),
	m_screenHeight(0),
	m_screenWidth(0)
{}

/// <summary>
/// DX�ɂ�����邷�ׂẴf�o�C�X���̉�����s��
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
	//	OGG�t�@�C�������
	//	OGGManager::Release();

	//	ALManager���������
	//ALManager::Release();
	//DX_Debug::Destroy();
}

/// <summary>
/// DirectX�̃f�o�C�X��������������
/// </summary>
/// <param name="hWnd">�E�B���h�E�̃C���X�^���X�n���h��</param>
/// <param name="hInst">�E�B���h�E�̃C���X�^���X</param>
/// <param name="width">�`�悷�镝</param>
/// <param name="height">�`�悷�鍂��</param>
/// <returns>����</returns>
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
/// �X�N���[���̍������擾����
/// </summary>
/// <returns>�X�N���[���̍���</returns>
UINT DX_System::GetScreenHeight()
{
	return m_screenHeight; 
}

/// <summary>
/// �X�N���[���̕����擾����
/// </summary>
/// <returns>�X�N���[���̕�</returns>
UINT DX_System::GetScreenWidth()
{
	return m_screenWidth; 
}

/// <summary>
/// DirectX�̃f�o�C�X���擾����
/// </summary>
/// <returns>�f�o�C�X</returns>
ID3D11Device* DX_System::GetDevice()
{
	return m_device.Get();
}

/// <summary>
/// DIrectX�̃f�o�C�X�R���e�L�X�g���擾����
/// </summary>
/// <returns>�f�o�C�X�R���e�L�X�g</returns>
ID3D11DeviceContext*	DX_System::GetDeviceContext()
{
	return m_deviceContext.Get();
}

/// <summary>
/// �X���b�v�`�F�C�����擾����
/// </summary>
/// <returns>�X���b�v�`�F�C��</returns>
IDXGISwapChain*		DX_System::GetSwapChain()
{
	return m_swapChain.Get();
}

/// <summary>
/// FeatureLevel���擾����
/// </summary>
/// <returns>FeatureLevel</returns>
D3D_FEATURE_LEVEL DX_System::GetFeatureLevel()
{
	return m_featureLevel;
}

/// <summary>
/// DriverType���擾����
/// </summary>
/// <returns>DriverType</returns>
D3D_DRIVER_TYPE DX_System::GetDriverType()
{
	return m_driverType;
}

/// <summary>
/// �����_�[�^�[�Q�b�g���擾����
/// </summary>
/// <returns>�����_�[�^�[�Q�b�g</returns>
ID3D11RenderTargetView*	DX_System::GetRenderTargetView()
{
	return m_rtv.Get();
}

/// <summary>
/// �[�x�E�X�e���V���r���[���擾����
/// </summary>
/// <returns>�[�x�E�X�e���V���r���[</returns>
ID3D11DepthStencilView*	DX_System::GetDepthStencilView()
{
	return m_dsv.Get();
}

/// <summary>
/// �[�x�E�X�e���V���o�b�t�@���擾����
/// </summary>
/// <returns>�[�x�E�X�e���V���o�b�t�@</returns>
ID3D11Texture2D*		DX_System::GetDepthStencilBuffer()
{
	return m_dsb.Get();
}


/// <summary>
/// �E�B���h�E�̃C���X�^���X�n���h�����擾����
/// </summary>
/// <returns>�E�B���h�E�̃C���X�^���X�n���h��</returns>
HWND	DX_System::GetWindowHandle()
{
	return m_windowHandle;
}

/// <summary>
/// �o�b�N�o�b�t�@������������
/// </summary>
/// <returns>����</returns>
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
/// ��ʕύX�ɔ����o�b�t�@�̃��T�C�Y���s��
/// </summary>
/// <param name="width">�X�N���[���̕�</param>
/// <param name="height">�X�N���[���̍���</param>
/// <returns>����</returns>
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
		// TODO:���s�������A���̃X�N���[���T�C�Y�őΉ����鏈�����K�v�B
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
/// DirectX�̃f�o�C�X�y�уf�o�C�X�R���e�L�X�g���тɃX���b�v�`�F�C�����쐬����
/// </summary>
/// <param name="hWnd">�E�B���h�E�̃C���X�^���X�n���h��</param>
bool DX_System::CreateDeviceAndSwapChain(const HWND hWnd)
{
	bool succeed = false;

	DX_Graphics* graphics = DX_Graphics::GetInstance();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };

	//	�o�b�N�o�b�t�@��
	swapChainDesc.BufferCount = 2;

	//	�o�b�N�o�b�t�@�̕��ƍ���
	swapChainDesc.BufferDesc.Width	= m_screenWidth;
	swapChainDesc.BufferDesc.Height	= m_screenHeight;

	//	�t�H�[�}�b�g
	swapChainDesc.BufferDesc.Format = graphics->GetFortmat();

	//	���t���b�V�����[�g�̕���ƕ��q
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= graphics->GetRefreshRateN();
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= graphics->GetRefreshRateD();

	//	�X�L�������C���ƃX�P�[�����O
	swapChainDesc.BufferDesc.ScanlineOrdering	= graphics->GetScanLineOrder();
	swapChainDesc.BufferDesc.Scaling			= graphics->GetScaling();

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

	//	�n�[�h�^�C�v
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
/// �����_�[�^�[�Q�b�g�r���[���쐬����
/// </summary>
/// <returns>����</returns>
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
/// �[�x�E�X�e���V���o�b�t�@���쐬����
/// </summary>
/// <returns>����<</returns>
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
		dsbDesc.Usage			= D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
		dsbDesc.CPUAccessFlags	= 0;   // CPU����̓A�N�Z�X���Ȃ�
		dsbDesc.MiscFlags		= 0;   // ���̑��̐ݒ�Ȃ�

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
/// �[�x�E�X�e���V���r���[���쐬
/// </summary>
/// <returns>����</returns>
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

