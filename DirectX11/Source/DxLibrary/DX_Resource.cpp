#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  ID3D11BufferからShaderResourceViewを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11ShaderResourceView*	DX_ResourceCreater::ShaderResourceView(ID3D11Buffer* buffer)
{
	ID3D11ShaderResourceView* srv = nullptr;

	//	バッファの設定を取得
	D3D11_BUFFER_DESC bufferDesc = { NULL };
	buffer->GetDesc(&bufferDesc);

	//	SRVの設定を行う
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement	= 0;

	if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		//	ByteAddressBufferの場合float4でアクセスするため、この設定
		srvDesc.Format					= DXGI_FORMAT_R32_TYPELESS;
		srvDesc.BufferEx.Flags			= D3D11_BUFFEREX_SRV_FLAG_RAW;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / 4;

	}
	else if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{

		srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
	}
	
	//	SRVを作成する
	HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &srv);

	return DX_Debug::GetInstance()->IsFailedHresult(hr) ? nullptr : srv;
}



//-----------------------------------------------------------------------------------------
//
//  ID3D11BufferからID3D11UnorderedAccessViewを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11UnorderedAccessView*	DX_ResourceCreater::UnorderedAccessView(ID3D11Buffer* buffer)
{
	ID3D11UnorderedAccessView*	uav = nullptr;

	//	バッファの設定を取得
	D3D11_BUFFER_DESC bufferDesc = { NULL };
	buffer->GetDesc(&bufferDesc);

	//	UAVの設定を行う
	D3D11_UNORDERED_ACCESS_VIEW_DESC  uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	uavDesc.ViewDimension			= D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement	= 0;
	
	if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS){

		//	ByteAddressBufferの場合float4でアクセスするため、この設定
		uavDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
		uavDesc.Buffer.Flags			= D3D11_BUFFEREX_SRV_FLAG_RAW;

		//	4byteの要素がいくつあるかを設定
		uavDesc.Buffer.NumElements	= bufferDesc.ByteWidth / 4;

	}
	else if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{

		//	型が不明
		uavDesc.Format				= DXGI_FORMAT_UNKNOWN;

		//	要素数が設定している
		uavDesc.Buffer.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
	}

	//	UAVを作成する
	HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateUnorderedAccessView(buffer, &uavDesc, &uav);

	return DX_Debug::GetInstance()->IsFailedHresult(hr) ? nullptr : uav;

}