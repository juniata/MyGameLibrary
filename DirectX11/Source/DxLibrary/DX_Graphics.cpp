#include	"DX_Library.h"
//-----------------------------------------------------------------------------------------
//
//	�}�N����`
//
//-----------------------------------------------------------------------------------------
#define FPS_60 1
#define FPS_30 2

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
 bool						DX_Graphics::m_standByMode					= false;
 unsigned int				DX_Graphics::m_refreshRateN					= 0;
 unsigned int				DX_Graphics::m_refreshRateD					= 0;
 unsigned int				DX_Graphics::m_videoCardMemory				= 0;
 char						DX_Graphics::m_videoCardDescription[128]	= {"\0"};
 DXGI_MODE_SCANLINE_ORDER	DX_Graphics::m_scanlineOrder				= DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
 DXGI_MODE_SCALING			DX_Graphics::m_scaling						= DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
 DXGI_FORMAT				DX_Graphics::m_format						= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

//-----------------------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------------------
bool DX_Graphics::Initialize()
{
	auto result = false;

	//	�ϐ���`
	Microsoft::WRL::ComPtr<IDXGIFactory>	factory;
	Microsoft::WRL::ComPtr<IDXGIAdapter>	adapter;
	Microsoft::WRL::ComPtr<IDXGIOutput>		output;
	DXGI_MODE_DESC*		pDisplayModeList	= nullptr;
	DXGI_ADAPTER_DESC	adapterDesc			= { NULL };
	UINT				numModes			= 0;


	const char* pErrMsg = nullptr;

	do {
		//	factry���쐬
		if (!DX_Debug::GetInstance()->CheckHresult(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory))) {
			pErrMsg = "failed to CreateDXGIFactory()";
			break;
		}

		// adapters���쐬
		if (!DX_Debug::GetInstance()->CheckHresult(factory->EnumAdapters(0, &adapter))) {
			pErrMsg = "failed to EnumAdapters()";
			break;
		}

		//	enumOutput���쐬
		if (!DX_Debug::GetInstance()->CheckHresult(adapter->EnumOutputs(0, &output))) {
			pErrMsg = "failed to EnumOutputs()";
			break;
		}

		//	displayModeList�̐����擾
		if (!DX_Debug::GetInstance()->CheckHresult(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList))) {
			pErrMsg = "failed to GetDisplayModeList()";
			break;
		}

		//	list�̐���������
		pDisplayModeList = new DXGI_MODE_DESC[numModes];

		//	list�̓��e���擾
		if (!DX_Debug::GetInstance()->CheckHresult(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList))) {
			pErrMsg = "failed to GetDisplayModeList()";
			break;
		}

		DX_System* pSystem = DX_System::GetInstance();
		unsigned int width = pSystem->GetScreenWidth();
		unsigned int height = pSystem->GetScreenHeight();


		//	�E�B���h�E�T�C�Y�Ɉ�v����GPU�����擾
		//	�E�B���h�E�T�C�Y�Ɉ�v���郊�t���b�V���V�[�g���擾
		for (UINT i = 0; i < numModes; ++i) {
			DXGI_MODE_DESC displayMode = pDisplayModeList[i];

			//DX_Debug::Printf("Width = %d", displayMode.Width);
			//DX_Debug::Printf(" height = %d", displayMode.Height);
			//DX_Debug::Printf(" m_refreshRateD = %d", displayMode.RefreshRate.Denominator);
			//DX_Debug::Printf(" m_refreshRateN = %d", displayMode.RefreshRate.Numerator);
			//DX_Debug::Printf(" m_scaling = %d", displayMode.Scaling);
			//DX_Debug::Printf(" m_scanlineOrder = %d", displayMode.ScanlineOrdering);
			//DX_Debug::Printf(" m_format = %d", displayMode.Format);
			//DX_Debug::Printf("\n\n");
			//	�X�N���[���̕�����v���Ă���ꍇ
			if (displayMode.Width == width && displayMode.Height == height) {
				//	GPU�̏����擾
				m_refreshRateD = displayMode.RefreshRate.Denominator;
				m_refreshRateN = displayMode.RefreshRate.Numerator;
				m_scaling = displayMode.Scaling;
				m_scanlineOrder = displayMode.ScanlineOrdering;
				m_format = displayMode.Format;
			}
		}

		//	���t���b�V�����[�g���擾�ł��Ă��Ȃ��ꍇ
		if (m_refreshRateD == 0 && m_refreshRateN == 0) {
			pErrMsg = "Get RefreshReate failed";
			break;
		}
		
		//	DXGI_ADAPTER_DESC���擾
		if (!DX_Debug::GetInstance()->CheckHresult(adapter->GetDesc(&adapterDesc))) {
			pErrMsg = "failed to GetDesc()";
			break;
		}

		//	byte��MegaByte
		m_videoCardMemory = DX::CAST::UI(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		//	�������ϊ�
		size_t	l_stringLendth = 0;
		if (::wcstombs_s(&l_stringLendth, m_videoCardDescription, sizeof(m_videoCardDescription), adapterDesc.Description, sizeof(m_videoCardDescription)) != 0) {
			pErrMsg = "failed to wcstombs_s()";
			break;
		}

		result = true;

	} while (false);

	DELETE_OBJ_ARRAY(pDisplayModeList);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//	�`��J�n
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::BeginRender(IDXGISwapChain* pSwapChain)
{
	if (m_standByMode){
		//	�ŏ����Ȃǂɂ���Ă���A�`�悷��K�v�������ꍇ��return
		if (pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED){ 
			return; 
		}
		m_standByMode = false;
	}
}

