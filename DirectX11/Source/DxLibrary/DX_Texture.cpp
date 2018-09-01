#include	"DX_Library.h"
#include	"../extern/DirectXTex/DirectXTex.h"
#include	<locale.h>

using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_TextureManager* DX_TextureManager::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------------------
DX_TextureManager::DX_TextureManager()
{
	for (int i = 0; i < TEXTURE_NUM; ++i)
	{
		ZeroMemory(m_textureList[i].filepath, sizeof(m_textureList[i].filepath));
		m_textureList[i].pSrv = nullptr;
	}
}

//-----------------------------------------------------------------------------------------
//
//	���
//
//-----------------------------------------------------------------------------------------
DX_TextureManager::~DX_TextureManager()
{
	// �{�������ł͉������Ȃ��B(Release(ID3D11ShaderResourceView*)�ł��ׂĊJ������邽�߂ł���B)
	for (int i = 0; i < TEXTURE_NUM; ++i)
	{
		if (m_textureList[i].pSrv) {
			SAFE_RELEASE(m_textureList[i].pSrv);
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���擾����
//
//-----------------------------------------------------------------------------------------
DX_TextureManager* DX_TextureManager::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_TextureManager();
	}

	return m_pInstance;
}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���������
//
//-----------------------------------------------------------------------------------------
void DX_TextureManager::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`�����擾����
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView* DX_TextureManager::GetTexture(const char* pFilepath)
{
	//	����΃|�C���^�A�Ȃ����nullptr���Ԃ�
	ID3D11ShaderResourceView* pTexture = SearchTexture(pFilepath);
	
	//	�������̂ŁA�e�N�X�`���̃|�C���^��Ԃ�
	if (pTexture){ return pTexture; }
	
	//���P�[���w��
	setlocale(LC_ALL, "japanese");

	std::string filepathStr(pFilepath);

	size_t size;
	if (0 != mbstowcs_s(&size, NULL, 0, filepathStr.c_str(), 0)) {
		return nullptr;
	}
	wchar_t filepath[MAX_PATH] = { NULL };
	mbstowcs_s(&size, filepath, size, pFilepath, _TRUNCATE);

	TexMetadata metadata;
	ScratchImage image;
	HRESULT hr = LoadFromWICFile(filepath, NULL, &metadata, image);

	//	�߂�l���`�F�b�N
	if (!DX_Debug::GetInstance()->IsHresultCheck(hr)) {
		//	�G���[�������ꍇ�Anullptr��Ԃ�
		return nullptr;
	}

	hr = CreateShaderResourceView(DX_System::GetInstance()->GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &pTexture);
	//	�߂�l���`�F�b�N
	if (!DX_Debug::GetInstance()->IsHresultCheck(hr)) {
		//	�G���[�������ꍇ�Anullptr��Ԃ�
		return nullptr;
	}

	//	���X�g�ɒǉ�����
	int texCount = 0;
	for (int i = 0; i < TEXTURE_NUM; ++i) {
		if (m_textureList[i].pSrv == nullptr) {
			m_textureList[i].pSrv = pTexture;
			strcpy_s(m_textureList[i].filepath, _MAX_PATH, pFilepath);
			break;
		}
		++texCount;
	}

	if (texCount == TEXTURE_NUM) {
		MessageBox(NULL, "texture max", "error", MB_OK);
		SAFE_RELEASE(pTexture);
		return nullptr;
	}

	return pTexture;
}


//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`���������������
//
//-----------------------------------------------------------------------------------------
void DX_TextureManager::Release(ID3D11ShaderResourceView* pTexture)
{
	if (pTexture)
	{
		for (int i = 0; i < TEXTURE_NUM; ++i)
		{
			if (m_textureList[i].pSrv == pTexture)
			{
				ULONG refCount = m_textureList[i].pSrv->Release();
				if (refCount == 0) {
					m_textureList[i].pSrv = nullptr;
				}
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------------------
//
//  �e�N�X�`�����g�p���Ă��邩�𒲂ׂ�
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView* DX_TextureManager::SearchTexture(const char* pFilepath)
{
	ID3D11ShaderResourceView* pSrv = nullptr;

	for (int i = 0; i < TEXTURE_NUM; ++i)
	{
		if (m_textureList[i].filepath == NULL)
		{
			continue;
		}
		if (strcmp(m_textureList[i].filepath, pFilepath) == 0)
		{
			pSrv = m_textureList[i].pSrv;
			break;
		}
	}

	return pSrv;
}