#pragma once
/*
	デバッグ時に画面に表示するためのクラス。

	アドベンチャーゲームなど、がっつり文字を表示するためのものではない。
*/
class DX_Text : public DX_Singleton<DX_Text>
{
public:
	/// <summary>
	/// メンバ変数等の解放処理を行う
	/// </summary>
	~DX_Text();

	/// <summary>
	/// 文字を表示する
	/// </summary>
	/// <param name="x">開始描画位置</param>
	/// <param name="y">開始描画位置</param>
	/// <param name="width">描画する一文字のサイズ(横幅)</param>
	/// <param name="height">描画する一文字のサイズ(高さ)</param>
	/// <param name="pszFormat">フォーマット。printf等と同じ使い方</param>
	/// <param name="...">可変長引数。printf等と同じ使い方</param>
	void Print(const float x, const float y, const float width, const float height, LPCSTR pszFormat, ...);
private:
	/// <summary>
	/// メンバ変数の初期化及びサンプラーやバッファの作成
	/// </summary>
	DX_Text();
	friend class DX_Singleton<DX_Text>;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_fontSampler;
	std::map<wchar_t, DX_Font*> m_fontList;
};