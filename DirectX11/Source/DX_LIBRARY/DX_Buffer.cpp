#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  �萔�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateConstantBuffer(
	ID3D11Device*	pDevice,
	const size_t	bufferSize
	)
{
	D3D11_BUFFER_DESC		l_bufferDesc = { NULL };
	D3D11_SUBRESOURCE_DATA	l_subResourceData = { NULL };

	l_bufferDesc.ByteWidth	= bufferSize;
	l_bufferDesc.Usage		= D3D11_USAGE_DEFAULT;
	l_bufferDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;

	ID3D11Buffer* l_pBuffer = nullptr;

	//	buffer���쐬����
	if (!DX_Debug::IsHresultCheck(pDevice->CreateBuffer(&l_bufferDesc, nullptr, &l_pBuffer))){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}


//-----------------------------------------------------------------------------------------
//
//  ���_�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateVertexBuffer(
	ID3D11Device*	pDevice,
	const size_t	vertexDataSize,
	const void*		pVertex
	)
{
	D3D11_BUFFER_DESC		l_bufferDesc = { NULL };
	D3D11_SUBRESOURCE_DATA	l_subResourceData = { NULL };

	//	buffer�̐ݒ�
	l_bufferDesc.ByteWidth		= vertexDataSize;
	l_bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;

	//	subResourceData�̐ݒ�
	l_subResourceData.pSysMem = pVertex;

	ID3D11Buffer* l_pBuffer = nullptr;
	
	//	buffer���쐬����
	if (!DX_Debug::IsHresultCheck(pDevice->CreateBuffer(&l_bufferDesc, &l_subResourceData, &l_pBuffer))){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}


//-----------------------------------------------------------------------------------------
//
//  �\�����o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateStructuredBuffer(
	ID3D11Device*	pDevice,
	const size_t	elementSize,
	const size_t	elementCount,
	void*	pData
	)
{
	ID3D11Buffer*	l_pBuffer = nullptr;

	D3D11_BUFFER_DESC l_bufferDesc = { NULL };
	l_bufferDesc.BindFlags				= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	l_bufferDesc.ByteWidth				= elementSize * elementCount;
	l_bufferDesc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	l_bufferDesc.StructureByteStride	= elementSize;

	HRESULT l_hr;
	if (pData){
		D3D11_SUBRESOURCE_DATA l_subResourceDate = { NULL };
		l_subResourceDate.pSysMem = pData;

		l_hr = pDevice->CreateBuffer(&l_bufferDesc, &l_subResourceDate, &l_pBuffer);
	}
	else{
		l_hr = pDevice->CreateBuffer(&l_bufferDesc, nullptr, &l_pBuffer);
	}

	//	���s���Ă���ꍇ�́Anull��Ԃ�
	if (DX_Debug::IsHresultCheck(l_hr) == false){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}

//-----------------------------------------------------------------------------------------
//
//  �o�C�g�A�h���X�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateByteAddressBuffer(
	ID3D11Device*	pDevice,
	const size_t	dataSize,
	void*			pData,
	const bool		isVertex
	)
{
	ID3D11Buffer*	l_pBuffer = nullptr;

	D3D11_BUFFER_DESC l_bufferDesc = { NULL };
	l_bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	l_bufferDesc.ByteWidth = dataSize;
	l_bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	l_bufferDesc.StructureByteStride = 0;

	if (isVertex){
		l_bufferDesc.BindFlags |= D3D11_BIND_VERTEX_BUFFER;
	}

	HRESULT l_hr;

	if (pData){
		D3D11_SUBRESOURCE_DATA l_subResourceDate = { NULL };
		l_subResourceDate.pSysMem = pData;

		l_hr = pDevice->CreateBuffer(&l_bufferDesc, &l_subResourceDate, &l_pBuffer);
	}
	else{
		l_hr = pDevice->CreateBuffer(&l_bufferDesc, nullptr, &l_pBuffer);
	}

	//	���s���Ă���ꍇ�́Anull��Ԃ�
	if (DX_Debug::IsHresultCheck(l_hr) == false){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}

//-----------------------------------------------------------------------------------------
//
//  �C���f�b�N�X�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateIndexBuffer(
	ID3D11Device*	pDevice,
	const size_t	indexDataSize,
	const void*		pIndex
	)
{
	D3D11_BUFFER_DESC		l_bufferDesc = { NULL };
	D3D11_SUBRESOURCE_DATA	l_subResourceData = { NULL };

	//	buffer�̐ݒ�
	l_bufferDesc.ByteWidth = indexDataSize;
	l_bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	//	subResourceData�̐ݒ�
	l_subResourceData.pSysMem = pIndex;

	ID3D11Buffer* l_pBuffer = nullptr;

	//	buffer���쐬����
	if (!DX_Debug::IsHresultCheck(pDevice->CreateBuffer(&l_bufferDesc, &l_subResourceData, &l_pBuffer))){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}

//-----------------------------------------------------------------------------------------
//
//  �X�L���o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CreateSkinBuffer(
	ID3D11Device*	pDevice,
	const size_t	skinDatasize,
	const void*		pIndex
	)
{
	D3D11_BUFFER_DESC		l_bufferDesc = { NULL };
	D3D11_SUBRESOURCE_DATA	l_subResourceData = { NULL };

	//	buffer�̐ݒ�
	l_bufferDesc.ByteWidth = skinDatasize;
	l_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//	subResourceData�̐ݒ�
	l_subResourceData.pSysMem = pIndex;

	ID3D11Buffer* l_pBuffer = nullptr;

	//	buffer���쐬����
	if (!DX_Debug::IsHresultCheck(pDevice->CreateBuffer(&l_bufferDesc, &l_subResourceData, &l_pBuffer))){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}

//-----------------------------------------------------------------------------------------
//
//  �W�I���g���X�g���[���o�͗p�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CreateStreamOutputBuffer(
	ID3D11Device*	pDevice,
	const size_t	bufferDateSize
	)
{
	D3D11_BUFFER_DESC		l_bufferDesc = { NULL };
	l_bufferDesc.ByteWidth	= bufferDateSize;
	l_bufferDesc.Usage		= D3D11_USAGE_DEFAULT;
	l_bufferDesc.BindFlags	= D3D11_BIND_STREAM_OUTPUT;

	ID3D11Buffer* l_pBuffer = nullptr;

	//	buffer���쐬����
	if (!DX_Debug::IsHresultCheck(pDevice->CreateBuffer(&l_bufferDesc, nullptr, &l_pBuffer))){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}


//-----------------------------------------------------------------------------------------
//
//  CPU�ŏ������݉\�ȃo�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CPUWriteBuffer(
	ID3D11Device*	pDevice,
	const size_t	bufferDateSize
	)
{
	D3D11_BUFFER_DESC		l_bufferDesc = { NULL };
	l_bufferDesc.ByteWidth		= bufferDateSize;
	l_bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	l_bufferDesc.BindFlags		= D3D11_BIND_SHADER_RESOURCE;
	l_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ID3D11Buffer* l_pBuffer = nullptr;

	//	buffer���쐬����
	if (!DX_Debug::IsHresultCheck(pDevice->CreateBuffer(&l_bufferDesc, nullptr, &l_pBuffer))){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}


//-----------------------------------------------------------------------------------------
//
//  CPU�œǂݍ��݉\�ȃo�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CPUReadBuffer(
	ID3D11Device*	pDevice,
	const size_t	bufferDateSize,
	const size_t	strctureByteStride
	)
{
	D3D11_BUFFER_DESC		l_bufferDesc = { NULL };
	l_bufferDesc.ByteWidth = bufferDateSize;
	l_bufferDesc.Usage			= D3D11_USAGE_STAGING;
	l_bufferDesc.BindFlags		= 0;
	l_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	l_bufferDesc.StructureByteStride = strctureByteStride;

	ID3D11Buffer* l_pBuffer = nullptr;

	//	buffer���쐬����
	if (!DX_Debug::IsHresultCheck(pDevice->CreateBuffer(&l_bufferDesc, nullptr, &l_pBuffer))){
		//SAFE_RELEASE(l_pBuffer);
	}

	return l_pBuffer;
}
//-----------------------------------------------------------------------------------------
//
//  2D��`��
//
//-----------------------------------------------------------------------------------------
void DX_Buffer::Render2D(ID3D11Buffer* pVertexBuffer, ID3D11ShaderResourceView* pShaderRerousceView)
{
	unsigned int l_stride = sizeof(tagVertex2D);
	unsigned int l_offset = 0;

	//	�f�o�C�X�R���e�L�X�g���擾
	auto l_deviceContext = DX_System::GetInstance()->GetDeviceContext();

	//	VertexBuffer�𑗂�
	l_deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &l_stride, &l_offset);

	//	InputLayout�̐ݒ�𑗂�
	l_deviceContext->IASetInputLayout(DX_ShaderManager::GetInstance()->GetDefaultInputLayout2D());

	//	Primitive�̐ݒ�𑗂�
	l_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//	PixelShader�Ƀe�N�X�`�����𑗂�
	DX_ResourceManager::SetShaderResources(l_deviceContext, 0, 1, &pShaderRerousceView,DX_SHADER_TYPE::PIXEL_SHADER);

	//	�`��
	l_deviceContext->Draw(4, 0);

}
