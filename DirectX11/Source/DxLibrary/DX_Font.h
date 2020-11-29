#pragma once

/*
	フォント(一文字)を管理
*/
class DX_Font
{
public:
	/// <summary>
	/// メンバ変数を初期化
	/// </summary>
	DX_Font(const wchar_t msg);

	/// <summary>
	/// メンバ変数の解放
	/// </summary>
	~DX_Font();

	/// <summary>
	/// フォントの情報を設定する
	/// </summary>
	/// <param name="rect">描画する範囲</param>
	void SetInfo(const DX::tagRect& rect);

	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="vertexBuffer">更新する頂点バッファ</param>
	/// <param name="samplerState">使用すすサンプラーステート</param>
	void Render(ID3D11Buffer** vertexBuffer, ID3D11SamplerState* samplerState);
private:
	ID3D11ShaderResourceView* m_fontSrv;
	DX::tagRect m_rect;
};