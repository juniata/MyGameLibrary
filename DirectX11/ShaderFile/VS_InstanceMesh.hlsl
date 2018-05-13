//-------------------------------------------------------------
//	�r���[�Ɋւ���s��
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){
	float4x4	g_viewMat;
	float4x4	g_projMat;
	float4x4	g_viewProjMat;
};

//-------------------------------------------------------------
//	�C���X�^���X���̍s��
//-------------------------------------------------------------
Buffer<float4>  g_instanceMat : register(t1);

//-------------------------------------------------------------
//	�C���X�^���X���̍s��
//-------------------------------------------------------------
cbuffer InstanceCount : register(b2){
	uint	g_instanceCount;
	uint3	padding;
};

//-------------------------------------------------------------
//	���C�e�B���O
//-------------------------------------------------------------
cbuffer Lighting : register(b3){
	struct tagLight{
		float4 ambientColor;
		float4 diffuseColor;
		float3 viewPos;
		float3 lightPos;
		float3 lightDir;
		float3 padding;
	}g_light;
};

//-------------------------------------------------------------
//	���_�V�F�[�_�[�ɑ����Ă�����
//-------------------------------------------------------------
struct VS_INPUT
{
	float3 pos		: POSITION;
	float2 tex		: TEXCOORD;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};


//-------------------------------------------------------------
//	�W�I���g���V�F�[�_�[�ɑ�����
//-------------------------------------------------------------
struct VS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	//	���g�ŎZ�o���đ���ϐ�
	float3 wPos		: WPOS;		//	���_���W�����[���h���W�ɂ�������
	float3 wNormal  : NORMAL;	//	���_�@�������[���h���W�ɂ�������
	float3 viewDir  : VIEWDIR;	//	���[���h���W�̒��_����J�����ւ̃x�N�g��
	float3 lightDir : LIGHTDIR;	//	���[���h���W�̒��_���烉�C�g�ւ̃x�N�g��
};

//-------------------------------------------------------------
//	�g�U�����Z�o
//-------------------------------------------------------------
inline float3 DiffuseColor(float3 wNormal)
{
	return (pow(dot(wNormal, -g_light.lightDir), 0) * g_light.diffuseColor).rgb;
}

//-------------------------------------------------------------
//	g_instanceMat���烏�[���h�s����擾����
//-------------------------------------------------------------
inline float4x4 GetInstanceMatrix(uint index)
{
	return float4x4(
		g_instanceMat[index * 4 + 0],
		g_instanceMat[index * 4 + 1],
		g_instanceMat[index * 4 + 2],
		g_instanceMat[index * 4 + 3]
		);
}

//-------------------------------------------------------------
//	���_�V�F�[�_�[
//-------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT In,uint instanceID : SV_InstanceID)
{
	VS_OUTPUT Out;

	//	���[���h�s��
	float4x4 l_worldMat = GetInstanceMatrix(instanceID % g_instanceCount);

	//	3x3�̃��[���h�s��
	float3x3 wMat = (float3x3)l_worldMat;
	Out.pos		= mul(l_worldMat, float4(In.pos, 1));
	Out.wPos	= In.pos.xyz;
	Out.pos		= mul(g_viewProjMat, Out.pos);
	Out.tex		= In.tex;

	//	�J�����ւ̃x�N�g��
	Out.viewDir = normalize(g_light.viewPos - Out.wPos);

	//	���C�g�ւ̃x�N�g��
	Out.lightDir = normalize(g_light.lightPos - Out.wPos);

	//	�@�������[���h���W��(���W�͂���Ȃ��̂�3x3mat�Ōv�Z)
	Out.wNormal = mul(wMat, In.normal);

	//	�g�U���𔽉f
	Out.color.rgb = In.color.rgb * DiffuseColor(Out.wNormal);

	//	�����𔽉f
	Out.color.rgb += g_light.ambientColor.rgb;

	//	���_�F�̔񓧖��x��1�ɂ��ďo��
	Out.color.a = 1.0f;
	return Out;
}

