#ifndef __DX_GEOMETRY_SHADER_H_
#define __DX_GEOMETRY_SHADER_H_

class DX_GeometryShader : public DX_Shader
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を初期化
	//
	//------------------------------------------------------------------------------
	DX_GeometryShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		実体があれば解放
	//
	//------------------------------------------------------------------------------
	~DX_GeometryShader();

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
		const unsigned int classInstanceCount = 0
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


	//------------------------------------------------------------------------------
	//
	//  @brief		ジオメトリーシェーダーから出力したデータを取得
	//	@param[in]	decreation				ジオメトリと同じセマンティクスのものにデータを出力
	//	@param[in]	decreationElementCount	decreationの要素数
	//	@param[in]	pBufferStrides			データを分割する線引き
	//	@param[in]	stridesElementCount		pBufferStridesの要素数
	//
	//------------------------------------------------------------------------------
	void CreateGeometryShaderWithStreamOutput(
		D3D11_SO_DECLARATION_ENTRY	decreation[],	
		const UINT					decreationElementCount,
		unsigned int*				pBufferStrides,
		const UINT					stridesElementCount
		);
private:
	ID3D11GeometryShader*	m_pGeometryShader;
	ID3D11GeometryShader*	m_pOutputGeometryShader;

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーオブジェクトを作成する
	//
	//------------------------------------------------------------------------------
	void CreateShaderObject();
};
#endif // !__DX_GEOMETRY_SHADER_H_
