#include	"DX_Library.h"
#include	<array>

//-----------------------------------------------------------------------------------------
//
//  マクロ定数
//
//-----------------------------------------------------------------------------------------
#define _NO_RESOURCE -1

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
//	bufferに関する変数
ID3D11Buffer*							DX_ResourceManager::m_pNullBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { nullptr };
DX_ResourceManager::tagResourceInfo		DX_ResourceManager::m_bufferInfo[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
int										DX_ResourceManager::m_bufferRegisterNum;

//	resoucrに関する変数
ID3D11ShaderResourceView*				DX_ResourceManager::m_pNullShaderResourceView[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
DX_ResourceManager::tagResourceInfo		DX_ResourceManager::m_srvInfo[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
int										DX_ResourceManager::m_srvRegisterNum = _NO_RESOURCE;

//-----------------------------------------------------------------------------------------
//
//  指定したシェーダーステージにバッファを設定する
//
//-----------------------------------------------------------------------------------------
void DX_ResourceManager::SetConstantbuffers(
	ID3D11DeviceContext*		pDeviceContext,
	const int					registerNum,
	const int					viewCount,
	ID3D11Buffer* const*		pBuffers,
	DX_SHADER_TYPE	shaderType
	)
{
	//	レジスタ番号を上限値を超えてないかをチェック
	DEBUG_VALUE_CHECK((registerNum < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) ? true : false, "レジスタ番号が上限を超えています｡");

	//	指定したレジスタに設定するバッファの数が上限値を超えているかをチェックする
	DEBUG_VALUE_CHECK(((D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - registerNum) > viewCount) ? true : false, "指定したレジスタ番号に設定できるバッファの数が上限を超えています");
	
	//	バッファが設定している場合
	if (m_bufferRegisterNum != _NO_RESOURCE)
	{
		//	前回の情報を取得
		tagResourceInfo* l_pBufferInfo = &m_bufferInfo[m_bufferRegisterNum];

		//	今から設定するリソースのレジスタ番号が、前回設定したリソースのレジスタ番号と同じな場合、
		//	前回設定したリソースの数だけ、NULLを設定する
		if (registerNum == l_pBufferInfo->registerNum){
			SelectSetShaderConstantBuffers(pDeviceContext, l_pBufferInfo->registerNum, l_pBufferInfo->viewCount, m_pNullBuffer, shaderType);
		}
	}

	// バッファを設定
	SelectSetShaderConstantBuffers(pDeviceContext, registerNum, viewCount, pBuffers, shaderType);

	//	前回のレジスタ番号を更新する
	m_bufferRegisterNum = registerNum;

	//	新しいデータを設定する
	tagResourceInfo* l_pBufferInfo = &m_bufferInfo[registerNum];
	l_pBufferInfo->registerNum = registerNum;
	l_pBufferInfo->viewCount = viewCount;
}

//-----------------------------------------------------------------------------------------
//
//  指定したシェーダーステージにリソースを設定する
//
//-----------------------------------------------------------------------------------------
void DX_ResourceManager::SetShaderResources(
	ID3D11DeviceContext*				pDeviceContext,
	const int							registerNum,
	const int							viewCount, 
	ID3D11ShaderResourceView* const*	pShaderResourceView,
	DX_SHADER_TYPE			shaderType
	)
{
	//	レジスタ番号を上限値を超えてないかをチェック
	DEBUG_VALUE_CHECK((registerNum < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) ? true : false, "レジスタ番号が上限を超えています｡");

	//	指定したレジスタに設定するバッファの数が上限値を超えているかをチェックする
	DEBUG_VALUE_CHECK(((D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - registerNum) > viewCount) ? true : false, "指定したレジスタ番号に設定できるバッファの数が上限を超えています");

	//	リソースが設定している場合
	if (m_srvRegisterNum != _NO_RESOURCE)
	{
		//	前回の情報を取得
		tagResourceInfo* l_pResoucrInfo = &m_srvInfo[m_srvRegisterNum];

		//	今から設定するリソースのレジスタ番号が、前回設定したリソースのレジスタ番号と同じな場合、
		//	前回設定したリソースの数だけ、NULLを設定する
		if (registerNum == l_pResoucrInfo->registerNum){
			SelectSetShaderResources(pDeviceContext, l_pResoucrInfo->registerNum, l_pResoucrInfo->viewCount, m_pNullShaderResourceView, shaderType);
		}
	}

	//	リソースを設定
	SelectSetShaderResources(pDeviceContext, registerNum, viewCount, pShaderResourceView, shaderType);

	//	前回のレジスタ番号を更新する
	m_srvRegisterNum = registerNum;

	//	新しいデータを設定する
	tagResourceInfo* l_pResoucrInfo = &m_srvInfo[registerNum];
	l_pResoucrInfo->registerNum = registerNum;
	l_pResoucrInfo->viewCount	= viewCount;
}


//-----------------------------------------------------------------------------------------
//
//  指定したシェーダーステージにバッファを設定する
//
//-----------------------------------------------------------------------------------------
void DX_ResourceManager::SelectSetShaderConstantBuffers(
	ID3D11DeviceContext*		pDeviceContext,
	const int					registerNum,
	const int					viewCount,
	ID3D11Buffer* const*		pBuffers,
	DX_SHADER_TYPE	shaderType
	)
{
	switch (shaderType)
	{
	case DX_SHADER_TYPE::VERTEX_SHADER:
		pDeviceContext->VSSetConstantBuffers(registerNum, viewCount, pBuffers);
		break;

	case DX_SHADER_TYPE::HULL_SHADER:
		pDeviceContext->HSSetConstantBuffers(registerNum, viewCount, pBuffers);
		break;

	case DX_SHADER_TYPE::DOMAIN_SHADER:
		pDeviceContext->DSSetConstantBuffers(registerNum, viewCount, pBuffers);
		break;

	case DX_SHADER_TYPE::GEOMETRY_SHADER:
		pDeviceContext->GSSetConstantBuffers(registerNum, viewCount, pBuffers);
		break;

	case DX_SHADER_TYPE::PIXEL_SHADER:
		pDeviceContext->PSSetConstantBuffers(registerNum, viewCount, pBuffers);
		break;

	case DX_SHADER_TYPE::COMPUTE_SHADER:
		pDeviceContext->CSSetConstantBuffers(registerNum, viewCount, pBuffers);
		break;

	}

}

//-----------------------------------------------------------------------------------------
//
//  指定したシェーダーステージにリソースを設定する
//
//-----------------------------------------------------------------------------------------
void DX_ResourceManager::SelectSetShaderResources(
	ID3D11DeviceContext*				pDeviceContext,
	const int							registerNum, 
	const int							viewCount,
	ID3D11ShaderResourceView* const*	pShaderResourceView,
	DX_SHADER_TYPE 			shaderType
	)
{
	switch (shaderType)
	{
	case DX_SHADER_TYPE::VERTEX_SHADER:
		pDeviceContext->VSSetShaderResources(registerNum, viewCount, pShaderResourceView);
		break;

	case DX_SHADER_TYPE::HULL_SHADER:
		pDeviceContext->HSSetShaderResources(registerNum, viewCount, pShaderResourceView);
		break;

	case DX_SHADER_TYPE::DOMAIN_SHADER:
		pDeviceContext->DSSetShaderResources(registerNum, viewCount, pShaderResourceView);
		break;

	case DX_SHADER_TYPE::GEOMETRY_SHADER:
		 pDeviceContext->GSSetShaderResources(registerNum, viewCount, pShaderResourceView);
		break;

	case DX_SHADER_TYPE::PIXEL_SHADER:
		pDeviceContext->PSSetShaderResources(registerNum, viewCount, pShaderResourceView);
		break;

	case DX_SHADER_TYPE::COMPUTE_SHADER:
		pDeviceContext->CSSetShaderResources(registerNum, viewCount, pShaderResourceView);
		break;

	}

}
