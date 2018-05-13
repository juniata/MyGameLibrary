#ifndef __DX_GRAPHICS_H_
#define __DX_GRAPHICS_H_

//****************************************************************************************************
//
//	Graphics
//
//****************************************************************************************************
class Graphics
{
private:
	//	��ʂ��J����Ă��鎞�����`����s�����߂̃t���O
	static bool m_stand_by_mode;

	//	���t���b�V���V�[�g�̕���ƕ��q
	static unsigned int m_refreshRateN;	//	���q nolecule
	static unsigned int m_refreshRateD;	//	���� denominator

	//	�r�f�I�J�[�h�̃�������
	static unsigned int m_videoCardMemory;

	//	�r�f�I�J�[�h�̖��O
	static char m_videoCardDescription[128];

	//	�X�L�������C��
	static DXGI_MODE_SCANLINE_ORDER m_scanlineOrder;

	//	�X�P�[�����O
	static DXGI_MODE_SCALING		m_scaling;

	//	�t�H�[�}�b�g
	static DXGI_FORMAT				m_format;


	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	Graphics(){}
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	~Graphics(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		Graphics������������
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		�`��J�n
	//	@param[in]	pSwapChain DX_System::GetSwapChain()
	//
	//------------------------------------------------------------------------------
	static void BeginRender(
		IDXGISwapChain* pSwapChain
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�`��I��
	//	@param[in]	pSwapChain DX_System::GetSwapChain()
	//
	//------------------------------------------------------------------------------
	static void EndRender(
		IDXGISwapChain* pSwapChain
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		���t���b�V���V�[�g�̕�����擾
	//	@return		m_refreshRateN
	//
	//------------------------------------------------------------------------------
	static unsigned int GetRefreshRateN();

	//------------------------------------------------------------------------------
	//
	//  @brief		���t���b�V���V�[�g�̕�����擾
	//	@return		m_refreshRateD
	//
	//------------------------------------------------------------------------------
	static unsigned int GetRefreshRateD();

	//------------------------------------------------------------------------------
	//
	//  @brief		�r�f�I�J�[�h�̃������ʂ̎擾
	//	@return		m_videoCardMemory
	//
	//------------------------------------------------------------------------------
	static unsigned int GetVieoCardMemory();

	//------------------------------------------------------------------------------
	//
	//  @brief		�r�f�I�J�[�h�̃������ʂ̎擾
	//	@return		m_videoCardDescription
	//
	//------------------------------------------------------------------------------
	static char* GetVideoCardDescription();

	//------------------------------------------------------------------------------
	//
	//  @brief		�X�L�������C���I�[�_�[���擾
	//	@return		m_scanlineOrder
	//
	//------------------------------------------------------------------------------
	static DXGI_MODE_SCANLINE_ORDER GetScanLineOrder();

	//------------------------------------------------------------------------------
	//
	//  @brief		�X�P�[�����O���擾
	//	@return		m_scaling
	//
	//------------------------------------------------------------------------------
	static DXGI_MODE_SCALING GetScaling();

	//------------------------------------------------------------------------------
	//
	//  @brief		�t�H�[�}�b�g�擾
	//	@return		m_format
	//
	//------------------------------------------------------------------------------
	static DXGI_FORMAT	GetFortmat();

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		CreateDXGIFactory�̃��b�p�[
	//	@param[out]	l_pFactory	���IDXGIFactory�|�C���^�ϐ�
	//
	//------------------------------------------------------------------------------
	static void CreateDXGIFactory(
		IDXGIFactory*& pFactary
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		IDXGIFactory::EnumAdapters�̃��b�p�[
	//	@param[in]	pFactory	CreateDXGIFactory()�ō쐬����IDXGIFactory�|�C���^�ϐ�
	//	@param[out]	pAdapter	���IDXGIAdapter�|�C���^�ϐ�
	//
	//------------------------------------------------------------------------------
	static void EnumAdapters(
		IDXGIFactory*	pFactory, 
		IDXGIAdapter*&	pAapter
		);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		IDXGIAdapter::EnumOutputs�̃��b�p�[
	//	@param[in]	pAdapter	EnumAdapters()�ō쐬����IDXGIAdapter�|�C���^�ϐ�
	//	@param[out]	pOutput		���IDXGIOutput�|�C���^�ϐ�
	//
	//------------------------------------------------------------------------------
	static void EnumOutputs(
		IDXGIAdapter* pAdapter,
		IDXGIOutput*& pOutput
		);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		IDXGIOutput::GetDisplayModeList�̃��b�p�[
	//	@param[in]	pOutput					EnumOutputs()�ō쐬����IDXGIOutput�|�C���^�ϐ�
	//	@param[out]	pNumModels				displayModeList�̐����擾
	//	@param[in or out]	pModelList		DXGI_MODE_DESC�|�C���^�ϐ�
	//
	//------------------------------------------------------------------------------
	static void GetDisplayModeList(
		IDXGIOutput*		pOutput, 
		unsigned int&		pNumModels, 
		DXGI_MODE_DESC*&	pModelList
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		IDXGIAdapter::GetDesc�̃��b�p�[
	//	@param[in]	pAdapter		EnumOutputs()�ō쐬����IDXGIAdapter�|�C���^�ϐ�
	//	@param[out] pAdapterDesc	���DXGI_ADAPTER_DESC�ϐ�
	//
	//------------------------------------------------------------------------------
	static void GetDesc(
		IDXGIAdapter*		pAdapter,
		DXGI_ADAPTER_DESC*	pAdapterDesc
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�E�B���h�E�T�C�Y�Ɉ�v�������t���b�V�����[�g���擾
	//	@param[in]	modelsCount			GetDisplayModeList()�Ŏ擾����displayModeList�̐�
	//	@param[in]	windowWidth			DX_System::GetWindowWidth()
	//	@param[in]	windowHeight		DX_System::GetWindowHeight()
	//	@param[in]	pDisplayModeList	displayModeList�Ŕz�񐶐�����DXGI_MODE_DESC�|�C���^�ϐ�
	//
	//------------------------------------------------------------------------------
	static void GetRefreshRate(
		const unsigned int		modelsCount,
		const unsigned int		windowWidth,
		const unsigned int		windowHeight,
		const DXGI_MODE_DESC*	pDisplayModeList
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		wcstombs_s�̃��b�p�[
	//	@param[in]	_Src		������
	//
	//------------------------------------------------------------------------------
	static void wcstombs_s(
		const wchar_t* _Src
		);
};
#endif // !__DX_GRAPHICS_H_
