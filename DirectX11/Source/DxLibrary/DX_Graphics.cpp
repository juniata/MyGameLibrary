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
bool						DX_Graphics::m_stand_by_mode = false;
unsigned int				DX_Graphics::m_refreshRateD = 0;
unsigned int				DX_Graphics::m_refreshRateN = 0;
unsigned int				DX_Graphics::m_videoCardMemory = 0;
char						DX_Graphics::m_videoCardDescription[128];
DXGI_MODE_SCANLINE_ORDER	DX_Graphics::m_scanlineOrder;
DXGI_MODE_SCALING			DX_Graphics::m_scaling;
DXGI_FORMAT					DX_Graphics::m_format;


//-----------------------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::Initialize()
{
	PROFILE("DX_Graphics::Initialize()");

	//	�ϐ���`
	IDXGIFactory*		l_pFactory			= nullptr;
	IDXGIAdapter*		l_pAdapter			= nullptr;
	IDXGIOutput*		l_pOutput			= nullptr;
	DXGI_MODE_DESC*		l_pDisplayModeList  = nullptr;
	DXGI_ADAPTER_DESC	l_adpterDesc = { NULL };
	unsigned int		l_numModes		= 0;

	try {
		//	factry���쐬
		if (!DX_Debug::GetInstance()->IsHresultCheck(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&l_pFactory))) {
			throw "CreateDXGIFactory() : failed";
		}

		// adapters���쐬
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pFactory->EnumAdapters(0, &l_pAdapter))) {
			throw "IDXGIFactory::EnumAdapters() : failed";
		}

		//	enumOutput���쐬
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pAdapter->EnumOutputs(0, &l_pOutput))) {
			throw "IDXGIAdapter::EnumOutputs() : failed";
		}

		//	displayModeList�̐����擾
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_numModes, l_pDisplayModeList))) {
				throw "IDXGIOutput::GetDisplayModeList() : DXGI_MODE_DESC element failed";
		}

		//	list�̐���������
		l_pDisplayModeList = new DXGI_MODE_DESC[l_numModes];

		//	list�̓��e���擾
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_numModes, l_pDisplayModeList))) {
			throw "IDXGIOutput::GetDisplayModeList() : numModels failed";
		}

		unsigned int width	= DX_System::GetWindowWidth();
		unsigned int height = DX_System::GetWindowHeight();

		//	�E�B���h�E�T�C�Y�Ɉ�v����GPU�����擾
		//	�E�B���h�E�T�C�Y�Ɉ�v���郊�t���b�V���V�[�g���擾
		for (unsigned int i = 0; i < l_numModes; ++i) {
			DXGI_MODE_DESC displayMode = l_pDisplayModeList[i];

			//DX_Debug::GetInstance()->Printf("Width = %d", displayMode.Width);
			//DX_Debug::GetInstance()->Printf(" height = %d", displayMode.Height);
			//DX_Debug::GetInstance()->Printf(" m_refreshRateD = %d", displayMode.RefreshRate.Denominator);
			//DX_Debug::GetInstance()->Printf(" m_refreshRateN = %d", displayMode.RefreshRate.Numerator);
			//DX_Debug::GetInstance()->Printf(" m_scaling = %d", displayMode.Scaling);
			//DX_Debug::GetInstance()->Printf(" m_scanlineOrder = %d", displayMode.ScanlineOrdering);
			//DX_Debug::GetInstance()->Printf(" m_format = %d", displayMode.Format);
			//DX_Debug::GetInstance()->Printf("\n\n");
			//	�X�N���[���̕�����v���Ă���ꍇ
			if (displayMode.Width == width && displayMode.Height == height) {
				//	GPU�̏����擾
				m_refreshRateD	= displayMode.RefreshRate.Denominator;
				m_refreshRateN	= displayMode.RefreshRate.Numerator;
				m_scaling		= displayMode.Scaling;
				m_scanlineOrder = displayMode.ScanlineOrdering;
				m_format		= displayMode.Format;
			}
		}
		
		//	���t���b�V�����[�g���擾�ł��Ă��Ȃ��ꍇ
		if (m_refreshRateD == 0 && m_refreshRateN == 0) {
			throw "Get RefreshReate failed";
		}

		//	DXGI_ADAPTER_DESC���擾
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pAdapter->GetDesc(&l_adpterDesc))) {
			throw "IDXGIAdapter::GetDesc(): failed";
		}

		//	byte��MegaByte
		m_videoCardMemory = unsigned int(l_adpterDesc.DedicatedVideoMemory / 1024 / 1024);

		//	�������ϊ�
		size_t	l_stringLendth = 0;
		if (::wcstombs_s(&l_stringLendth, m_videoCardDescription, sizeof(m_videoCardDescription), l_adpterDesc.Description, sizeof(m_videoCardDescription)) != 0) {
			throw "wcstombs_s() failed";
		}

		//	�������
		DELETE_OBJ_ARRAY(l_pDisplayModeList);
		SAFE_RELEASE(l_pFactory);
		SAFE_RELEASE(l_pAdapter);
		SAFE_RELEASE(l_pOutput);
	}
	catch (char* pErrorMsg){
		throw  pErrorMsg;
	}
}


//-----------------------------------------------------------------------------------------
//
//	�`��J�n
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::BeginRender(IDXGISwapChain* pSwapChain)
{
	if (m_stand_by_mode){
		//	�ŏ����Ȃǂɂ���Ă���A�`�悷��K�v�������ꍇ��return
		if (pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED){ return; }
		m_stand_by_mode = false;
	}
}

//-----------------------------------------------------------------------------------------
//
//	�`��I��
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::EndRender(IDXGISwapChain* pSwapChain)
{
	//	�X�^���o�C���[�h�̏ꍇreturn
	if (m_stand_by_mode){ return; }
	if (pSwapChain->Present(FPS_60, 0) == DXGI_STATUS_OCCLUDED){ m_stand_by_mode = true; }

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
char* DX_Graphics::GetVideoCardDescription()
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

