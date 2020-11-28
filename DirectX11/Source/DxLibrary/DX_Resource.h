#ifndef __DX_RESOURCE_H_
#define __DX_RESOURCE_H_


/*
	���\�[�X(View)�쐬��⏕����N���X
*/
class DX_ResourceCreater
{
public:
	/// <summary>
	/// �V�F�[�_�[�Ɏg�p���郊�\�[�X���쐬����
	/// </summary>
	/// <param name="buffer"></param>
	/// <returns></returns>
	static ID3D11ShaderResourceView*	ShaderResourceView(ID3D11Buffer* buffer);

	//------------------------------------------------------------------------------
	//
	//  @brief		ID3D11Buffer����ID3D11UnorderedAccessView���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pBuffer			�쐬����ID3D11Buffer
	//	@return		�쐬����ID3D11UnorderedAccessView
	//	@note		�쐬�Ɏ��s�����ꍇ��nullptr���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11UnorderedAccessView*	UnorderedAccessView(ID3D11Buffer* buffer);
};
#endif // !__DX_RESOURCE_H_
