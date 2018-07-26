#include	"DX_Library.h"
#include	<array>

//-----------------------------------------------------------------------------------------
//
//  �}�N���萔
//
//-----------------------------------------------------------------------------------------
#define _NO_RESOURCE -1

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
//	buffer�Ɋւ���ϐ�
ID3D11Buffer*							DX_ResourceManager::m_pNullBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { nullptr };
DX_ResourceManager::tagResourceInfo		DX_ResourceManager::m_bufferInfo[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
int										DX_ResourceManager::m_bufferRegisterNum;

//	resoucr�Ɋւ���ϐ�
ID3D11ShaderResourceView*				DX_ResourceManager::m_pNullShaderResourceView[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
DX_ResourceManager::tagResourceInfo		DX_ResourceManager::m_srvInfo[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
int										DX_ResourceManager::m_srvRegisterNum = _NO_RESOURCE;

//-----------------------------------------------------------------------------------------
//
//  �w�肵���V�F�[�_�[�X�e�[�W�Ƀo�b�t�@��ݒ肷��
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
	//	���W�X�^�ԍ�������l�𒴂��ĂȂ������`�F�b�N
	DEBUG_VALUE_CHECK((registerNum < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) ? true : false, "���W�X�^�ԍ�������𒴂��Ă��܂��");

	//	�w�肵�����W�X�^�ɐݒ肷��o�b�t�@�̐�������l�𒴂��Ă��邩���`�F�b�N����
	DEBUG_VALUE_CHECK(((D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - registerNum) > viewCount) ? true : false, "�w�肵�����W�X�^�ԍ��ɐݒ�ł���o�b�t�@�̐�������𒴂��Ă��܂�");
	
	//	�o�b�t�@���ݒ肵�Ă���ꍇ
	if (m_bufferRegisterNum != _NO_RESOURCE)
	{
		//	�O��̏����擾
		tagResourceInfo* l_pBufferInfo = &m_bufferInfo[m_bufferRegisterNum];

		//	������ݒ肷�郊�\�[�X�̃��W�X�^�ԍ����A�O��ݒ肵�����\�[�X�̃��W�X�^�ԍ��Ɠ����ȏꍇ�A
		//	�O��ݒ肵�����\�[�X�̐������ANULL��ݒ肷��
		if (registerNum == l_pBufferInfo->registerNum){
			SelectSetShaderConstantBuffers(pDeviceContext, l_pBufferInfo->registerNum, l_pBufferInfo->viewCount, m_pNullBuffer, shaderType);
		}
	}

	// �o�b�t�@��ݒ�
	SelectSetShaderConstantBuffers(pDeviceContext, registerNum, viewCount, pBuffers, shaderType);

	//	�O��̃��W�X�^�ԍ����X�V����
	m_bufferRegisterNum = registerNum;

	//	�V�����f�[�^��ݒ肷��
	tagResourceInfo* l_pBufferInfo = &m_bufferInfo[registerNum];
	l_pBufferInfo->registerNum = registerNum;
	l_pBufferInfo->viewCount = viewCount;
}

//-----------------------------------------------------------------------------------------
//
//  �w�肵���V�F�[�_�[�X�e�[�W�Ƀ��\�[�X��ݒ肷��
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
	//	���W�X�^�ԍ�������l�𒴂��ĂȂ������`�F�b�N
	DEBUG_VALUE_CHECK((registerNum < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) ? true : false, "���W�X�^�ԍ�������𒴂��Ă��܂��");

	//	�w�肵�����W�X�^�ɐݒ肷��o�b�t�@�̐�������l�𒴂��Ă��邩���`�F�b�N����
	DEBUG_VALUE_CHECK(((D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - registerNum) > viewCount) ? true : false, "�w�肵�����W�X�^�ԍ��ɐݒ�ł���o�b�t�@�̐�������𒴂��Ă��܂�");

	//	���\�[�X���ݒ肵�Ă���ꍇ
	if (m_srvRegisterNum != _NO_RESOURCE)
	{
		//	�O��̏����擾
		tagResourceInfo* l_pResoucrInfo = &m_srvInfo[m_srvRegisterNum];

		//	������ݒ肷�郊�\�[�X�̃��W�X�^�ԍ����A�O��ݒ肵�����\�[�X�̃��W�X�^�ԍ��Ɠ����ȏꍇ�A
		//	�O��ݒ肵�����\�[�X�̐������ANULL��ݒ肷��
		if (registerNum == l_pResoucrInfo->registerNum){
			SelectSetShaderResources(pDeviceContext, l_pResoucrInfo->registerNum, l_pResoucrInfo->viewCount, m_pNullShaderResourceView, shaderType);
		}
	}

	//	���\�[�X��ݒ�
	SelectSetShaderResources(pDeviceContext, registerNum, viewCount, pShaderResourceView, shaderType);

	//	�O��̃��W�X�^�ԍ����X�V����
	m_srvRegisterNum = registerNum;

	//	�V�����f�[�^��ݒ肷��
	tagResourceInfo* l_pResoucrInfo = &m_srvInfo[registerNum];
	l_pResoucrInfo->registerNum = registerNum;
	l_pResoucrInfo->viewCount	= viewCount;
}


//-----------------------------------------------------------------------------------------
//
//  �w�肵���V�F�[�_�[�X�e�[�W�Ƀo�b�t�@��ݒ肷��
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
//  �w�肵���V�F�[�_�[�X�e�[�W�Ƀ��\�[�X��ݒ肷��
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
