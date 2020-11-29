#include	"DX_Library.h"
#include	"../extern/DirectXTex/DirectXTex.h"
#include	<locale.h>
#include	<tchar.h>
using namespace DirectX;


/// <summary>
/// マップの解放
/// </summary>
DX_TextureManager::~DX_TextureManager()
{
	m_textures.clear();
}

/// <summary>
/// テクスチャを取得する(指定したテクスチャがなければ作成する)
/// </summary>
/// <param name="filepath">テクスチャのファイルパス</param>
/// <returns>テクスチャ(存在しないテクスチャの作成に失敗している場合はnullptr)</returns>
ID3D11ShaderResourceView* DX_TextureManager::GetTexture(const char* filepath)
{
	ID3D11ShaderResourceView* texture = SearchTexture(filepath);

	if (texture)
	{
		texture->AddRef();
	}
	else
	{
		texture = CreateTexture(filepath);
	}

	return texture;
}

/// <summary>
/// 指定したテクスチャのサイズを取得する
/// </summary>
/// <param name="width">テクスチャサイズ(幅)が格納される</param>
/// <param name="height">テクスチャサイズ(高さ)が格納される</param>
/// <param name="srv">サイズの欲しいテクスチャ</param>
void DX_TextureManager::GetTextureSize(UINT* width, UINT* height, const ID3D11ShaderResourceView* srv)
{
	for (auto texture : m_textures) 
	{
		if (texture.second.Get() == srv)
		{
			//	テクスチャの細かな情報を取得
			Microsoft::WRL::ComPtr<ID3D11Resource>	resource;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2d;

			texture.second->GetResource(&resource);

			if (DX_Debug::GetInstance()->IsFailedHresult(resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)tex2d.GetAddressOf())))
			{
				TRACE("Failed to QueryInterface()");
			}
			else
			{
				D3D11_TEXTURE2D_DESC desc;
				tex2d->GetDesc(&desc);
				*height = desc.Height;
				*width	= desc.Width;
			}
			break;
		}
	}
}


/// <summary>
/// 指定したテクスチャを解放する
/// </summary>
/// <param name="texture">解放したいテクスチャ</param>
void DX_TextureManager::Release(ID3D11ShaderResourceView* texture)
{
	if (texture)
	{
		for (auto tex : m_textures)
		{
			if (tex.second.Get() == texture)
			{
				tex.second->Release();
				break;
			}
		}
	}
}

