#pragma once

/*
	�t�H���g(�ꕶ��)���Ǘ�
*/
class DX_Font
{
public:
	/// <summary>
	/// �����o�ϐ���������
	/// </summary>
	DX_Font(const wchar_t msg);

	/// <summary>
	/// �����o�ϐ��̉��
	/// </summary>
	~DX_Font();

	/// <summary>
	/// �t�H���g�̏���ݒ肷��
	/// </summary>
	/// <param name="rect">�`�悷��͈�</param>
	void SetInfo(const DX::tagRect& rect);

	/// <summary>
	/// �`�悷��
	/// </summary>
	/// <param name="vertexBuffer">�X�V���钸�_�o�b�t�@</param>
	/// <param name="samplerState">�g�p�����T���v���[�X�e�[�g</param>
	void Render(ID3D11Buffer** vertexBuffer, ID3D11SamplerState* samplerState);
private:
	ID3D11ShaderResourceView* m_fontSrv;
	DX::tagRect m_rect;
};