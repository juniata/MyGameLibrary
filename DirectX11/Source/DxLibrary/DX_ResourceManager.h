#ifndef __DX_RESOURCE_MANAGER_H_
#define __DX_RESOURCE_MANAGER_H_
//****************************************************************************************************
//
//	�V�F�[�_�[�Ƀ��\�[�X�𑗂����肷��̂��Ǘ�����N���X
//	�o�b�t�@�⃊�\�[�X�������ꏊ�ɐݒ肳��Ă���ꍇ�ANull��ݒ肵�Ă���o�b�t�@�⃊�\�[�X��
//	�ݒ肵�܂�
//
//****************************************************************************************************
class DX_ResourceManager
{
private:
	struct tagResourceInfo{
		int registerNum;
		int viewCount;
		tagResourceInfo() : registerNum(-1), viewCount(0){}
	};

	//	buffer�Ɋւ���ϐ�
	static ID3D11Buffer*				m_pNullBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	static tagResourceInfo				m_bufferInfo[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
	static int							m_bufferRegisterNum;

	//	resource�Ɋւ���ϐ�
	static ID3D11ShaderResourceView*	m_pNullShaderResourceView[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	static tagResourceInfo				m_srvInfo[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
	static int							m_srvRegisterNum;
	
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�w�肵���V�F�[�_�[�X�e�[�W��ID3D11Buffer��ݒ肷��
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	registerNum		hlsl����register(�ԍ�)
	//	@param[in]	viewCount		�w�肵�����W�X�^�ɑ��郊�\�[�X�̐�
	//	@param[in]	shaderType		���\�[�X�𑗂�X�e�[�W�̃^�C�v
	//
	//------------------------------------------------------------------------------
	static void SetConstantbuffers(
		ID3D11DeviceContext*		pDeviceContext,
		const int					registerNum,
		const int					viewCount,
		ID3D11Buffer* const*		pBuffers,
		DX_SHADER_TYPE	shaderType
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�w�肵���V�F�[�_�[�X�e�[�W��ShaderResourceView��ݒ肷��
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	registerNum		hlsl����register(�ԍ�)
	//	@param[in]	viewCount		�w�肵�����W�X�^�ɑ��郊�\�[�X�̐�
	//	@param[in]	shaderType		���\�[�X�𑗂�X�e�[�W�̃^�C�v
	//
	//------------------------------------------------------------------------------
	static void SetShaderResources(
		ID3D11DeviceContext*				pDeviceContext, 
		const int							registerNum, 
		const int							viewCount, 
		ID3D11ShaderResourceView* const*	pShaderResourceView, 
		DX_SHADER_TYPE			shaderType
		);

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		�w�肵���V�F�[�_�[�X�e�[�W��ID3D11Buffer��ݒ肷��
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	registerNum		hlsl����register(�ԍ�)
	//	@param[in]	viewCount		�w�肵�����W�X�^�ɑ��郊�\�[�X�̐�
	//	@param[in]	shaderType		���\�[�X�𑗂�X�e�[�W�̃^�C�v
	//
	//------------------------------------------------------------------------------
	static void SelectSetShaderConstantBuffers(
		ID3D11DeviceContext*		pDeviceContext,
		const int					registerNum,
		const int					viewCount,
		ID3D11Buffer* const*		pBuffers,
		DX_SHADER_TYPE	shaderType
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�w�肵���V�F�[�_�[�X�e�[�W��ShaderResourceView��ݒ肷��
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	registerNum		hlsl����register(�ԍ�)
	//	@param[in]	viewCount		�w�肵�����W�X�^�ɑ��郊�\�[�X�̐�
	//	@param[in]	shaderType		���\�[�X�𑗂�X�e�[�W�̃^�C�v
	//
	//------------------------------------------------------------------------------
	static void SelectSetShaderResources(
		ID3D11DeviceContext*				pDeviceContext,
		const int							registerNum,
		const int							viewCount, 
		ID3D11ShaderResourceView* const*	pShaderResourceView,
		DX_SHADER_TYPE			shaderType
		);
};

#endif // !__DX_RESOURCE_MANAGER_H_
