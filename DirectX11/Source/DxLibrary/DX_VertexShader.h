#ifndef __DX_VERTEX_SHADER_H_
#define __DX_VERTEX_SHADER_H_

/// <summary>
/// ���_�V�F�[�_�[
/// </summary>
class DX_VertexShader : public DX_Shader
{
public:
	/// <summary>
	/// �����o�ϐ���������
	/// </summary>
	DX_VertexShader();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DX_VertexShader();

	/// <summary>
	/// �V�F�[�_�[�𗘗p����
	/// </summary>
	/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
	void Begin(const unsigned int classInstanceCount = 0);

	/// <summary>
	/// �V�F�[�_�[�̗��p���I����
	/// </summary>
	void End();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	/// <summary>
	/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
	/// </summary>
	/// <returns>����</returns>
	bool  CreateShaderObject();
};
#endif // !__DX_VERTEX_SHADER_H_
