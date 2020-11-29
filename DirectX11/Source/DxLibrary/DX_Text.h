#pragma once
/*
	�f�o�b�O���ɉ�ʂɕ\�����邽�߂̃N���X�B

	�A�h�x���`���[�Q�[���ȂǁA�����蕶����\�����邽�߂̂��̂ł͂Ȃ��B
*/
class DX_Text : public DX_Singleton<DX_Text>
{
public:
	/// <summary>
	/// �����o�ϐ����̉���������s��
	/// </summary>
	~DX_Text();

	/// <summary>
	/// ������\������
	/// </summary>
	/// <param name="x">�J�n�`��ʒu</param>
	/// <param name="y">�J�n�`��ʒu</param>
	/// <param name="width">�`�悷��ꕶ���̃T�C�Y(����)</param>
	/// <param name="height">�`�悷��ꕶ���̃T�C�Y(����)</param>
	/// <param name="pszFormat">�t�H�[�}�b�g�Bprintf���Ɠ����g����</param>
	/// <param name="...">�ϒ������Bprintf���Ɠ����g����</param>
	void Print(const float x, const float y, const float width, const float height, LPCSTR pszFormat, ...);
private:
	/// <summary>
	/// �����o�ϐ��̏������y�уT���v���[��o�b�t�@�̍쐬
	/// </summary>
	DX_Text();
	friend class DX_Singleton<DX_Text>;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_fontSampler;
	std::map<wchar_t, DX_Font*> m_fontList;
};