#ifndef __DX_TEXTURE_H_
#define __DX_TEXTURE_H_

#include <map>

/// <summary>
/// テクスチャを管理しています。
/// </summary>
class DX_TextureManager
{
public:
	/// <summary>
	/// テクスチャを取得する(指定したテクスチャがなければ作成する)
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャ(存在しないテクスチャの作成に失敗している場合はnullptr)</returns>
	static ID3D11ShaderResourceView* GetTexture(const char* filepath);
	
	/// <summary>
	/// 指定したテクスチャのサイズを取得する
	/// </summary>
	/// <param name="width">テクスチャサイズ(幅)が格納される</param>
	/// <param name="height">テクスチャサイズ(高さ)が格納される</param>
	/// <param name="srv">サイズの欲しいテクスチャ</param>
	static void GetTextureSize(UINT* width, UINT* height, const ID3D11ShaderResourceView* srv);

	/// <summary>
	/// 指定したテクスチャを解放する
	/// </summary>
	/// <param name="texture">解放したいテクスチャ</param>
	static void Release(ID3D11ShaderResourceView* texture);

	/// <summary>
	/// 指定した文字のテクスチャを取得
	/// </summary>
	/// <param name="text">文字</param>
	/// <returns>指定した文字のテクスチャを取得(取得できなかった場合はnullptr)</returns>
	static ID3D11ShaderResourceView* GetFontTexture(const wchar_t* text);
private:
	static std::map<const char*, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

	/// <summary>
	/// 指定したファイルパスのテクスチャをリストから探して取得する
	/// </summary>
	/// <param name="filepath">探すテクスチャのファイルパス</param>
	/// <returns>あればテクスチャ、なければnullptr</returns>
	static ID3D11ShaderResourceView* SearchTexture(const char* filepath);

	/// <summary>
	/// 指定したファイルパスからテクスチャを作成する
	/// </summary>
	/// <param name="filepath">作成するテクスチャのファイルパス</param>
	/// <returns>作成できた場合テクスチャ、失敗した場合nullptr</returns>
	static ID3D11ShaderResourceView*  CreateTexture(const char* pFilepath);
};
#endif // !__DX_TEXTURE_H_
