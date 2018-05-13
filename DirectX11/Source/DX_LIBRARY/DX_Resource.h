#ifndef __DX_RESOURCE_H_
#define __DX_RESOURCE_H_


//****************************************************************************************************
//
//	Resource(~View)�𐶐�����N���X
//
//****************************************************************************************************
class Resource
{
private:

public:
	
	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D11Buffer����ShaderResourceView���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pBuffer		�쐬����ID3D11Buffer
	//	@return		�쐬����ID3D11ShaderResourceView
	//	@note		�쐬�Ɏ��s�����ꍇ��nullptr���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11ShaderResourceView*	CreateShaderResourceView(
		ID3D11Device*	pDevice,
		ID3D11Buffer*	pBuffer
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D11Buffer����ID3D11UnorderedAccessView���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pBuffer			�쐬����ID3D11Buffer
	//	@return		�쐬����ID3D11UnorderedAccessView
	//	@note		�쐬�Ɏ��s�����ꍇ��nullptr���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11UnorderedAccessView*	CreateUnorderedAccessView(
		ID3D11Device*	pDevice,
		ID3D11Buffer*	pBuffer
		);
};
#endif // !__DX_RESOURCE_H_
