#ifndef __DX_DEBUG_H_
#define __DX_DEBUG_H_

//	�R���\�[����ʂɏo�͂��镶���̐F
enum class SET_PRINT_COLOR : int {
	BLUE,		//	�F
	RED,		//	�ԐF
	GREEN,		//	��	
	DEFAULT,	//	�����̐F
};


//****************************************************************************************************
//
//	debug���̂݁A�l�`�F�b�N������B
//
//****************************************************************************************************
#if defined(DEBUG) || defined(_DEBUG)
template<class T> bool DebugValueCheck(T value, const char* pErrMsg)
{
	bool isSucceed = value ? true : false;
	
	if (isSucceed == false) {
		MessageBox(DX_System::GetInstance()->GetWindowHandle(), pErrMsg,  "Error", MB_OK);
	}

	return isSucceed;
}


void __Trace(const char* pFile, int line, LPCSTR pszFormat, ...);
#define TRACE(x)			__Trace(__FILE__, __LINE__, x)
#define TRACE2(x, x2)		__Trace(__FILE__, __LINE__, x, x2)
#define TRACE3(x, x2, x3)	__Trace(__FILE__, __LINE__, x, x2, x3)

#else
#define DebugValueCheck(bFlag, pMessage) true
#define TRACE(x)
#define TRACE2(x, x2)
#define TRACE3(x, x2, x3)
#endif

struct IDXGIDebug;
//****************************************************************************************************
//
//	DX_Debug
//
//****************************************************************************************************
class DX_Debug : public DX_Singleton<DX_Debug>
{
	friend class DX_Singleton<DX_Debug>;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ��̏��������s��
	//
	//------------------------------------------------------------------------------
	void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D�n�̐����I�u�W�F�N�g�𒲍�����
	//
	//------------------------------------------------------------------------------
	void ReportLiveDeviceObjects(const char* pMessage);

	//------------------------------------------------------------------------------
	//
	//  @brief		�߂�l���`�F�b�N����
	//	@param[in]	hr	���܂��܂Ȋ֐��̖߂�l(D3D�n)
	//	@retrun		true:����	false:���s
	//
	//------------------------------------------------------------------------------
	bool CheckHresult(HRESULT hr);

	//------------------------------------------------------------------------------
	//
	//  @brief		�߂�l���`�F�b�N����
	//	@param[in]	hr	���܂��܂Ȋ֐��̖߂�l(D3D�n)
	//	@retrun		true:����	false:���s
	//
	//------------------------------------------------------------------------------
	bool IsFailedHresult(HRESULT hr);

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�t�@�C�����`�F�b�N
	//	@param[in]	hr			D3DX11CompileFromFile()�̖߂�l
	//	@param[in]	pBytecord	D3DX11CompileFromFile()�ŏo�����o�C�g�R�[�h
	//
	//------------------------------------------------------------------------------
	void CheckSourceCordOfShaderFile(HRESULT hr, ID3DBlob* pBytecord);

	//------------------------------------------------------------------------------
	//
	//  @brief		�R���\�[����ʂɏo�͂��镶���̐F��ݒ�
	//	@param[in]	printColor	SET_PRINT_COLOR
	//	
	//------------------------------------------------------------------------------
	void SetPrintColor(SET_PRINT_COLOR printColor);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		�R���\�[����ʂɕ������o�͂���
	//	@param[in]	pFormat printf�̃��b�p�[�Ȃ̂œ����g����
	//	@note		debug���̂ݏo��
	//	
	//------------------------------------------------------------------------------
	void Printf(const char* pFormat, ...);
private:
#if defined(DEBUG) || defined(_DEBUG)
	Microsoft::WRL::ComPtr<IDXGIDebug> m_debug;
	HANDLE	m_consoleHandle;
	CONSOLE_SCREEN_BUFFER_INFO m_csbi;
#endif

	//------------------------------------------------------------------------------
	//
	//  @brief		�R���X�g���N�^
	//
	//------------------------------------------------------------------------------
	DX_Debug();

#if defined(DEBUG) || defined(_DEBUG)
	//------------------------------------------------------------------------------
	//
	//  @brief		m_debug�𐶐�����
	//
	//------------------------------------------------------------------------------
	void CreateDebugDevice();
#endif


};
#endif // !__DX_DEBUG_H_
