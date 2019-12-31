#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  ID3D11Buffer����ShaderResourceView���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView*	DX_Resource::CreateShaderResourceView(
	ID3D11Device*	pDevice,
	ID3D11Buffer*	pBuffer
	)
{
	ID3D11ShaderResourceView* l_pSRV = nullptr;

	//	�o�b�t�@�̐ݒ���擾
	D3D11_BUFFER_DESC l_bufferDesc = { NULL };
	pBuffer->GetDesc(&l_bufferDesc);

	//	SRV�̐ݒ���s��
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));

	l_srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
	l_srvDesc.BufferEx.FirstElement	= 0;

	if (l_bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS){
		
		//	ByteAddressBuffer�̏ꍇfloat4�ŃA�N�Z�X���邽�߁A���̐ݒ�
		l_srvDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
		l_srvDesc.BufferEx.Flags		= D3D11_BUFFEREX_SRV_FLAG_RAW;
		l_srvDesc.BufferEx.NumElements	= l_bufferDesc.ByteWidth / 4;

	}else if (l_bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED){

		l_srvDesc.Format				= DXGI_FORMAT_UNKNOWN;
		l_srvDesc.BufferEx.NumElements	= l_bufferDesc.ByteWidth / l_bufferDesc.StructureByteStride;
	}
	
	//	SRV���쐬����
	HRESULT l_hr = pDevice->CreateShaderResourceView(pBuffer, &l_srvDesc, &l_pSRV);

	//	�߂�l�`�F�b�N
	if (DX_Debug::GetInstance()->CheckHresult(l_hr) == false){
		l_pSRV->Release();
	}
	return l_pSRV;
}



//-----------------------------------------------------------------------------------------
//
//  ID3D11Buffer����ID3D11UnorderedAccessView���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11UnorderedAccessView*	DX_Resource::CreateUnorderedAccessView(
	ID3D11Device*	pDevice,
	ID3D11Buffer*	pBuffer
	)
{
	ID3D11UnorderedAccessView*	l_pUAV = nullptr;

	//	�o�b�t�@�̐ݒ���擾
	D3D11_BUFFER_DESC l_bufferDesc = { NULL };
	pBuffer->GetDesc(&l_bufferDesc);

	//	UAV�̐ݒ���s��
	D3D11_UNORDERED_ACCESS_VIEW_DESC  l_uavDesc;
	ZeroMemory(&l_uavDesc, sizeof(l_uavDesc));

	l_uavDesc.ViewDimension			= D3D11_UAV_DIMENSION_BUFFER;
	l_uavDesc.Buffer.FirstElement	= 0;
	
	if (l_bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS){

		//	ByteAddressBuffer�̏ꍇfloat4�ŃA�N�Z�X���邽�߁A���̐ݒ�
		l_uavDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
		l_uavDesc.Buffer.Flags			= D3D11_BUFFEREX_SRV_FLAG_RAW;

		//	4byte�̗v�f���������邩��ݒ�
		l_uavDesc.Buffer.NumElements	= l_bufferDesc.ByteWidth / 4;

	}
	else if (l_bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED){

		//	�^���s��
		l_uavDesc.Format				= DXGI_FORMAT_UNKNOWN;

		//	�v�f�����ݒ肵�Ă���
		l_uavDesc.Buffer.NumElements	= l_bufferDesc.ByteWidth / l_bufferDesc.StructureByteStride;
	}

	//	UAV���쐬����
	HRESULT l_hr = pDevice->CreateUnorderedAccessView(pBuffer, &l_uavDesc, &l_pUAV);

	//	�߂�l�`�F�b�N
	if (DX_Debug::GetInstance()->CheckHresult(l_hr) == false){
		l_pUAV->Release();
	}

	return l_pUAV;

}