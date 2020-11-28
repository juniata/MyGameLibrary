#ifndef __DX_GEOMETRY_SHADER_H_
#define __DX_GEOMETRY_SHADER_H_

/// <summary>
/// ジオメトリシェーダー
/// </summary>
class DX_GeometryShader : public DX_Shader
{
public:
	/// <summary>
	/// メンバ変数を初期化
	/// </summary>
	DX_GeometryShader();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DX_GeometryShader();

	/// シェーダーを利用する
	/// </summary>
	/// <param name="classInstanceCount">クラスインスタンスの数</param>
	void Begin(const unsigned int classInstanceCount = 0);


	/// <summary>
	/// ジオメトリシェーダーから出力したデータを取得
	/// </summary>
	/// <param name="decreation">ジオメトリと同じセマンティクスのものにデータを出力</param>
	/// <param name="decreationElementCount">decreationの要素数</param>
	/// <param name="bufferStrides">データを分割する線引き</param>
	/// <param name="stridesElementCount">bufferStridesの要素数</param>
	void CreateGeometryShaderWithStreamOutput(D3D11_SO_DECLARATION_ENTRY decreation[], const UINT decreationElementCount, unsigned int* bufferStrides, const UINT stridesElementCount);
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_outputGeometryShader;


	/// <summary>
	/// シェーダーオブジェクトを作成する
	/// </summary>
	/// <returns>成否</returns>
	bool CreateShaderObject();
};
#endif // !__DX_GEOMETRY_SHADER_H_
