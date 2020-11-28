#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  ID3D11Buffer����ShaderResourceView���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView*	DX_ResourceCreater::ShaderResourceView(ID3D11Buffer* buffer)
{
	ID3D11ShaderResourceView* srv = nullptr;

	//	�o�b�t�@�̐ݒ���擾
	D3D11_BUFFER_DESC bufferDesc = { NULL };
	buffer->GetDesc(&bufferDesc);

	//	SRV�̐ݒ���s��
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement	= 0;

	if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		//	ByteAddressBuffer�̏ꍇfloat4�ŃA�N�Z�X���邽�߁A���̐ݒ�
		srvDesc.Format					= DXGI_FORMAT_R32_TYPELESS;
		srvDesc.BufferEx.Flags			= D3D11_BUFFEREX_SRV_FLAG_RAW;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / 4;

	}
	else if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{

		srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
	}
	
	//	SRV���쐬����
	HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &srv);

	return DX_Debug::GetInstance()->IsFailedHresult(hr) ? nullptr : srv;
}



//-----------------------------------------------------------------------------------------
//
//  ID3D11Buffer����ID3D11UnorderedAccessView���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11UnorderedAccessView*	DX_ResourceCreater::UnorderedAccessView(ID3D11Buffer* buffer)
{
	ID3D11UnorderedAccessView*	uav = nullptr;

	//	�o�b�t�@�̐ݒ���擾
	D3D11_BUFFER_DESC bufferDesc = { NULL };
	buffer->GetDesc(&bufferDesc);

	//	UAV�̐ݒ���s��
	D3D11_UNORDERED_ACCESS_VIEW_DESC  uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	uavDesc.ViewDimension			= D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement	= 0;
	
	if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS){

		//	ByteAddressBuffer�̏ꍇfloat4�ŃA�N�Z�X���邽�߁A���̐ݒ�
		uavDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
		uavDesc.Buffer.Flags			= D3D11_BUFFEREX_SRV_FLAG_RAW;

		//	4byte�̗v�f���������邩��ݒ�
		uavDesc.Buffer.NumElements	= bufferDesc.ByteWidth / 4;

	}
	else if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{

		//	�^���s��
		uavDesc.Format				= DXGI_FORMAT_UNKNOWN;

		//	�v�f�����ݒ肵�Ă���
		uavDesc.Buffer.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
	}

	//	UAV���쐬����
	HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateUnorderedAccessView(buffer, &uavDesc, &uav);

	return DX_Debug::GetInstance()->IsFailedHresult(hr) ? nullptr : uav;

}