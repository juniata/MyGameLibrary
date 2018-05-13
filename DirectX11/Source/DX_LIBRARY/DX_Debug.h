#ifndef __DX_DEBUG_H_
#define __DX_DEBUG_H_

//	�R���\�[����ʂɏo�͂��镶���̐F
enum class SET_PRINT_COLOR : int{
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
#define DEBUG_VALUE_CHECK(bFlag, pMessage)										\
{																				\
	if (!bFlag){																\
		char l_message[128] = { NULL };											\
		int	 l_strlen = 0;														\
																				\
		const char* l_pDirectory = strstr(__FILE__, "dx_library\\");			\
		if (l_pDirectory == nullptr){											\
			l_pDirectory = strstr(__FILE__, "myself_made_source\\");			\
			l_strlen = strlen("myself_made_source\\");							\
		}else{																	\
			l_strlen = strlen("dx_library\\");									\
		}																		\
		sprintf_s(l_message, "%s line[%d]", &l_pDirectory[l_strlen], __LINE__);	\
		MessageBox(NULL, pMessage, l_message, MB_OK);							\
		exit(EXIT_FAILURE);														\
	}																			\
}																				
#else
#define DEBUG_VALUE_CHECK(bFlag,pMessage)
#endif

//****************************************************************************************************
//
//	Debug
//
//****************************************************************************************************
class Debug
{
private:
#if defined(DEBUG) || defined(_DEBUG)
	static ComPtr<ID3D11Debug>	m_debug;
	static HANDLE	m_consoleHandle;
	static CONSOLE_SCREEN_BUFFER_INFO m_csbi;
#endif

	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	Debug() = delete;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	~Debug(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ��̏��������s��
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ��̉�����s��
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D�n�̐����I�u�W�F�N�g�𒲍�����
	//
	//------------------------------------------------------------------------------
	static void ReportLiveDeviceObjects(
		const char* pMessage
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�߂�l���`�F�b�N����
	//	@param[in]	hr	���܂��܂Ȋ֐��̖߂�l(D3D�n)
	//	@retrun		true:����	false:���s
	//
	//------------------------------------------------------------------------------
	static bool IsHresultCheck(
		HRESULT hr
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�t�@�C�����`�F�b�N
	//	@param[in]	hr			D3DX11CompileFromFile()�̖߂�l
	//	@param[in]	pBytecord	D3DX11CompileFromFile()�ŏo�����o�C�g�R�[�h
	//
	//------------------------------------------------------------------------------
	static void CheckSourceCordOfShaderFile(
		HRESULT		hr,
		ID3DBlob*	pBytecord
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�R���\�[����ʂɏo�͂��镶���̐F��ݒ�
	//	@param[in]	printColor	SET_PRINT_COLOR
	//	
	//------------------------------------------------------------------------------
	static void SetPrintColor(
		dx_library::SET_PRINT_COLOR printColor
		);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		�R���\�[����ʂɕ������o�͂���
	//	@param[in]	pFormat printf�̃��b�p�[�Ȃ̂œ����g����
	//	@note		debug���̂ݏo��
	//	
	//------------------------------------------------------------------------------
	static void Printf(
		const char* pFormat,
		...
		);

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		m_pDebug�𐶐�����
	//
	//------------------------------------------------------------------------------
	static void CreateDebugDevice();


};
#endif // !__DX_DEBUG_H_
