#ifndef __DX_COMPUTE_SHADER_H_
#define __DX_COMPUTE_SHADER_H_


class DX_ComputeShader : public DX_Shader
{
public:
	/// <summary>
	/// メンバ変数を初期化
	/// </summary>
	DX_ComputeShader();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DX_ComputeShader();


	/// シェーダーを利用する
	/// </summary>
	/// <param name="classInstanceCount">クラスインスタンスの数</param>
	void Begin(const unsigned int classInstanceCount = 0);

private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_conmputeShader;


	/// <summary>
	/// シェーダーオブジェクトを作成する
	/// </summary>
	/// <returns>成否</returns>
	bool CreateShaderObject();
};
#endif // !__DX_COMPUTE_SHADER_H_
