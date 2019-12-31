#ifndef __DX_SHADER_MANAGER_H_
#define __DX_SHADER_MANAGER_H_

//	�g�p����V�F�[�_�[(�`��p�C�v���C���̏��Ԃɐݒ肵�Ă���)
enum class DX_SHADER_TYPE : int{
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER
};


typedef struct DEFAULT_SHADER{

	typedef struct _2D{
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
		static const char* INSTANCE_VERTEX_SHADER;
	}_2D;

	typedef struct _OBJECT {
		static const char* VERTEX_SHADER;
		static const char* GEOMETRY_SHADER;
		static const char* PIXEL_SHADER;
	}_OBJECT;

}DEFAULT_SHADER;

typedef DEFAULT_SHADER::_2D			DEFAULT_2D_SHADER;
typedef DEFAULT_SHADER::_OBJECT		DEFAULT_OBJECT_SHADER;


//****************************************************************************************************
//
//	ShaderManager
//
//****************************************************************************************************
class DX_ShaderManager
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ��̏�����(�V�F�[�_�[�̍쐬)
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�Ɋւ���ϐ������
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief		�w�肵���V�F�[�_�[���擾����
	//	@return		DX_Shader*
	//
	//------------------------------------------------------------------------------
	static DX_Shader* GetShader(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g��2D�`��pInputLayout���擾����
	//	@return		m_pInplutLayout2D���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayout2D();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g�̃C���X�^���X2D�`��pInputLayout���擾����
	//	@return		m_pInplutLayout3D���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayoutInstance2D();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g�̃C���X�^���X2D�`��pInputLayout���擾����
	//	@return		m_pInplutLayout3D���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayoutObject() {
		return m_pInputLayoutObject
			;
	}

	//------------------------------------------------------------------------------
	//
	//  @brief		���[���h�s��𑗂�
	//	@param[in]	worldMat			���[���h�s��
	//	@param[n]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//	@note		���W�X�^�ԍ���1
	//
	//------------------------------------------------------------------------------
	static void SetWorldMat(ID3D11Buffer* pBuffer, const DirectX::XMFLOAT4X4& worldMat, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType);

	//------------------------------------------------------------------------------
	//
	//  @brief		int���V�F�[�_�[�ɑ���
	//	@return		m_pInplutLayout3D���Ԃ�
	//
	//------------------------------------------------------------------------------
	static void SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const int value, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		Vector4���V�F�[�_�[�ɑ���
	//	@param[in]	registerNum		�V�F�[�_�[�̃��W�X�^�ԍ�
	//	@param[in]	vec4			Vector4
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType		SHADER_TYPE
	//	
	//------------------------------------------------------------------------------
	static void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT4&				vec4,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

	//------------------------------------------------------------------------------
	//
	//  @brief		Vector3���V�F�[�_�[�ɑ���
	//	@param[in]	registerNum		�V�F�[�_�[�̃��W�X�^�ԍ�
	//	@param[in]	vec4			Vector4
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType		SHADER_TYPE
	//	
	//------------------------------------------------------------------------------
	static void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT3&				vec3,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

	static void SetVectorResource(const unsigned int registerNum, const DirectX::XMFLOAT4* pVec3,
		const unsigned int			vecCount,
		ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType);
	//------------------------------------------------------------------------------
	//
	//  @brief		Vector2���V�F�[�_�[�ɑ���
	//	@param[in]	registerNum		�V�F�[�_�[�̃��W�X�^�ԍ�
	//	@param[in]	vec4			Vector4
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType		SHADER_TYPE
	//	
	//------------------------------------------------------------------------------
	static void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT2&				vec2,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

	//------------------------------------------------------------------------------
	//
	//  @brief		�s����V�F�[�_�[�ɑ���
	//	@param[in]	regiserNum			�V�F�[�_�[�̃��W�X�^�ԍ�
	//	@param[in]	mat					Matrix
	//	@param[in]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//	
	//------------------------------------------------------------------------------
	static void SetMatrix(ID3D11Buffer* pBuffer, const unsigned int regiserNum, const DirectX::XMFLOAT4X4& mat, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType);

	//------------------------------------------------------------------------------
	//
	//  @brief		�����̍s����V�F�[�_�[�ɑ���
	//	@param[in]	regiserNum			�V�F�[�_�[�̃��W�X�^�ԍ�
	//	@param[in]	pMat				�s��𕡐�����
	//	@param[in]	matCount			����s��
	//	@param[in]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//
	//------------------------------------------------------------------------------
	static void SetMatrix(ID3D11Buffer* pBuffer, const unsigned int regiserNum, const DirectX::XMFLOAT4X4*	pMat, const unsigned int matCount, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType);

	//------------------------------------------------------------------------------
	//
	//  @brief		�����̍s���SRV�Ƃ��ăV�F�[�_�[�ɑ���
	//	@param[in]	regiserNum			�V�F�[�_�[�̃��W�X�^�ԍ�
	//	@param[in]	pMat				�s��𕡐�����
	//	@param[in]	matCount			����s��
	//	@param[in]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//
	//------------------------------------------------------------------------------
	static void SetMatrixResoruce(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4*				pMat,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

	static void SetMatrixResoruce(
		const unsigned int			regiserNum,
		ID3D11Buffer*			pBuffer,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

private:
	static const size_t SHADER_NUM = 10;
	struct Shader {
		DX_Shader* pShader;
		const char* pFilepath;
	};
	static Shader m_shaders[SHADER_NUM];

	static ID3D11InputLayout*	m_pInputLayout2D;
	static ID3D11InputLayout*	m_pInputLayoutInstance2D;
	static ID3D11InputLayout*	m_pInputLayoutObject;

	//	�R���s���[�g�V�F�[�_�[���g���邩�ǂ���
	static bool m_bCanUsetoComputeShader;
	
	//------------------------------------------------------------------------------
	//
	//  @brief		�R���s���[�g�V�F�[�_�[���g���邩�ǂ������`�F�b�N
	//
	//------------------------------------------------------------------------------
	static void UsedComputeShaderCheck();

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�̍쐬
	//	@param[in]	pFilepath	�V�F�[�_�[�t�@�C���̃p�X
	//
	//------------------------------------------------------------------------------
	static void CreateShader(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ��ɂ���InputLauout���쐬����
	//
	//------------------------------------------------------------------------------
	static void CreateInputLayout();

	//------------------------------------------------------------------------------
	//
	//  @brief		InputLauout���쐬����
	//	@param[in]	pDevice				DX_System::GetDevice()
	//	@param[in]	pInputLayoutDesc	�쐬����D3D11_INPUT_ELEMENT_DESC
	//	@param[in]	inputLayoutCount	�쐬����D3D11_INPUT_ELEMENT_DESC�̗v�f��
	//	@param[in]	pByteCord			�V�F�[�_�[���R���p�C�������Ƃ��̃o�C�g�R�[�h
	//	@param[in]	pInputLayout		�쐬����pInputLayout
	//
	//------------------------------------------------------------------------------
	static void CreateInputLayout(
		ID3D11Device*				pDevice,
		D3D11_INPUT_ELEMENT_DESC*	pInputLayoutDesc,
		const unsigned int			inputLayoutCount,
		ID3DBlob*					pByteCord,
		ID3D11InputLayout**			pInputLayout
		);


};
#endif // !__DX_SHADER_MANAGER_H_
