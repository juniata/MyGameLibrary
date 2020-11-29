#ifndef __DX_PIXEL_SHADER_H_
#define __DX_PIXEL_SHADER_H_

/// <summary>
/// �s�N�Z���V�F�[�_�[
/// </summary>
class DX_PixelShader : public DX_Shader
{
public:
	/// <summary>
	/// �����o�ϐ���������
	/// </summary>
	DX_PixelShader();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DX_PixelShader();

	/// �V�F�[�_�[�𗘗p����
	/// </summary>
	/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
	void Begin(const unsigned int classInstanceCount = 0);

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	/// <summary>
	/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
	/// </summary>
	/// <returns>����</returns>
	bool CreateShaderObject();
};
#endif // !__DX_PIXEL_SHADER_H_
