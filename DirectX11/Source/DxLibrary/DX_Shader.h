#ifndef __DX_SHADER_H_
#define __DX_SHADER_H_

enum class SHADER_TYPE {
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER,
};

//****************************************************************************************************
//
//	Shader
//
//****************************************************************************************************
class DX_Shader
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DX_Shader();

	/// <summary>
	/// �����t���R���X�g���N�^
	/// </summary>
	/// <param name="type">�V�F�[�_�[�^�C�v</param>
	DX_Shader(SHADER_TYPE type);

	/// <summary>
	///	���z�f�X�g���N�^
	/// </summary>
	virtual ~DX_Shader();

	/// <summary>
	/// �V�F�[�_�[���쐬����
	/// </summary>
	/// <param name="filepath">�V�F�[�_�[�̃t�@�C���p�X</param>
	/// <returns>����</returns>
	bool CreateShader(const char* filepath);

	/// <summary>
	/// �V�F�[�_�[�𗘗p����
	/// </summary>
	/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
	virtual void Begin(const unsigned int classInstanceCount = 0) = 0;

	/// <summary>
	/// �V�F�[�_�[�̗��p���I����
	/// </summary>
	void End();

	/// <summary>
	/// �V�F�[�_�[�t�@�C�����R���p�C���������̃o�C�g�R�[�h���擾
	/// </summary>
	/// <returns>�o�C�g�R�[�h</returns>
	ID3DBlob* GetByteCord();

protected:
	Microsoft::WRL::ComPtr<ID3D11ClassLinkage>	m_classLinkage;
	Microsoft::WRL::ComPtr<ID3D11ClassInstance>	m_classInstance;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
	Microsoft::WRL::ComPtr<ID3DBlob>			m_bytecord;

	SHADER_TYPE m_shaderType;

	/// <summary>
	/// �V�F�[�_�[�t�@�C�����R���p�C������
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X</param>
	/// <returns>����</returns>
	bool Compile(const char* filepath);

	/// <summary>
	/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
	/// </summary>
	/// <returns>����</returns>
	virtual bool CreateShaderObject() = 0;

	/// <summary>
	/// ���I�V�F�[�_�[��L���ɂ��郊���N���쐬
	/// </summary>
	/// <returns>����</returns>
	bool CreateClassLinkage();

private:
	//	�e�V�F�[�_�[�̃G���g���[�|�C���g
	static const char* VS_ENTRY_POINT;
	static const char* HS_ENTRY_POINT;
	static const char* DS_ENTRY_POINT;
	static const char* GS_ENTRY_POINT;
	static const char* PS_ENTRY_POINT;
	static const char* CS_ENTRY_POINT;

	//	�e�V�F�[�_�[�̃o�[�W����
	static const char* VS_VERSION;
	static const char* HS_VERSION;
	static const char* DS_VERSION;
	static const char* GS_VERSION;
	static const char* PS_VERSION;
	static const char* CS_VERSION;
};

//	�e�V�F�[�_�[�w�b�_�[���C���N���[�h
#include	"DX_VertexShader.h"
#include	"DX_GeometryShader.h"
#include	"DX_PixelShader.h"
#include	"DX_ComputeShader.h"
#endif // !__DX_SHADER_H_
