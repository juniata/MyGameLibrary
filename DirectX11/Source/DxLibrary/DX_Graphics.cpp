#include	"DX_Library.h"

/// <summary>
/// �����o�ϐ��̏��������s��
/// </summary>
DX_Graphics::DX_Graphics() :
	m_standByMode(false),
	m_refreshRateN(0),
	m_refreshRateD(0),
	m_videoCardMemory(0),
	m_scanlineOrder(DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED),
	m_scaling(DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED),
	m_format(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
{
	ZeroMemory(m_videoCardDescription, sizeof(m_videoCardDescription));
}

/// <summary>
/// ���j�^�[�Ɋւ�������擾���s���B
/// </summary>
/// <returns>����</returns>
bool DX_Graphics::Initialize()
{
	auto succeed = false;

	DXGI_MODE_DESC*		displayModeList	= nullptr;

	do
	{
		DX_Debug* debug = DX_Debug::GetInstance();

		Microsoft::WRL::ComPtr<IDXGIFactory>	factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter>	adapter;
		Microsoft::WRL::ComPtr<IDXGIOutput>		output;
		DXGI_ADAPTER_DESC	adapterDesc = { NULL };
		UINT				numModes	= 0;

		
		if (debug->IsFailedHresult(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
		{
			TRACE("failed to CreateDXGIFactory()")
			break;
		}

		if (debug->IsFailedHresult(factory->EnumAdapters(0, &adapter)))
		{
			TRACE("failed to EnumAdapters()")
			break;
		}

		if (debug->IsFailedHresult(adapter->EnumOutputs(0, &output)))
		{
			TRACE("failed to EnumOutputs()")
			break;
		}

		if (debug->IsFailedHresult(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
		{
			TRACE("failed to GetDisplayModeList()")
			break;
		}

		displayModeList = new DXGI_MODE_DESC[numModes];

		if (debug->IsFailedHresult(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
		{
			TRACE("failed to GetDisplayModeList()")
			break;
		}

		DX_System* systme = DX_System::GetInstance();
		UINT width	= systme->GetScreenWidth();
		UINT height = systme->GetScreenHeight();


		//	�E�B���h�E�T�C�Y�Ɉ�v����GPU�����擾
		//	�E�B���h�E�T�C�Y�Ɉ�v���郊�t���b�V���V�[�g���擾
		for (UINT i = 0; i < numModes; ++i) 
		{
			DXGI_MODE_DESC displayMode = displayModeList[i];

			//DX_Debug::Printf("Width = %d", displayMode.Width);
			//DX_Debug::Printf(" height = %d", displayMode.Height);
			//DX_Debug::Printf(" m_refreshRateD = %d", displayMode.RefreshRate.Denominator);
			//DX_Debug::Printf(" m_refreshRateN = %d", displayMode.RefreshRate.Numerator);
			//DX_Debug::Printf(" m_scaling = %d", displayMode.Scaling);
			//DX_Debug::Printf(" m_scanlineOrder = %d", displayMode.ScanlineOrdering);
			//DX_Debug::Printf(" m_format = %d", displayMode.Format);
			//DX_Debug::Printf("\n\n");
			if (displayMode.Width == width && displayMode.Height == height)
			{
				m_refreshRateD	= displayMode.RefreshRate.Denominator;
				m_refreshRateN	= displayMode.RefreshRate.Numerator;
				m_scaling		= displayMode.Scaling;
				m_scanlineOrder = displayMode.ScanlineOrdering;
				m_format		= displayMode.Format;
			}
		}

		if (m_refreshRateD == 0 || m_refreshRateN == 0)
		{
			TRACE("failed to get refreshRate")
			break;
		}
		
		//	DXGI_ADAPTER_DESC���擾
		if (debug->IsFailedHresult(adapter->GetDesc(&adapterDesc)))
		{
			TRACE("failed to GetDesc()")
			break;
		}

		//	�o�C�g���烁�K�o�C�g�ɒP�ʕϊ�����
		m_videoCardMemory = static_cast<unsigned int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		//	�������ϊ�
		size_t	stringLendth = 0;
		if (::wcstombs_s(&stringLendth, m_videoCardDescription, sizeof(m_videoCardDescription), adapterDesc.Description, sizeof(m_videoCardDescription)) != 0)
		{
			// TODO:���O�ɂ͎c�����A�����܂ŏd�v�ł͂Ȃ��̂�break�͂��Ȃ��B
			TRACE("failed to wcstombs_s()");
		}

		succeed = true;

	} while (false);

	DELETE_OBJ_ARRAY(displayModeList);

	return succeed;
}

/// <summary>
/// �`��J�n
/// </summary>
void DX_Graphics::BeginRender()
{
	if (m_standByMode)
	{
		//	�ŏ����Ȃǂɂ���Ă���A�`�悷��K�v�������ꍇ��return
		if (DX_System::GetInstance()->GetSwapChain()->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
		{ 
			return; 
		}
		m_standByMode = false;
	}
}

/// <summary>
/// �`��I��
/// </summary>
void DX_Graphics::EndRender()
{
	if (m_standByMode == false)
	{
		if (DX_System::GetInstance()->GetSwapChain()->Present(1, 0) == DXGI_STATUS_OCCLUDED)
		{
			m_standByMode = true; 
		}
	}

}


/// <summary>
/// ���t���b�V���V�[�g�̕�����擾
/// </summary>
/// <returns>���t���b�V���V�[�g�̕���</returns>
unsigned int DX_Graphics::GetRefreshRateN()
{
	return m_refreshRateN;
}

/// <summary>
/// ���t���b�V���V�[�g�̕��q���擾
/// </summary>
/// <returns>���t���b�V���V�[�g�̕��q</returns>
unsigned int DX_Graphics::GetRefreshRateD()
{
	return m_refreshRateD;
}

/// <summary>
/// �r�f�I�J�[�h�̃������ʂ̎擾
/// </summary>
/// <returns>�r�f�I�J�[�h�̃�������</returns>
unsigned int DX_Graphics::GetVieoCardMemory()
{
	return m_videoCardMemory;
}

/// <summary>
///�@�r�f�I�J�[�h�̖��O���擾
/// </summary>
/// <returns>�r�f�I�J�[�h�̖��O</returns>
const char* DX_Graphics::GetVideoCardDescription()
{
	return m_videoCardDescription;
}

/// <summary>
/// �X�L�������C���I�[�_�[���擾
/// </summary>
/// <returns>�X�L�������C���I�[�_�[</returns>
DXGI_MODE_SCANLINE_ORDER DX_Graphics::GetScanLineOrder()
{
	return m_scanlineOrder;
}

/// <summary>
/// �X�P�[�����O���擾
/// </summary>
/// <returns>�X�P�[�����O</returns>
DXGI_MODE_SCALING DX_Graphics::GetScaling()
{
	return m_scaling;
}

/// <summary>
/// �t�H�[�}�b�g���擾
/// </summary>
/// <returns>�t�H�[�}�b�g</returns>
DXGI_FORMAT	DX_Graphics::GetFortmat()
{
	return m_format;
}

