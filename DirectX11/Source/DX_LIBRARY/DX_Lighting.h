#ifndef __DX_LIGHTING_H_
#define __DX_LIGHTING_H_

//	���_�V�F�[�_�[�ōs�����C�e�B���O���
struct tagVertexLighting{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT3 viewPos;
	DirectX::XMFLOAT3 lightPos;
	DirectX::XMFLOAT3 lightDir;
	DirectX::XMFLOAT3	padding;	//	16byte alignment
};

//	�s�N�Z���V�F�[�_�[�ōs��lighting���
struct tagPixelLighting{
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 viewPos;
	DirectX::XMFLOAT3 lightDir;
	DirectX::XMFLOAT2 padding;	//	16byte alignment
};
//****************************************************************************************************
//
//	Lighting
//
//****************************************************************************************************
class Lighting
{
private:
	static DirectX::XMFLOAT4 m_ambient;	//	�g�U��
	static DirectX::XMFLOAT4 m_diffuse;	//	����
	static DirectX::XMFLOAT4 m_specular;	//	���ˌ�
	static DirectX::XMFLOAT3 m_viewPos;	//	���_
	static DirectX::XMFLOAT3 m_lightPos;	//	���C�g�̍��W
	static DirectX::XMFLOAT3 m_lightDir;	//	���C�g�̌���

	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	Lighting() = delete;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�������Ȃ�
	//
	//------------------------------------------------------------------------------
	~Lighting(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ���������
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		������ݒ�
	//	@param[in]	ambient	����
	//	@note		rgb�̏��� 0~1�̒l
	//
	//------------------------------------------------------------------------------
	static void SetAmbient(const DirectX::XMFLOAT3& ambient);

	//------------------------------------------------------------------------------
	//
	//  @brief		�g�U����ݒ�
	//	@param[in]	diffuse	�g�U��
	//	@note		rgb�̏��� 0~1�̒l
	//
	//------------------------------------------------------------------------------
	static void SetDiffuse(const DirectX::XMFLOAT3& diffuse);

	//------------------------------------------------------------------------------
	//
	//  @brief		���ˌ���ݒ�
	//	@param[in]	specular	���ˌ�
	//	@note		rgb�̏��� 0~1�̒l
	//
	//------------------------------------------------------------------------------
	static void SetSpecular(const DirectX::XMFLOAT3& specular);

	//------------------------------------------------------------------------------
	//
	//  @brief		���C�g�̍��W��ݒ�
	//	@param[in]	pos	���C�g���W
	//
	//------------------------------------------------------------------------------
	static void SetLightPos(const DirectX::XMFLOAT3& pos);

	//------------------------------------------------------------------------------
	//
	//  @brief		���̌�����ݒ�
	//	@param[in]	dir	���̌���
	//	@note		���K�����Ă������Ă�������
	//
	//------------------------------------------------------------------------------
	static void SetLightDir(const DirectX::XMFLOAT3& dir);

	//------------------------------------------------------------------------------
	//
	//  @brief		���_��ݒ�
	//	@param[in]	pos	���_���W
	//
	//------------------------------------------------------------------------------
	static void SetViewPos(const DirectX::XMFLOAT3& pos);

	//------------------------------------------------------------------------------
	//
	//  @brief		���_�V�F�[�_�[�ōs�����C�g����ݒ�
	//
	//------------------------------------------------------------------------------
	static void SetLightVertexShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		�s�N�Z���V�F�[�_�[�ōs�����C�g����ݒ�
	//
	//------------------------------------------------------------------------------
	static void SetLightPixelShader();
};
#endif // !__DX_LIGHTING_H_
