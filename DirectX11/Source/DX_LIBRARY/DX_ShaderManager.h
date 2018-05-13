#ifndef __DX_SHADER_MANAGER_H_
#define __DX_SHADER_MANAGER_H_

//	�g�p����V�F�[�_�[(�`��p�C�v���C���̏��Ԃɐݒ肵�Ă���)
enum class SHADER_TYPE : int{
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
	}_2D;

	typedef struct _MESH{
		static const char* VERTEX_SHADER;
		static const char* GEOMETRY_SHADER;
		static const char* PIXEL_SHADER;
	}_MESH;

	typedef struct _SKIN_MESH{
		static const char* VERTEX_SHADER;
		static const char* GEOMETRY_SHADER;
		static const char* PIXEL_SHADER;
	}_SKIN_MESH;

}DEFAULT_SHADER;

typedef DEFAULT_SHADER::_2D			DEFAULT_2D_SHADER;
typedef DEFAULT_SHADER::_MESH		DEFAULT_MESH_SHADER;
typedef DEFAULT_SHADER::_SKIN_MESH	DEFAULT_SKIN_MESH_SHADER;

//	2D
__declspec(selectany) const char* DEFAULT_2D_SHADER::VERTEX_SHADER	= "ShaderFile\\VS_2DObject.hlsl";
__declspec(selectany) const char* DEFAULT_2D_SHADER::PIXEL_SHADER	= "ShaderFile\\PS_2DObject.hlsl";

//	Mesh
__declspec(selectany) const char* DEFAULT_MESH_SHADER::VERTEX_SHADER	= "ShaderFile\\VS_3DObject.hlsl";
__declspec(selectany) const char* DEFAULT_MESH_SHADER::GEOMETRY_SHADER	= "ShaderFile\\GS_3DObject.hlsl";
__declspec(selectany) const char* DEFAULT_MESH_SHADER::PIXEL_SHADER		= "ShaderFile\\PS_3DObject.hlsl";

//	SkinMesh
__declspec(selectany) const char* DEFAULT_SKIN_MESH_SHADER::VERTEX_SHADER	= "ShaderFile\\VS_SkinMesh.hlsl";
__declspec(selectany) const char* DEFAULT_SKIN_MESH_SHADER::GEOMETRY_SHADER = "ShaderFile\\GS_3DObject.hlsl";
__declspec(selectany) const char* DEFAULT_SKIN_MESH_SHADER::PIXEL_SHADER	= "ShaderFile\\PS_3DObject.hlsl";


//****************************************************************************************************
//
//	ShaderManager
//
//****************************************************************************************************
class ShaderManager
{
private:
	static std::map<const char*, dx_library::Shader*>		m_shaders;

	static ID3D11InputLayout*	m_pInputLayout2D;
	static ID3D11InputLayout*	m_pInputLayout3D;
	static ID3D11InputLayout*	m_pInputLayoutSkinMesh;
	static ID3D11InputLayout*	m_pInputLayoutInstanceMesh;

	//	�R���s���[�g�V�F�[�_�[���g���邩�ǂ���
	static bool m_bCanUsetoComputeShader;

	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	ShaderManager() = delete;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	~ShaderManager(){}

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

	static dx_library::Shader*	GetShader(const char* pShaderFileName);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g��3D�`��p���_�V�F�[�_�[���擾
	//	@return		DX_VertexShader���Ԃ�
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader* GetDefaultVertexShader3D();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g�̃X�L�����b�V���`��p���_�V�F�[�_�[���擾
	//	@return		DX_VertexShader���Ԃ�
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader* GetDefaultVertexShaderSkinMesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g��3D�`��p�W�I���g���V�F�[�_�[���擾
	//	@return		DX_GeometryShader���Ԃ�
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader*	GetDefaultGeometryShader3D();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g�̃��C�s�b�N�����̓�����3D�`��p�W�I���g���V�F�[�_�[���擾
	//	@return		DX_GeometryShader���Ԃ�
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader*	GetDefaultGeometryShaderRayPick();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g��3D�`��p�s�N�Z���V�F�[�_�[���擾
	//	@return		DX_PixelShader���Ԃ�
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader* GetDefaultPixelShader3D();

	//------------------------------------------------------------------------------
	//
	//  @brief		�C���X�^���X���b�V���p�R���s���[�g�V�F�[�_�[���擾
	//	@return		DX_ComputeShader���Ԃ�
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader* GetInstanceMeshComputeShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g��2D�`��pInputLayout���擾����
	//	@return		m_pInplutLayout2D���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayout2D();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g��3D�`��pInputLayout���擾����
	//	@return		m_pInplutLayout3D���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayout3D();

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�t�H���g�̃X�L�����b�V���`��pInputLayout���擾����
	//	@return		m_pInputLayoutSkinMesh���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayoutSkinMesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		�C���X�^���X���b�V���`��pInputLayout���擾����
	//	@return		m_pInputLayoutInstanceMesh���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetInputLayoutInstanceMesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		���[���h�s��𑗂�
	//	@param[in]	worldMat			���[���h�s��
	//	@param[n]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//	@note		���W�X�^�ԍ���1
	//
	//------------------------------------------------------------------------------
	static void SetWorldMat(
		const DirectX::XMFLOAT4X4&				worldMat,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
		);

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
		dx_library::SHADER_TYPE	shaderType
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
		dx_library::SHADER_TYPE	shaderType
		);

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
		dx_library::SHADER_TYPE	shaderType
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
	static void SetMatrix(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4&				mat,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
		);

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
	static void SetMatrix(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4*				pMat,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
		);

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
		dx_library::SHADER_TYPE	shaderType
		);

	static void SetMatrixResoruce(
		const unsigned int			regiserNum,
		ID3D11Buffer*			pBuffer,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
		);


private:
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
	static void CreateShader(
		const char* pFilepath
		);

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
