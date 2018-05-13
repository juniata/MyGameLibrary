#include	"DX_Library.h"
#include "../DirectXTex/DirectXTex.h"
#include	<locale.h>

using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
std::map<const char*, ID3D11ShaderResourceView*> DX_TextureManager::m_textureList;

//-----------------------------------------------------------------------------------------
//
//  テクスチャを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView* DX_TextureManager::GetTexture(
	const char* pFilepath
	)
{
	//	あればポインタ、なければnullptrが返る
	ID3D11ShaderResourceView* l_pTexture = SearchTexture(pFilepath);
	
	//	見つけたので、テクスチャのポインタを返す
	if (l_pTexture){ return l_pTexture; }
	
	//ロケール指定
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

	//	戻り値をチェック
	if (!DX_Debug::IsHresultCheck(l_hr)) {
		//	エラーだった場合、nullptrを返す
		return nullptr;
	}

	l_hr = CreateShaderResourceView(DX_System::GetInstance()->GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &l_pTexture);
	//	戻り値をチェック
	if (!DX_Debug::IsHresultCheck(l_hr)) {
		//	エラーだった場合、nullptrを返す
		return nullptr;
	}
	//	リストに追加する
	m_textureList[pFilepath] = l_pTexture;

	return l_pTexture;
}


//-----------------------------------------------------------------------------------------
//
//  テクスチャを情報を解放する
//
//-----------------------------------------------------------------------------------------
void DX_TextureManager::Release(
	ID3D11ShaderResourceView* pTexture
	)
{
	for (auto itr = m_textureList.begin(); itr != m_textureList.end(); ++itr){
		//	同じ情報が見つかった場合
		if (itr->second == pTexture){
			//	テクスチャを解放
			if (0 >= itr->second->Release()) {
				itr->second = nullptr;

				//	自身をリストから外す
				m_textureList.erase(itr);
			}
			break;
		}
	}
}


//-----------------------------------------------------------------------------------------
//
//  テクスチャを使用しているかを調べる
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

	//	無かったので、nullptrを返す
	return nullptr;
}