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

	try{
		//	factry���쐬
		CreateDXGIFactory(l_pFactory);

		// adapters���쐬
		EnumAdapters(l_pFactory, l_pAdapter);

		//	enumOutput���쐬
		EnumOutputs(l_pAdapter, l_pOutput);

		//	displayModeList�̐����擾
		GetDisplayModeList(l_pOutput, l_numModes, l_pDisplayModeList);

		//	list�̐���������
		l_pDisplayModeList = new DXGI_MODE_DESC[l_numModes];

		//	list�̓��e���擾
		GetDisplayModeList(l_pOutput, l_numModes, l_pDisplayModeList);

		//	�E�B���h�E�T�C�Y�Ɉ�v����GPU�����擾
		GetRefreshRate(l_numModes, DX_System::GetWindowWidth(), DX_System::GetWindowHeight(), l_pDisplayModeList);

		//	DXGI_ADAPTER_DESC���擾
		GetDesc(l_pAdapter, &l_adpterDesc);

		//	byte��MegaByte
		m_videoCardMemory = unsigned int(l_adpterDesc.DedicatedVideoMemory / 1024 / 1024);

		//	�������ϊ�
		wcstombs_s(l_adpterDesc.Description);

		//	�������
		if (l_pDisplayModeList){ delete[] l_pDisplayModeList; l_pDisplayModeList = nullptr; }
		if (l_pOutput){ l_pOutput->Release(); l_pOutput = nullptr; }
		if (l_pAdapter){ l_pAdapter->Release(); l_pAdapter = nullptr; }
		if (l_pFactory){ l_pFactory->Release(); l_pFactory = nullptr; }
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

//-----------------------------------------------------------------------------------------
//
//	CreateDXGIFactory�̃��b�p�[
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::CreateDXGIFactory(IDXGIFactory*& pFactary)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(::CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactary))){
		throw "CreateDXGIFactory() : failed";
	}
}

//-----------------------------------------------------------------------------------------
//
//	IDXGIFactory::EnumAdapters�̃��b�p�[
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::EnumAdapters(IDXGIFactory* pFactory, IDXGIAdapter*& pAapter)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(pFactory->EnumAdapters(0, &pAapter))){
		throw "IDXGIFactory::EnumAdapters() : failed";
	}
}

//-----------------------------------------------------------------------------------------
//
//	IDXGIAdapter::EnumOutputs�̃��b�p�[
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::EnumOutputs(IDXGIAdapter* pAdapter, IDXGIOutput*& pOutput)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(pAdapter->EnumOutputs(0, &pOutput))){
		throw "IDXGIAdapter::EnumOutputs() : failed";
	}
}

//-----------------------------------------------------------------------------------------
//
//	IDXGIOutput::GetDisplayModeList�̃��b�p�[
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::GetDisplayModeList(IDXGIOutput* pOutput, unsigned int& pNumModels, DXGI_MODE_DESC*& pModelList)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &pNumModels, pModelList))){
		if (pModelList == nullptr){
			throw "IDXGIOutput::GetDisplayModeList() : DXGI_MODE_DESC element failed";
		}
		else{
			throw "IDXGIOutput::GetDisplayModeList() : numModels failed";
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//	IDXGIAdapter::GetDesc�̃��b�p�[
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::GetDesc(IDXGIAdapter* pAdapter, DXGI_ADAPTER_DESC* pAdapterDesc)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(pAdapter->GetDesc(pAdapterDesc))){
		throw "IDXGIAdapter::GetDesc(): failed";
	}
}

//-----------------------------------------------------------------------------------------
//
//	�E�B���h�E�T�C�Y�Ɉ�v�������t���b�V�����[�g���擾
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::GetRefreshRate(
	const unsigned int modelsCount,
	const unsigned int windowWidth,
	const unsigned int windowHeight,
	const DXGI_MODE_DESC* pDisplayModeList
	)
{
	//	�E�B���h�E�T�C�Y�Ɉ�v���郊�t���b�V���V�[�g���擾
	for (unsigned int i = 0; i < modelsCount; ++i){

		//	�X�N���[���̕�����v���Ă���ꍇ
		if (pDisplayModeList[i].Width == windowWidth){

			//	�X�N���[���̍�������v���Ă���ꍇ
			if (pDisplayModeList[i].Height == windowHeight){

				//	GPU�̏����擾
				m_refreshRateD	= pDisplayModeList[i].RefreshRate.Denominator;
				m_refreshRateN	= pDisplayModeList[i].RefreshRate.Numerator;
				m_scaling		= pDisplayModeList[i].Scaling;
				m_scanlineOrder = pDisplayModeList[i].ScanlineOrdering;
				m_format		= pDisplayModeList[i].Format;
			}
		}
	}


	//	���t���b�V�����[�g���擾�ł��Ă��Ȃ��ꍇ
	if (m_refreshRateD == 0 && m_refreshRateN == 0){
		throw "Get RefreshReate failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//	������ϊ�����
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::wcstombs_s(
	const wchar_t * _Src
	)
{
	size_t	l_stringLendth = 0;

	if (::wcstombs_s(
		&l_stringLendth,
		m_videoCardDescription,
		sizeof(m_videoCardDescription),
		_Src,
		sizeof(m_videoCardDescription)
		) != 0)
	{
		throw "wcstombs_s() failed";
	}
}