/// <summary>
/// 指定した文字のテクスチャを取得
/// </summary>
/// <param name="text">文字</param>
/// <returns>指定した文字のテクスチャを取得(取得できなかった場合はnullptr)</returns>
ID3D11ShaderResourceView* DX_TextureManager::GetFontTexture(const wchar_t* text)
{
	size_t ret = 0;
	char filepath[_MAX_PATH] = { '\0' };
	wcstombs_s(&ret, filepath, _MAX_PATH, text, wcslen(text));
	
	ID3D11ShaderResourceView* fontSrv = SearchTexture(filepath);

	do
	{
		if (fontSrv)
		{
			fontSrv->AddRef();
			break;
		}

		// フォントの作成
		int fontSize = 100;
		int fontWeight = 0;
		LOGFONTW lf =
		{
			fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
			SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN,
			_T(L"ＭＳ Ｐ明朝")
		};
		HFONT hFont = CreateFontIndirectW(&lf);
		if (hFont == NULL)
		{
			TRACE("failed to CreateFontIndirectW()")
			break;
		}

		// デバイスコンテキスト取得
		// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
		HDC		hdc = GetDC(NULL);
		HFONT	oldFont = (HFONT)SelectObject(hdc, hFont);

		wchar_t* c = const_cast<wchar_t*>(text);
		// フォントビットマップ取得
		UINT code = (UINT)* c;

		const int gradFlag = GGO_GRAY4_BITMAP;
		// 階調の最大値
		int grad = 0;
		switch (gradFlag)
		{
		case GGO_GRAY2_BITMAP:
			grad = 4;
			break;
		case GGO_GRAY4_BITMAP:
			grad = 16;
			break;
		case GGO_GRAY8_BITMAP:
			grad = 64;
			break;
		}

		// フォントビットマップ取得
		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		GLYPHMETRICS gm;
		CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };

		/*
			ここで帰ってきたバイト数とブラックボックス矩形(gmBlackBoxX, gmBlackBoxY)から求めたバイト数が一致しない場合がある。
			これは、ブラックボックス矩形が文字を完全に囲んでいるわけではないことを証左している。
			↓参考ページ
			http://misohena.jp/article/ggo_trap/index.html
			↑横幅が4バイトアラインでサイズが変更してしまうため、そのように感じるだけ。
			gmBlackBoxXを4バイトアラインしてから計算すると合う。
		*/
		DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
		BYTE* fontBitmap = new BYTE[size];
		GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, fontBitmap, &mat);

		// オブジェクトの開放
		SelectObject(hdc, oldFont);
		DeleteObject(hFont);
		ReleaseDC(NULL, hdc);

		int fontWidth	= gm.gmCellIncX;
		int fontHeight	= tm.tmHeight;

		D3D11_TEXTURE2D_DESC fontTextureDesc;
		ZeroMemory(&fontTextureDesc, sizeof(fontTextureDesc));
		fontTextureDesc.Width				= fontWidth;
		fontTextureDesc.Height				= fontHeight;
		fontTextureDesc.MipLevels			= 1;
		fontTextureDesc.ArraySize			= 1;
		fontTextureDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
		fontTextureDesc.SampleDesc.Count	= 1;
		fontTextureDesc.Usage				= D3D11_USAGE_DYNAMIC;
		fontTextureDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		fontTextureDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

		DX_System*				system = DX_System::GetInstance();
		ID3D11Device*			device = system->GetDevice();
		ID3D11DeviceContext*	deviceContext = system->GetDeviceContext();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> fontTexture;
		device->CreateTexture2D(&fontTextureDesc, nullptr, &fontTexture);

		D3D11_MAPPED_SUBRESOURCE subResource;
		deviceContext->Map(fontTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

		BYTE* textureBuffer = (BYTE*)subResource.pData;
		memset(textureBuffer, 0, subResource.RowPitch * fontHeight);

		int iOfs_x = gm.gmptGlyphOrigin.x;
		int iOfs_y = tm.tmAscent - gm.gmptGlyphOrigin.y;
		int iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
		int iBmp_h = gm.gmBlackBoxY;
		int x, y;
		DWORD alpha, color;
		for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
		{
			for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
			{
				alpha = (255 * fontBitmap[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / grad;
				color = 0x00ffffff | (alpha << 24);

				memcpy((BYTE*)textureBuffer + subResource.RowPitch * y + 4 * x, &color, sizeof(DWORD));
			}
		}
		deviceContext->Unmap(fontTexture.Get(), 0);

		delete[] fontBitmap;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format				= fontTextureDesc.Format;
		srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = fontTextureDesc.MipLevels;

		if (DX_Debug::GetInstance()->IsFailedHresult(device->CreateShaderResourceView(fontTexture.Get(), &srvDesc, &fontSrv)))
		{
			break;
		}

		m_textures[filepath] = fontSrv;
	} while (false);
	
	return fontSrv;
}

/// <summary>
/// 指定したファイルパスのテクスチャをリストから探して取得する
/// </summary>
/// <param name="filepath">探すテクスチャのファイルパス</param>
/// <returns>あればテクスチャ、なければnullptr</returns>
ID3D11ShaderResourceView* DX_TextureManager::SearchTexture(const char* filepath)
{
	return m_textures.count(filepath) ? m_textures[filepath].Get() : nullptr;
}

/// <summary>
/// 指定したファイルパスからテクスチャを作成する
/// </summary>
/// <param name="filepath">作成するテクスチャのファイルパス</param>
/// <returns>作成できた場合テクスチャ、失敗した場合nullptr</returns>
ID3D11ShaderResourceView*  DX_TextureManager::CreateTexture(const char* filepath)
{
	ID3D11SRV* texture = nullptr;

	do
	{
		setlocale(LC_ALL, "japanese");

		// マルチバイト文字をワイド文字に変換する
		size_t size = 0;
		if (0 != mbstowcs_s(&size, NULL, 0, filepath, 0))
		{
			TRACE("Failed to mbstowcs_s()\n");
			return nullptr;
		}
		wchar_t wfilepath[MAX_PATH] = { NULL };
		mbstowcs_s(&size, wfilepath, size, filepath, _TRUNCATE);


		TexMetadata metadata;
		ScratchImage image;
		DX_Debug* pDebug = DX_Debug::GetInstance();
		if (pDebug->IsFailedHresult(LoadFromWICFile(wfilepath, NULL, &metadata, image))) {
			TRACE("failed to LoadFromWICFile()\n");
			return nullptr;
		}

		if (pDebug->IsFailedHresult(CreateShaderResourceView(DX_System::GetInstance()->GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture))) {
			TRACE("Failed to CreateShaderResourceView()\n");
			return nullptr;
		}

		texture->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(filepath) - 1, filepath);

		m_textures[filepath].Attach(texture);

	} while (false);


	return texture;
}