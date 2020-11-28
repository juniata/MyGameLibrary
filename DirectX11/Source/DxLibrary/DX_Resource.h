#ifndef __DX_RESOURCE_H_
#define __DX_RESOURCE_H_


/*
	リソース(View)作成を補助するクラス
*/
class DX_ResourceCreater
{
public:
	/// <summary>
	/// シェーダーに使用するリソースを作成する
	/// </summary>
	/// <param name="buffer"></param>
	/// <returns></returns>
	static ID3D11ShaderResourceView*	ShaderResourceView(ID3D11Buffer* buffer);

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D11BufferからID3D11UnorderedAccessViewを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pBuffer			作成したID3D11Buffer
	//	@return		作成したID3D11UnorderedAccessView
	//	@note		作成に失敗した場合はnullptrが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11UnorderedAccessView*	UnorderedAccessView(ID3D11Buffer* buffer);
};
#endif // !__DX_RESOURCE_H_
