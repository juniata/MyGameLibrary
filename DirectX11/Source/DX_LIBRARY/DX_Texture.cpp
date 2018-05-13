#include	"DX_Library.h"
#include "../DirectXTex/DirectXTex.h"
#include	<locale.h>

using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
std::map<const char*, ID3D11ShaderResourceView*> DX_TextureManager::m_textureList;

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`�����擾����
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView* DX_TextureManager::GetTexture(
	const char* pFilepath
	)
{
	//	����΃|�C���^�A�Ȃ����nullptr���Ԃ�
	ID3D11ShaderResourceView* l_pTexture = SearchTexture(pFilepath);
	
	//	�������̂ŁA�e�N�X�`���̃|�C���^��Ԃ�
	if (l_pTexture){ return l_pTexture; }
	
	//���P�[���w��
	setlocale(LC_ALL, "japanese");

	std::string l_filepath(pFilepath);

	size_t l_size;
	if (0 != mbstowcs_s(&l_size, NULL, 0, l_filepath.c_str(), 0)) {
		return nullptr;
	}
	wchar_t filepath[MAX_PATH] = { NULL };
	mbstowcs_s(&l_size, filepath, l_size, pFilepath, _TRUNCATE);

	TexMetadata metadata;
	ScratchImage image;
	HRESULT l_hr = LoadFromWICFile(filepath, NULL, &metadata, image);

	//	�߂�l���`�F�b�N
	if (!DX_Debug::IsHresultCheck(l_hr)) {
		//	�G���[�������ꍇ�Anullptr��Ԃ�
		return nullptr;
	}

	l_hr = CreateShaderResourceView(DX_System::GetInstance()->GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &l_pTexture);
	//	�߂�l���`�F�b�N
	if (!DX_Debug::IsHresultCheck(l_hr)) {
		//	�G���[�������ꍇ�Anullptr��Ԃ�
		return nullptr;
	}
	//	���X�g�ɒǉ�����
	m_textureList[pFilepath] = l_pTexture;

	return l_pTexture;
}


//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`���������������
//
//-----------------------------------------------------------------------------------------
void DX_TextureManager::Release(
	ID3D11ShaderResourceView* pTexture
	)
{
	for (auto itr = m_textureList.begin(); itr != m_textureList.end(); ++itr){
		//	������񂪌��������ꍇ
		if (itr->second == pTexture){
			//	�e�N�X�`�������
			if (0 >= itr->second->Release()) {
				itr->second = nullptr;

				//	���g�����X�g����O��
				m_textureList.erase(itr);
			}
			break;
		}
	}
}


//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`�����g�p���Ă��邩�𒲂ׂ�
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView* DX_TextureManager::SearchTexture(
	const char* pFilepath
	)
{
	if (m_textureList.count(pFilepath)) {
		m_textureList[pFilepath]->AddRef();
		return m_textureList[pFilepath];
	}

	//	���������̂ŁAnullptr��Ԃ�
	return nullptr;
}