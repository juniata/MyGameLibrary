#ifndef __DX_COMPUTE_SHADER_H_
#define __DX_COMPUTE_SHADER_H_


class ComputeShader : public dx_library::Shader
{
private:
	ComPtr<ID3D11ComputeShader>	m_computeShader;

public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を初期化
	//
	//------------------------------------------------------------------------------
	ComputeShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		実体があれば解放
	//
	//------------------------------------------------------------------------------
	~ComputeShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーを作成する
	//	@param[in]	pFilepath	シェーダーファイルのパス
	//
	//------------------------------------------------------------------------------
	void CreateShader(
		const char* pFilepath
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの使用を開始
	//	@param[in]	classInstanceCount	クラスインスタンスの数
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	void Begin(
		ID3D11DeviceContext*	pDeviceContext,
		const unsigned int		classInstanceCount = 0
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの利用を終える
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	void End(
		ID3D11DeviceContext*	pDeviceContext
		);

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーオブジェクトを作成する
	//
	//------------------------------------------------------------------------------
	void CreateShaderObject();
};
#endif // !__DX_COMPUTE_SHADER_H_
