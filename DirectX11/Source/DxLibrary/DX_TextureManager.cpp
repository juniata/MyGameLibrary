#include	"DX_Library.h"
#include	"../extern/DirectXTex/DirectXTex.h"
#include	<locale.h>

using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  static定義
//
//-----------------------------------------------------------------------------------------
DX::tagTextureInfo DX_TextureManager::m_textures[DX_TextureManager::TEXTURE_NUM];

//-----------------------------------------------------------------------------------------
//
//  変数を初期化する
//
//-----------------------------------------------------------------------------------------
void DX_TextureManager::Initialize()
{
	for (int i = 0; i < TEXTURE_NUM; ++i) {
		strcpy_s(m_textures[i].filepath, _MAX_PATH, "\0");
		m_textures[i].pSrv = nullptr;
	}
}

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
	if (pTexture) { 
		return pTexture;
	}

	//	SRVを作成する
	{
		//ロケール指定
		setlocale(LC_ALL, "japanese");

		// マルチバイト文字をワイド文字に変換する
		size_t size = 0;
		if (0 != mbstowcs_s(&size, NULL, 0, pFilepath, 0)) {
			TRACE("Failed to mbstowcs_s()\n");
			return nullptr;
		}
		wchar_t filepath[MAX_PATH] = { NULL };
		mbstowcs_s(&size, filepath, size, pFilepath, _TRUNCATE);


		TexMetadata metadata;
		ScratchImage image;
		if (DX_Debug::GetInstance()->IsFailedHresult(LoadFromWICFile(filepath, NULL, &metadata, image))) {
			TRACE("failed to LoadFromWICFile()\n");
			return nullptr;
		}


		if (DX_Debug::GetInstance()->IsFailedHresult(CreateShaderResourceView(DX_System::GetInstance()->GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &pTexture))) {
			TRACE("Failed to CreateShaderResourceView()\n");
			return nullptr;
		}
	}


	//	リストに追加する
	DX::tagTextureInfo* texInfo = nullptr;
	{
		int texCount = 0;
		for (int i = 0; i < TEXTURE_NUM; ++i) {
			if (m_textures[i].pSrv == nullptr) {
				m_textures[i].pSrv = pTexture;
				strcpy_s(m_textures[i].filepath, _MAX_PATH, pFilepath);
				texInfo = &m_textures[i];
				break;
			}
			++texCount;
		}

		if (texCount == TEXTURE_NUM) {
			TRACE("use texture num is max");
			MessageBox(NULL, "texture max", "error", MB_OK);
			SAFE_RELEASE(pTexture);
			return nullptr;
		}
	}
	
	//	テクスチャの細かな情報を取得
	{
		ID3D11Resource* pResource = nullptr;
		pTexture->GetResource(&pResource);

		ID3D11Texture2D* pTex2D = nullptr;
		if (DX_Debug::GetInstance()->IsFailedHresult(pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTex2D))) {
			TRACE("Failed to QueryInterface()\n");
			SAFE_RELEASE(pTexture);
		}
		else {
			pTex2D->GetDesc(texInfo);
		}

		SAFE_RELEASE(pResource);
		SAFE_RELEASE(pTex2D);
	}
	
	return pTexture;
}

//-----------------------------------------------------------------------------------------
//
//  テクスチャサイズを取得する
//
//-----------------------------------------------------------------------------------------
void DX_TextureManager::GetTextureSize(UINT* width, UINT* height, const ID3D11ShaderResourceView* pSrv)
{
	for (int i = 0; i < TEXTURE_NUM; ++i)
	{
		if (m_textures[i].pSrv == pSrv) {
			*width = m_textures[i].Width;
			*height = m_textures[i].Height;
			break;
		}
	}
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
			if (m_textures[i].pSrv == pTexture)
			{
				ULONG refCount = m_textures[i].pSrv->Release();
				if (refCount == 0) {
					m_textures[i].pSrv = nullptr;
				}
				break;
			}
		}
	}
}

void DX_TextureManager::AllRelease()
{
	for (int i = 0; i < TEXTURE_NUM; ++i)
	{
		SAFE_RELEASE(m_textures[i].pSrv);
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
		if (m_textures[i].filepath == NULL)
		{
			continue;
		}
		if (strcmp(m_textures[i].filepath, pFilepath) == 0)
		{
			pSrv = m_textures[i].pSrv;
			break;
		}
	}

	return pSrv;
}