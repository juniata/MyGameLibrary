#ifndef __DX_RESOURCE_H_
#define __DX_RESOURCE_H_


//****************************************************************************************************
//
//	Resource(~View)‚ğ¶¬‚·‚éƒNƒ‰ƒX
//
//****************************************************************************************************
class Resource
{
private:

public:
	
	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D11Buffer‚©‚çShaderResourceView‚ğì¬‚·‚é
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pBuffer		ì¬‚µ‚½ID3D11Buffer
	//	@return		ì¬‚µ‚½ID3D11ShaderResourceView
	//	@note		ì¬‚É¸”s‚µ‚½ê‡‚Ínullptr‚ª•Ô‚é
	//
	//------------------------------------------------------------------------------
	static ID3D11ShaderResourceView*	CreateShaderResourceView(
		ID3D11Device*	pDevice,
		ID3D11Buffer*	pBuffer
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D11Buffer‚©‚çID3D11UnorderedAccessView‚ğì¬‚·‚é
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pBuffer			ì¬‚µ‚½ID3D11Buffer
	//	@return		ì¬‚µ‚½ID3D11UnorderedAccessView
	//	@note		ì¬‚É¸”s‚µ‚½ê‡‚Ínullptr‚ª•Ô‚é
	//
	//------------------------------------------------------------------------------
	static ID3D11UnorderedAccessView*	CreateUnorderedAccessView(
		ID3D11Device*	pDevice,
		ID3D11Buffer*	pBuffer
		);
};
#endif // !__DX_RESOURCE_H_
