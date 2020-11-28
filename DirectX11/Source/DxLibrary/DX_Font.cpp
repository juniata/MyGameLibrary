#include "DX_Library.h"
#include "../extern/DirectXTex/DirectXTex.h"
#include <tchar.h>
#include <locale.h>

/// <summary>
/// メンバ変数を初期化
/// </summary>
DX_Font::DX_Font(const wchar_t* msg) : m_fontSrv(nullptr)
{
	ZeroMemory(&m_rect, sizeof(m_rect));
	m_fontSrv = DX_TextureManager::GetFontTexture(msg);

}

/// <summary>
/// メンバ変数の解放
/// </summary>
DX_Font::~DX_Font()
{
	DX_TextureManager::Release(m_fontSrv);
}

/// <summary>
/// フォントの情報を設定する
/// </summary>
/// <param name="msg">メッセージ</param>
/// <param name="rect">描画する範囲</param>
void DX_Font::SetInfo(const wchar_t* msg, const DX::tagRect& rect)
{
	m_fontSrv = DX_TextureManager::GetFontTexture(msg);
	m_rect = rect;
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="vertexBuffer">更新する頂点バッファ</param>
/// <param name="samplerState">使用すすサンプラーステート</param>
void DX_Font::Render(ID3D11Buffer** buffer, ID3D11SamplerState* samplerState)
{
	DX_System* system = DX_System::GetInstance();
	ID3D11DeviceContext* deviceContext = system->GetDeviceContext();

	float windowWidth = static_cast<float>(system->GetScreenWidth());
	float windowHeight = static_cast<float>(system->GetScreenHeight());

	//	-1.0f ~ 1.0fに座標を正規化する
	DirectX::XMFLOAT2 center(1.0f / (windowWidth * 0.5f), 1.0f / (windowHeight * 0.5f));


	DX::tagRect norRectPos;
	norRectPos.left = center.x * m_rect.x - 1.0f;
	norRectPos.right = center.x * m_rect.w - 1.0f;
	norRectPos.bottom = 1.0f - center.y * m_rect.h;
	norRectPos.top = 1.0f - center.y * m_rect.y;

	//	頂点バッファを作成
	DX::tagVertex2D vertices[] = {
		// 左下
		DirectX::XMFLOAT3(norRectPos.left, norRectPos.bottom, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		// 左上
		DirectX::XMFLOAT3(norRectPos.left, norRectPos.top, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		// 右下
		DirectX::XMFLOAT3(norRectPos.right, norRectPos.bottom, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		// 右上
		DirectX::XMFLOAT3(norRectPos.right, norRectPos.top, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};

	// バッファの上書き
	deviceContext->UpdateSubresource(*buffer, 0, nullptr, vertices, 0, 0);


	DX_ShaderManager* shaderManager = DX_ShaderManager::GetInstance();

	DX_Shader* vertexShader = shaderManager->GetShader(SHADER_OBJECT_2D::VERTEX_SHADER);
	DX_Shader* pixelShader = shaderManager->GetShader(SHADER_OBJECT_2D::PIXEL_SHADER);

	vertexShader->Begin();
	pixelShader->Begin();

	unsigned int stride = sizeof(DX::tagVertex2D);
	unsigned int offset = 0;

	deviceContext->PSSetSamplers(0, 1, &samplerState);
	deviceContext->IASetVertexBuffers(0, 1, buffer, &stride, &offset);
	deviceContext->IASetInputLayout(shaderManager->GetInputLayout2D());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	shaderManager->SetShaderResources(0, 1, &m_fontSrv, SHADER_TYPE::PIXEL_SHADER);

	deviceContext->Draw(4, 0);

	vertexShader->End();
	pixelShader->End();

}