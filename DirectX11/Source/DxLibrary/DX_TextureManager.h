#ifndef __DX_TEXTURE_H_
#define __DX_TEXTURE_H_

#include <map>

/// <summary>
/// �e�N�X�`�����Ǘ����Ă��܂��B
/// </summary>
class DX_TextureManager
{
public:
	/// <summary>
	/// �e�N�X�`�����擾����(�w�肵���e�N�X�`�����Ȃ���΍쐬����)
	/// </summary>
	/// <param name="filepath">�e�N�X�`���̃t�@�C���p�X</param>
	/// <returns>�e�N�X�`��(���݂��Ȃ��e�N�X�`���̍쐬�Ɏ��s���Ă���ꍇ��nullptr)</returns>
	static ID3D11ShaderResourceView* GetTexture(const char* filepath);
	
	/// <summary>
	/// �w�肵���e�N�X�`���̃T�C�Y���擾����
	/// </summary>
	/// <param name="width">�e�N�X�`���T�C�Y(��)���i�[�����</param>
	/// <param name="height">�e�N�X�`���T�C�Y(����)���i�[�����</param>
	/// <param name="srv">�T�C�Y�̗~�����e�N�X�`��</param>
	static void GetTextureSize(UINT* width, UINT* height, const ID3D11ShaderResourceView* srv);

	/// <summary>
	/// �w�肵���e�N�X�`�����������
	/// </summary>
	/// <param name="texture">����������e�N�X�`��</param>
	static void Release(ID3D11ShaderResourceView* texture);

	/// <summary>
	/// �w�肵�������̃e�N�X�`�����擾
	/// </summary>
	/// <param name="text">����</param>
	/// <returns>�w�肵�������̃e�N�X�`�����擾(�擾�ł��Ȃ������ꍇ��nullptr)</returns>
	static ID3D11ShaderResourceView* GetFontTexture(const wchar_t* text);
private:
	static std::map<const char*, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

	/// <summary>
	/// �w�肵���t�@�C���p�X�̃e�N�X�`�������X�g����T���Ď擾����
	/// </summary>
	/// <param name="filepath">�T���e�N�X�`���̃t�@�C���p�X</param>
	/// <returns>����΃e�N�X�`���A�Ȃ����nullptr</returns>
	static ID3D11ShaderResourceView* SearchTexture(const char* filepath);

	/// <summary>
	/// �w�肵���t�@�C���p�X����e�N�X�`�����쐬����
	/// </summary>
	/// <param name="filepath">�쐬����e�N�X�`���̃t�@�C���p�X</param>
	/// <returns>�쐬�ł����ꍇ�e�N�X�`���A���s�����ꍇnullptr</returns>
	static ID3D11ShaderResourceView*  CreateTexture(const char* pFilepath);
};
#endif // !__DX_TEXTURE_H_
