#include "DX_Library.h"
#include "../extern/DirectXTex/DirectXTex.h"
#include <tchar.h>
#include <locale.h>

/// <summary>
/// メンバ変数の初期化
/// </summary>
DX_Text::DX_Text()
{
	DX_System*		system = DX_System::GetInstance();
	ID3D11Device*	device = system->GetDevice();
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samDesc, &m_fontSampler);
	// TODO: なぜかExisting private data of same name with different size found!が出るので、コメントアウト
	//m_fontSampler->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("DX_Text::m_fontSampler") - 1, "DX_Text::m_fontSampler");

	//	頂点バッファを作成
	DX::tagVertex2D vertices[] = {
		/* 左下 */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* 左上 */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* 右下 */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* 右上 */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};

	m_vertexBuffer.Attach(DX_BufferCreater::VertexBuffer(sizeof(vertices), vertices));
	m_vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("DX_Text::m_vertexBuffer") - 1, "DX_Text::m_vertexBuffer");

}

/// <summary>
/// メンバ変数の初期化及びサンプラーやバッファの作成
/// </summary>
DX_Text::~DX_Text()
{
	for (auto itr : m_fontList)
	{
		DELETE_OBJ(itr.second);
	}

	m_fontList.clear();
}

/// <summary>
/// 文字を表示する
/// </summary>
/// <param name="x">開始描画位置</param>
/// <param name="y">開始描画位置</param>
/// <param name="width">描画する一文字のサイズ(横幅)</param>
/// <param name="height">描画する一文字のサイズ(高さ)</param>
/// <param name="pszFormat">フォーマット。printf等と同じ使い方</param>
/// <param name="...">可変長引数。printf等と同じ使い方</param>
void DX_Text::Print(const float x, const float y, const float width, const float height, LPCSTR pszFormat, ...)
{
	va_list	argp;
	char pszBuf[256];

	va_start(argp, pszFormat);
	vsprintf_s(pszBuf, pszFormat, argp);
	va_end(argp);

	char message[256];
	sprintf_s(message, "%s", pszBuf);

	wchar_t msg[256];
	size_t ret = 0;
	mbstowcs_s(&ret, msg, 256, message, _TRUNCATE);

	size_t size = wcslen(msg);
	DX_Font* font = nullptr;
	for (int i = 0; i < size; ++i)
	{
		wchar_t c = msg[i];

		DX::tagRect rect;
		rect.x = x + width * static_cast<float>(i);
		rect.y = y;
		rect.w = rect.x + width;
		rect.h = rect.y + height;

		// キーがなければ生成する
		if (!m_fontList.count(c))
		{
			m_fontList[c] = new DX_Font(c);
		}
		font = m_fontList[c];
		// TODO:なぜか毎度ＳＲＶを取得する必要がある。。なぜ？
		font->SetInfo(rect);
		font->Render(m_vertexBuffer.GetAddressOf(), m_fontSampler.Get());
	}
}
