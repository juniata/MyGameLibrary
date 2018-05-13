#ifndef __DX_RESOURCE_H_
#define __DX_RESOURCE_H_


//****************************************************************************************************
//
//	Resource(~View)を生成するクラス
//
//****************************************************************************************************
class Resource
{
private:

public:
	
	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D11BufferからShaderResourceViewを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pBuffer		作成したID3D11Buffer
	//	@return		作成したID3D11ShaderResourceView
	//	@note		作成に失敗した場合はnullptrが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11ShaderResourceView*	CreateShaderResourceView(
		ID3D11Device*	pDevice,
		ID3D11Buffer*	pBuffer
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D11BufferからID3D11UnorderedAccessViewを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pBuffer			作成したID3D11Buffer
	//	@return		作成したID3D11UnorderedAccessView
	//	@note		作成に失敗した場合はnullptrが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11UnorderedAccessView*	CreateUnorderedAccessView(
		ID3D11Device*	pDevice,
		ID3D11Buffer*	pBuffer
		);
};
#endif // !__DX_RESOURCE_H_
