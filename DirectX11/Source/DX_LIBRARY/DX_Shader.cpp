#include	"DX_Library.h"
#include <locale.h>

//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_Shader::DX_Shader()
{}


//-----------------------------------------------------------------------------------------
//
//	���̂�����Ή��
//
//-----------------------------------------------------------------------------------------
DX_Shader::~DX_Shader()
{
}

//-----------------------------------------------------------------------------------------
//
//	�R���p�C���������̃o�C�g�R�[�h���擾����
//
//-----------------------------------------------------------------------------------------
ID3DBlob* DX_Shader::GetByteCord()
{
	return m_bytecord.Get();
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

	ComPtr<ID3DBlob>	l_errorBlob;
	HRESULT		l_hr =
		D3DCompileFromFile(
		filepath,			//	�t�@�C����
		nullptr,			//	�}�N����`�Ȃ�
		nullptr,			//	�C���N���[�h�t�@�C������
		pEntryPoint,		//	�G���g���[�|�C���g
		pShaderVersion,		//	�V�F�[�_�[�o�[�W����
		l_dwShaderFlags,	//	�R���p�C���I�v�V����
		0,					//	�R���p�C���I�v�V��������
		&m_bytecord,		//	�R���p�C�����ꂽ�o�C�g�R�[�h
		&l_errorBlob		//	�G���[���b�Z�[�W
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
	DX_Debug::CheckSourceCordOfShaderFile(l_hr, l_errorBlob.Get());
}

//-----------------------------------------------------------------------------------------
//
//	���I�V�F�[�_�[��L���ɂ��郊���N���쐬
//
//-----------------------------------------------------------------------------------------
void DX_Shader::CreateClassLinkage()
{
	if (!DX_Debug::IsHresultCheck(DX_System::GetInstance()->GetDevice()->CreateClassLinkage(&m_classLinkage))){
		//SAFE_RELEASE(m_pClassLinkage);
	}
}

