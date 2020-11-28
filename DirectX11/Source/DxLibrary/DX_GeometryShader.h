#ifndef __DX_GEOMETRY_SHADER_H_
#define __DX_GEOMETRY_SHADER_H_

/// <summary>
/// �W�I���g���V�F�[�_�[
/// </summary>
class DX_GeometryShader : public DX_Shader
{
public:
	/// <summary>
	/// �����o�ϐ���������
	/// </summary>
	DX_GeometryShader();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DX_GeometryShader();

	/// �V�F�[�_�[�𗘗p����
	/// </summary>
	/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
	void Begin(const unsigned int classInstanceCount = 0);


	/// <summary>
	/// �W�I���g���V�F�[�_�[����o�͂����f�[�^���擾
	/// </summary>
	/// <param name="decreation">�W�I���g���Ɠ����Z�}���e�B�N�X�̂��̂Ƀf�[�^���o��</param>
	/// <param name="decreationElementCount">decreation�̗v�f��</param>
	/// <param name="bufferStrides">�f�[�^�𕪊����������</param>
	/// <param name="stridesElementCount">bufferStrides�̗v�f��</param>
	void CreateGeometryShaderWithStreamOutput(D3D11_SO_DECLARATION_ENTRY decreation[], const UINT decreationElementCount, unsigned int* bufferStrides, const UINT stridesElementCount);
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_outputGeometryShader;


	/// <summary>
	/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
	/// </summary>
	/// <returns>����</returns>
	bool CreateShaderObject();
};
#endif // !__DX_GEOMETRY_SHADER_H_