//-----------------------------------------------------------------------------------------
//
//	�`��I��
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::EndRender(IDXGISwapChain* pSwapChain)
{
	if (m_standByMode == false) {
		if (pSwapChain->Present(1, 0) == DXGI_STATUS_OCCLUDED) {
			m_standByMode = true; 
		}
	}

}


//-----------------------------------------------------------------------------------------
//
//	���t���b�V���V�[�g�̕�����擾
//
//-----------------------------------------------------------------------------------------
unsigned int DX_Graphics::GetRefreshRateN()
{
	return m_refreshRateN;
}

//-----------------------------------------------------------------------------------------
//
//	���t���b�V���V�[�g�̕��q���擾
//
//-----------------------------------------------------------------------------------------
unsigned int DX_Graphics::GetRefreshRateD()
{
	return m_refreshRateD;
}

//-----------------------------------------------------------------------------------------
//
//	�r�f�I�J�[�h�̃������ʂ̎擾
//
//-----------------------------------------------------------------------------------------
unsigned int DX_Graphics::GetVieoCardMemory()
{
	return m_videoCardMemory;
}
//-----------------------------------------------------------------------------------------
//
//	�r�f�I�J�[�h�̖��O���擾
//
//-----------------------------------------------------------------------------------------
const char* DX_Graphics::GetVideoCardDescription()
{
	return m_videoCardDescription;
}

//-----------------------------------------------------------------------------------------
//
//	�X�L�������C���I�[�_�[���擾
//
//-----------------------------------------------------------------------------------------
DXGI_MODE_SCANLINE_ORDER DX_Graphics::GetScanLineOrder()
{
	return m_scanlineOrder;
}

//-----------------------------------------------------------------------------------------
//
//	�X�P�[�����O���擾
//
//-----------------------------------------------------------------------------------------
DXGI_MODE_SCALING DX_Graphics::GetScaling()
{
	return m_scaling;
}

//-----------------------------------------------------------------------------------------
//
//	�t�H�[�}�b�g�擾
//
//-----------------------------------------------------------------------------------------
DXGI_FORMAT	DX_Graphics::GetFortmat()
{
	return m_format;
}

