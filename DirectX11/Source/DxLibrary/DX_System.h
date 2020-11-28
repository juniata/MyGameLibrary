#ifndef __DX_SYSTEM_H_
#define __DX_SYSTEM_H_
	
/*
	DirectX�f�o�C�X�����Ǘ�����N���X�ł��B
*/
class DX_System : public DX_Singleton<DX_System>
{
public:
	/// <summary>
	/// DirectX�̃f�o�C�X��������������
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�̃C���X�^���X�n���h��</param>
	/// <param name="hInst">�E�B���h�E�̃C���X�^���X</param>
	/// <param name="width">�X�N���[���̕�</param>
	/// <param name="height">�X�N���[���̍���</param>
	/// <returns>����</returns>
	bool Initialize(HWND hWnd, HINSTANCE hInst, const UINT width, const UINT height);

	/// <summary>
	/// �X�N���[���̍������擾����
	/// </summary>
	/// <returns>�X�N���[���̍���</returns>
	UINT GetScreenHeight();

	/// <summary>
	/// �X�N���[���̕����擾����
	/// </summary>
	/// <returns>�X�N���[���̕�</returns>
	UINT GetScreenWidth();
	
	/// <summary>
	/// DirectX�̃f�o�C�X���擾����
	/// </summary>
	/// <returns>�f�o�C�X</returns>
	ID3D11Device*	GetDevice();

	/// <summary>
	/// DIrectX�̃f�o�C�X�R���e�L�X�g���擾����
	/// </summary>
	/// <returns>�f�o�C�X�R���e�L�X�g</returns>
	ID3D11DeviceContext*	GetDeviceContext();

	/// <summary>
	/// �X���b�v�`�F�C�����擾����
	/// </summary>
	/// <returns>�X���b�v�`�F�C��</returns>
	IDXGISwapChain*	GetSwapChain();

	/// <summary>
	/// FeatureLevel���擾����
	/// </summary>
	/// <returns>FeatureLevel</returns>
	D3D_FEATURE_LEVEL GetFeatureLevel();

	/// <summary>
	/// DriverType���擾����
	/// </summary>
	/// <returns>DriverType</returns>
	D3D_DRIVER_TYPE GetDriverType();

	/// <summary>
	/// �����_�[�^�[�Q�b�g���擾����
	/// </summary>
	/// <returns>�����_�[�^�[�Q�b�g</returns>
	ID3D11RenderTargetView*  GetRenderTargetView();

	/// <summary>
	/// �[�x�E�X�e���V���r���[���擾����
	/// </summary>
	/// <returns>�[�x�E�X�e���V���r���[</returns>
	ID3D11DepthStencilView* 	GetDepthStencilView();

	/// <summary>
	/// �[�x�E�X�e���V���o�b�t�@���擾����
	/// </summary>
	/// <returns>�[�x�E�X�e���V���o�b�t�@</returns>
	ID3D11Texture2D*		GetDepthStencilBuffer();

	/// <summary>
	/// �E�B���h�E�̃C���X�^���X�n���h�����擾����
	/// </summary>
	/// <returns>�E�B���h�E�̃C���X�^���X�n���h��</returns>
	HWND	GetWindowHandle();

	/// <summary>
	/// �o�b�N�o�b�t�@������������
	/// </summary>
	/// <returns>����</returns>
	bool InitBuckBuffer();

	/// <summary>
	/// ��ʕύX�ɔ����o�b�t�@�̃��T�C�Y���s��
	/// </summary>
	/// <param name="width">�X�N���[���̕�</param>
	/// <param name="height">�X�N���[���̍���</param>
	/// <returns>����</returns>
	bool BufferResize(const WORD width, const WORD height);
private:
	friend class DX_Singleton<DX_System>;

	Microsoft::WRL::ComPtr<ID3D11Device>			m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_rtv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_dsv;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_dsb;
	D3D_FEATURE_LEVEL		m_featureLevel;
	D3D_DRIVER_TYPE			m_driverType;
	HWND					m_windowHandle;

	UINT m_screenWidth;
	UINT m_screenHeight;

	/// <summary>
	/// �����o�ϐ�������������
	/// </summary>
	DX_System();

	/// <summary>
	/// DX�ɂ�����邷�ׂẴf�o�C�X���̉�����s��
	/// </summary>
	~DX_System();

	/// <summary>
	/// DirectX�̃f�o�C�X�y�уf�o�C�X�R���e�L�X�g���тɃX���b�v�`�F�C�����쐬����
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�̃C���X�^���X�n���h��</param>
	/// <returns>����</returns>
	bool CreateDeviceAndSwapChain(const HWND hWnd);

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[���쐬����
	/// </summary>
	/// <returns>����</returns>
	bool CreateRenderTargetView();

	/// <summary>
	/// �[�x�E�X�e���V���o�b�t�@���쐬����
	/// </summary>
	/// <returns>����<</returns>
	bool CreateDepthStencilBuffer();

	/// <summary>
	/// �[�x�E�X�e���V���r���[���쐬
	/// </summary>
	/// <returns>����</returns>
	bool CreateDepthStencilView();
};
#endif // !__DX_SYSTEM_H_
