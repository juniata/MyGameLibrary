#ifndef __DX_PIXEL_SHADER_H_
#define __DX_PIXEL_SHADER_H_

/// <summary>
/// ピクセルシェーダー
/// </summary>
class DX_PixelShader : public DX_Shader
{
public:
	/// <summary>
	/// メンバ変数を初期化
	/// </summary>
	DX_PixelShader();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DX_PixelShader();

	/// シェーダーを利用する
	/// </summary>
	/// <param name="classInstanceCount">クラスインスタンスの数</param>
	void Begin(const unsigned int classInstanceCount = 0);

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	/// <summary>
	/// シェーダーオブジェクトを作成する
	/// </summary>
	/// <returns>成否</returns>
	bool CreateShaderObject();
};
#endif // !__DX_PIXEL_SHADER_H_
