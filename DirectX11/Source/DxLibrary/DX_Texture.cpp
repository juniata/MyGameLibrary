#include	"DX_Library.h"
#include	"../extern/DirectXTex/DirectXTex.h"
#include	<locale.h>

using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_TextureManager::TextureInfo DX_TextureManager::m_textureList[TEXTURE_NUM] = { NULL };

//-----------------------------------------------------------------------------------------
//
//  テクスチャを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView* DX_TextureManager::GetTexture(const char* pFilepath)
{
	//	あればポインタ、なければnullptrが返る
	ID3D11ShaderResourceView* pTexture = SearchTexture(pFilepath);
	
	//	見つけたので、テクスチャのポインタを返す
	if (pTexture){ return pTexture; }
	
	//ロケール指定
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

	//	戻り値をチェック
	if (!DX_Debug::GetInstance()->IsHresultCheck(hr)) {
		//	エラーだった場合、nullptrを返す
		return nullptr;
	}

	hr = CreateShaderResourceView(DX_System::GetInstance()->GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &pTexture);
	//	戻り値をチェック
	if (!DX_Debug::GetInstance()->IsHresultCheck(hr)) {
		//	エラーだった場合、nullptrを返す
		return nullptr;
	}

	//	リストに追加する
	int texCount = 0;
	for (int i = 0; i < TEXTURE_NUM; ++i) {
		if (m_textureList[i].pSrv == nullptr) {
			m_textureList[i].pSrv = pTexture;
			m_textureList[i].pFilepath = pFilepath;
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
//  テクスチャを情報を解放する
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
//  テクスチャを使用しているかを調べる
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView* DX_TextureManager::SearchTexture(const char* pFilepath)
{
	ID3D11ShaderResourceView* pSrv = nullptr;

	for (int i = 0; i < TEXTURE_NUM; ++i)
	{
		if (m_textureList[i].pFilepath == NULL)
		{
			continue;
		}
		if (strcmp(m_textureList[i].pFilepath, pFilepath) == 0)
		{
			pSrv = m_textureList[i].pSrv;
			break;
		}
	}

	return pSrv;
}