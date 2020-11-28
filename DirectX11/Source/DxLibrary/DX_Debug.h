#ifndef __DX_DEBUG_H_
#define __DX_DEBUG_H_

//	�R���\�[����ʂɏo�͂��镶���̐F
enum class SET_PRINT_COLOR : int {
	BLUE,		//	�F
	RED,		//	�ԐF
	GREEN,		//	��	
	DEFAULT,	//	�����̐F
};


/// <summary>
/// Debug�R���\�[���ɕ������o�͂���B
/// </summary>
#if defined(DEBUG) || defined(_DEBUG)
	void __Trace(const char* file, int line, LPCSTR pszFormat, ...);
	#define TRACE(...)			__Trace(__FILE__, __LINE__, __VA_ARGS__);
#else
	#define TRACE(...)
#endif

struct IDXGIDebug;

/// <summary>
/// IDXGIDebug���Ǘ�
/// </summary>
class DX_Debug : public DX_Singleton<DX_Debug>
{
public:
	/// <summary>
	/// �f�o�C�X�̐��������s��
	/// </summary>
	void Initialize();

	/// <summary>
	/// ID3D�n�̐����I�u�W�F�N�g�𒲍�����
	/// </summary>
	/// <param name="message">�f�o�b�O�R���\�[���ɕ\��������e</param>
	void ReportLiveDeviceObjects(const char* message);

	/// <summary>
	/// HRESULT�^���`�F�b�N����
	/// </summary>
	/// <param name="hr">HRESULT�^�̖߂�l</param>
	/// <returns>����</returns>
	bool CheckHresult(HRESULT hr);

	/// <summary>
	/// HRESULT�^���`�F�b�N����
	/// </summary>
	/// <param name="hr">HRESULT�^�̖߂�l</param>
	/// <returns>����</returns>
	bool IsFailedHresult(HRESULT hr);

private:
	friend class DX_Singleton<DX_Debug>;

	/// <summary>
	/// �����o�ϐ��̏��������s���B
	/// </summary>
	DX_Debug();


#if defined(DEBUG) || defined(_DEBUG)
	Microsoft::WRL::ComPtr<IDXGIDebug> m_debug;
	HANDLE	m_consoleHandle;
	CONSOLE_SCREEN_BUFFER_INFO m_csbi;
	
	/// <summary>
	/// IDXGIDebug�𐶐�����
	/// </summary>
	void CreateDebugDevice();
#endif


};
#endif // !__DX_DEBUG_H_
