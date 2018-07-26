#include	"DX_Library.h"
#include <locale.h>

//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_Shader::DX_Shader() :
	m_pBytecord(nullptr),
	m_pClassInstance(nullptr),
	m_pClassLinkage(nullptr),
	m_pInputLayout(nullptr)
{}


//-----------------------------------------------------------------------------------------
//
//	���̂�����Ή��
//
//-----------------------------------------------------------------------------------------
DX_Shader::~DX_Shader()
{
	SAFE_RELEASE(m_pBytecord);
	SAFE_RELEASE(m_pClassInstance);
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pClassLinkage);
}

//-----------------------------------------------------------------------------------------
//
//	�R���p�C���������̃o�C�g�R�[�h���擾����
//
//-----------------------------------------------------------------------------------------
ID3DBlob* DX_Shader::GetByteCord()
{
	return m_pBytecord;
}

//-----------------------------------------------------------------------------------------
//
//	�V�F�[�_�[�t�@�C�����R���p�C������
//
//-----------------------------------------------------------------------------------------
void DX_Shader::CompileFromFile(
	const char* pFilepath,
	const char* pEntryPoint,
	const char* pShaderVersion
	)
{
	
	//	��D��V�F�[�_�[
#if defined(DEBUG) || defined(_DEBUG)
	DWORD l_dwShaderFlags =
		D3D10_SHADER_ENABLE_STRICTNESS | 
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR |
		D3D10_SHADER_DEBUG;
#else
	DWORD l_dwShaderFlags =
		D3D10_SHADER_ENABLE_STRICTNESS | 
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#endif
	
	//���P�[���w��
	setlocale(LC_ALL, "japanese");

	size_t strlen;
	wchar_t filepath[MAX_PATH];
	mbstowcs_s(&strlen, filepath, strnlen_s(pFilepath,MAX_PATH)+1, pFilepath, _TRUNCATE
	);

	ID3DBlob*	l_pErrorBlob = nullptr;
	HRESULT		l_hr =
		D3DCompileFromFile(
		filepath,			//	�t�@�C����
		nullptr,			//	�}�N����`�Ȃ�
		nullptr,			//	�C���N���[�h�t�@�C������
		pEntryPoint,		//	�G���g���[�|�C���g
		pShaderVersion,		//	�V�F�[�_�[�o�[�W����
		l_dwShaderFlags,	//	�R���p�C���I�v�V����
		0,					//	�R���p�C���I�v�V��������
		&m_pBytecord,		//	�R���p�C�����ꂽ�o�C�g�R�[�h
		&l_pErrorBlob		//	�G���[���b�Z�[�W
	);
	//l_hr = D3DX11CompileFromFile(
	//	pFilepath,			//	�t�@�C����
	//	nullptr,			//	�}�N����`�Ȃ�
	//	nullptr,			//	�C���N���[�h�t�@�C������
	//	pEntryPoint,		//	�G���g���[�|�C���g
	//	pShaderVersion,		//	�V�F�[�_�[�o�[�W����
	//	l_dwShaderFlags,	//	�R���p�C���I�v�V����
	//	0,					//	�R���p�C���I�v�V��������
	//	nullptr,			//	�����ɃR���p�C�����Ă���֐��𔲂���
	//	&m_pBytecord,		//	�R���p�C�����ꂽ�o�C�g�R�[�h
	//	&l_pErrorBlob,		//	�G���[���b�Z�[�W
	//	nullptr				//	�߂�l
	//	);

	//	�V�F�[�_�[�t�@�C���̃\�[�X�R�[�h���`�F�b�N
	DX_Debug::GetInstance()->CheckSourceCordOfShaderFile(l_hr, l_pErrorBlob);
	SAFE_RELEASE(l_pErrorBlob);
}

//-----------------------------------------------------------------------------------------
//
//	���I�V�F�[�_�[��L���ɂ��郊���N���쐬
//
//-----------------------------------------------------------------------------------------
void DX_Shader::CreateClassLinkage()
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(DX_System::GetInstance()->GetDevice()->CreateClassLinkage(&m_pClassLinkage))){
		SAFE_RELEASE(m_pClassLinkage);
	}
}
