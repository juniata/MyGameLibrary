#include	"DX_Library.h"
#include	<wrl/client.h>
using namespace Microsoft::WRL;

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
DX_Graphics* DX_Graphics::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------------------
DX_Graphics::DX_Graphics() :
	m_stand_by_mode(false),
	m_refreshRateD(0),
	m_refreshRateN(0),
	m_videoCardMemory(0),
	m_scaling(DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED),
	m_format(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
{
	ZeroMemory(m_videoCardDescription, sizeof(m_videoCardDescription));
}

//-----------------------------------------------------------------------------------------
//
//	���
//
//-----------------------------------------------------------------------------------------
DX_Graphics::~DX_Graphics()
{}

//-----------------------------------------------------------------------------------------
//
//	�C���X�^���X���擾����
//
//-----------------------------------------------------------------------------------------
DX_Graphics* DX_Graphics::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_Graphics();
	}

	return m_pInstance;
}
//-----------------------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::Initialize()
{
	PROFILE("DX_Graphics::Initialize()");

	DX_Graphics* pGraphics = DX_Graphics::GetInstance();

	//	�ϐ���`
	ComPtr<IDXGIFactory>	factory;
	ComPtr<IDXGIAdapter>	adapter;
	ComPtr<IDXGIOutput>		output;
	DXGI_MODE_DESC*		pDisplayModeList  = nullptr;
	DXGI_ADAPTER_DESC	adpterDesc = { NULL };
	unsigned int		numModes		= 0;

	DX_Debug* pDebugInstance = DX_Debug::GetInstance();
	
	//	factry���쐬
	pDebugInstance->ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), "CreateDXGIFactory() : failed");

	// adapters���쐬
	pDebugInstance->ThrowIfFailed(factory->EnumAdapters(0, &adapter), "IDXGIFactory::EnumAdapters() : failed");

	//	enumOutput���쐬
	pDebugInstance->ThrowIfFailed(adapter->EnumOutputs(0, &output), "IDXGIAdapter::EnumOutputs() : failed");

	//	displayModeList�̐����擾
	pDebugInstance->ThrowIfFailed(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList), "IDXGIOutput::GetDisplayModeList() : DXGI_MODE_DESC element faile");

	//	list�̐���������
	pDisplayModeList = new DXGI_MODE_DESC[numModes];

	//	list�̓��e���擾
	pDebugInstance->ThrowIfFailed(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList), "IDXGIOutput::GetDisplayModeList() : numModels failed");

	unsigned int width	= DX_System::GetWindowWidth();
	unsigned int height = DX_System::GetWindowHeight();

	//	�E�B���h�E�T�C�Y�Ɉ�v����GPU�����擾
	//	�E�B���h�E�T�C�Y�Ɉ�v���郊�t���b�V���V�[�g���擾
	for (unsigned int i = 0; i < numModes; ++i) {
		DXGI_MODE_DESC displayMode = pDisplayModeList[i];

		//	�X�N���[���̕�����v���Ă���ꍇ
		if (displayMode.Width == width && displayMode.Height == height) {
			//	GPU�̏����擾
			pGraphics->m_refreshRateD	= displayMode.RefreshRate.Denominator;
			pGraphics->m_refreshRateN	= displayMode.RefreshRate.Numerator;
			pGraphics->m_scaling		= displayMode.Scaling;
			pGraphics->m_scanlineOrder = displayMode.ScanlineOrdering;
			pGraphics->m_format		= displayMode.Format;
		}
	}

	//	�������
	DELETE_OBJ_ARRAY(pDisplayModeList);

	//	���t���b�V�����[�g���擾�ł��Ă��Ȃ��ꍇ
	if (pGraphics->m_refreshRateD == 0 && pGraphics->m_refreshRateN == 0) {
		MessageBox(NULL, "Get RefreshReate failed", "error", MB_OK);
		throw;
	}

	//	DXGI_ADAPTER_DESC���擾
	if (!DX_Debug::GetInstance()->IsHresultCheck(adapter->GetDesc(&adpterDesc))) {
		MessageBox(NULL, "IDXGIAdapter::GetDesc(): failed", "error", MB_OK);
		throw;
	}

	//	byte��MegaByte
	pGraphics->m_videoCardMemory = unsigned int(adpterDesc.DedicatedVideoMemory / 1024 / 1024);

	//	�������ϊ�
	size_t	l_stringLendth = 0;
	if (::wcstombs_s(&l_stringLendth, pGraphics->m_videoCardDescription, sizeof(pGraphics->m_videoCardDescription), adpterDesc.Description, sizeof(pGraphics->m_videoCardDescription)) != 0) {
		MessageBox(NULL, "wcstombs_s() failed", "error", MB_OK);
		throw;
	}

	
}

//-----------------------------------------------------------------------------------------
//
//	�C���X�^���X�����
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//	�`��J�n
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::BeginRender(ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain)
{
	if (m_stand_by_mode){
		//	�ŏ����Ȃǂɂ���Ă���A�`�悷��K�v�������ꍇ��return
		if (pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED){ return; }
		m_stand_by_mode = false;
	}


	DX_View::Clear(pContext);

	DX_System* pSystem = DX_System::GetInstance();
	DX_RenderState* pRenderState = DX_RenderState::GetInstance();
	
	//	�T���v���[��ݒ肷��
	ID3D11SamplerState* const sampler[1] = { pRenderState->GetDefaultSamplerState() };
	pContext->PSSetSamplers(0, 1, sampler);

	//	�|���S���`��ݒ�
	pContext->RSSetState(pRenderState->GetDefaultRasterizerState());

	//	RS�Ƀr���[�|�[�g��ݒ�
	pContext->RSSetViewports(1, DX_View::GetViewPort());

	//	OM�ɕK�v����ݒ�
	float l_blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11RenderTargetView* const targets[1] = { pSystem->GetDefaultRenderTargetView() };
	pContext->OMSetRenderTargets(1, targets, pSystem->GetDefaultDepthStencilView());
	pContext->OMSetDepthStencilState(pRenderState->GetDefaultDepthStencilState(), 1);
	pContext->OMSetBlendState(pRenderState->GetDefaultBlendState(), l_blendFactor, 0xffffffff);
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

