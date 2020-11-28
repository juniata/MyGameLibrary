#ifndef __DX_VERTEX_SHADER_H_
#define __DX_VERTEX_SHADER_H_

/// <summary>
/// 頂点シェーダー
/// </summary>
class DX_VertexShader : public DX_Shader
{
public:
	/// <summary>
	/// メンバ変数を初期化
	/// </summary>
	DX_VertexShader();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DX_VertexShader();

	/// <summary>
	/// シェーダーを利用する
	/// </summary>
	/// <param name="classInstanceCount">クラスインスタンスの数</param>
	void Begin(const unsigned int classInstanceCount = 0);

	/// <summary>
	/// シェーダーの利用を終える
	/// </summary>
	void End();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	/// <summary>
	/// シェーダーオブジェクトを作成する
	/// </summary>
	/// <returns>成否</returns>
	bool  CreateShaderObject();
};
#endif // !__DX_VERTEX_SHADER_H_
