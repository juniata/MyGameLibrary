#ifndef __DX_COMPUTE_SHADER_H_
#define __DX_COMPUTE_SHADER_H_


class DX_ComputeShader : public DX_Shader
{
public:
	/// <summary>
	/// �����o�ϐ���������
	/// </summary>
	DX_ComputeShader();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DX_ComputeShader();


	/// �V�F�[�_�[�𗘗p����
	/// </summary>
	/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
	void Begin(const unsigned int classInstanceCount = 0);

private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_conmputeShader;


	/// <summary>
	/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
	/// </summary>
	/// <returns>����</returns>
	bool CreateShaderObject();
};
#endif // !__DX_COMPUTE_SHADER_H_
