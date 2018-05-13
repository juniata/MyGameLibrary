#ifndef __DX_RESOURCE_MANAGER_H_
#define __DX_RESOURCE_MANAGER_H_
//****************************************************************************************************
//
//	シェーダーにリソースを送ったりするのを管理するクラス
//	バッファやリソースが同じ場所に設定されている場合、Nullを設定してからバッファやリソースを
//	設定します
//
//****************************************************************************************************
class DX_ResourceManager
{
private:
	struct tagResourceInfo{
		int registerNum;
		int viewCount;
		tagResourceInfo() : registerNum(-1), viewCount(0){}
	};

	//	bufferに関する変数
	static ID3D11Buffer*				m_pNullBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	static tagResourceInfo				m_bufferInfo[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	static int							m_bufferRegisterNum;

	//	resourceに関する変数
	static ID3D11ShaderResourceView*	m_pNullShaderResourceView[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	static tagResourceInfo				m_srvInfo[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	static int							m_srvRegisterNum;
	
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		指定したシェーダーステージにID3D11Bufferを設定する
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	registerNum		hlsl側のregister(番号)
	//	@param[in]	viewCount		指定したレジスタに送るリソースの数
	//	@param[in]	shaderType		リソースを送るステージのタイプ
	//
	//------------------------------------------------------------------------------
	static void SetConstantbuffers(
		ID3D11DeviceContext*		pDeviceContext,
		const int					registerNum,
		const int					viewCount,
		ID3D11Buffer* const*		pBuffers,
		DX_SHADER_TYPE	shaderType
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		指定したシェーダーステージにShaderResourceViewを設定する
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	registerNum		hlsl側のregister(番号)
	//	@param[in]	viewCount		指定したレジスタに送るリソースの数
	//	@param[in]	shaderType		リソースを送るステージのタイプ
	//
	//------------------------------------------------------------------------------
	static void SetShaderResources(
		ID3D11DeviceContext*				pDeviceContext, 
		const int							registerNum, 
		const int							viewCount, 
		ID3D11ShaderResourceView* const*	pShaderResourceView, 
		DX_SHADER_TYPE			shaderType
		);

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		指定したシェーダーステージにID3D11Bufferを設定する
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	registerNum		hlsl側のregister(番号)
	//	@param[in]	viewCount		指定したレジスタに送るリソースの数
	//	@param[in]	shaderType		リソースを送るステージのタイプ
	//
	//------------------------------------------------------------------------------
	static void SelectSetShaderConstantBuffers(
		ID3D11DeviceContext*		pDeviceContext,
		const int					registerNum,
		const int					viewCount,
		ID3D11Buffer* const*		pBuffers,
		DX_SHADER_TYPE	shaderType
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		指定したシェーダーステージにShaderResourceViewを設定する
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	registerNum		hlsl側のregister(番号)
	//	@param[in]	viewCount		指定したレジスタに送るリソースの数
	//	@param[in]	shaderType		リソースを送るステージのタイプ
	//
	//------------------------------------------------------------------------------
	static void SelectSetShaderResources(
		ID3D11DeviceContext*				pDeviceContext,
		const int							registerNum,
		const int							viewCount, 
		ID3D11ShaderResourceView* const*	pShaderResourceView,
		DX_SHADER_TYPE			shaderType
		);
};

#endif // !__DX_RESOURCE_MANAGER_H_
