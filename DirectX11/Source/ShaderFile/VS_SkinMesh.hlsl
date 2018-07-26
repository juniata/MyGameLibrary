//-------------------------------------------------------------
//	�r���[�Ɋւ���s��
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){

	float4x4	g_viewMat;
	float4x4	g_projMat;
	float4x4	g_viewProjMat;
};

//-------------------------------------------------------------
//	�I�u�W�F�N�g�̃��[���h���W
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1){
	float4x4	g_worldMat;
};

//-------------------------------------------------------------
//	�{�[���s��
//-------------------------------------------------------------
Buffer<float4>  g_bones : register(t1);

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
	//	startslot 0
	float3  pos			: POSITION;
	float2  tex			: TEXCOORD;
	float3	normal		: NORMAL;
	float4	color		: COLOR;
	
	//	startslot 1
	uint    skinIndex	: SKIN_INDEX;
	float4	skinWeight	: SKIN_WEIGHT;
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
//	�{�[���C���f�b�N�X���g��Buffer<float4> g_bones����s����擾
//-------------------------------------------------------------
float3x4 GetSkinningMatrix(uint boneIndex)
{
	float4 r0 = g_bones.Load(boneIndex * 4 + 0);
	float4 r1 = g_bones.Load(boneIndex * 4 + 1);
	float4 r2 = g_bones.Load(boneIndex * 4 + 2);

	//	4�Ԗڂ̗v�f��0,0,0,1�Ȃ̂ŗv��Ȃ�
	return float3x4(r0, r1, r2);
}

//-------------------------------------------------------------
//	�X�L�j���O����
//-------------------------------------------------------------
inline float3x4 UpdateSkinMesh( const uint boneIndex, const float4 skinWeight)
{
	//	�{�[���s��̒P�ʍs��
	float3x4 skinIdentity = 0;

	//	�X�L�j���O����
	skinIdentity =  GetSkinningMatrix((boneIndex & 0x000000ff)		)	* skinWeight.x;
	skinIdentity += GetSkinningMatrix((boneIndex & 0x0000ff00) >> 8 )	* skinWeight.y;
	skinIdentity += GetSkinningMatrix((boneIndex & 0x00ff0000) >> 16)	* skinWeight.z;
	skinIdentity += GetSkinningMatrix((boneIndex & 0xff000000) >> 24)	* skinWeight.w;

	return skinIdentity;
}

//-------------------------------------------------------------
//	�{�[�����e����^���钸�_�̐F��ݒ�
//-------------------------------------------------------------
inline void SetBoneColor(inout float4 vertexColor, float3 color, uint skinIndex, int boneNum)
{
	if ((skinIndex & 0x000000ff) == boneNum){
		vertexColor.rgb = color;
	}else if (((skinIndex & 0x0000ff00) >>  8) == boneNum){
		vertexColor.rgb = color;
	}else if (((skinIndex & 0x00ff0000) >> 16) == boneNum){
		vertexColor.rgb = color;
	}else if (((skinIndex & 0xff000000) >> 24) == boneNum){
		vertexColor.rgb = color;
	}
}

//-------------------------------------------------------------
//	�g�U�����Z�o
//-------------------------------------------------------------
inline float3 DiffuseColor(float3 wNormal)
{
	return (pow(dot(wNormal, -g_light.lightDir), 0) * g_light.diffuseColor).rgb;
}

//-------------------------------------------------------------
//	float4 = float3 * float3x4
//-------------------------------------------------------------
inline float4 MultiFloat3toFloat3x4(float3 pos, float3x4 mat)
{
	float4 outpos;

	//	dot(mat[0],pos) + mat._14�ł��v�Z��
	outpos.x = (pos.x * mat._11) + (pos.y * mat._12) + (pos.z * mat._13) + mat._14;
	outpos.y = (pos.x * mat._21) + (pos.y * mat._22) + (pos.z * mat._23) + mat._24;
	outpos.z = (pos.x * mat._31) + (pos.y * mat._32) + (pos.z * mat._33) + mat._34;
	outpos.w = 1;

	return outpos;
}

//-------------------------------------------------------------
//	���_�V�F�[�_�[
//-------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT In)
{
	VS_OUTPUT Out;

	float3x3 wMat = g_worldMat;

	//	�X�L�j���O�s��
	float3x4 skinningMatrix = UpdateSkinMesh(In.skinIndex, In.skinWeight);

	//	���_���W���X�L�j���O
	Out.pos = MultiFloat3toFloat3x4(In.pos, skinningMatrix);
	
	//	���[�J�����W���烏�[���h���W
	//Out.pos = MultiFloat3toFloat3x4(Out.pos.xyz, g_worldMat);
	Out.pos = mul(g_worldMat, Out.pos);

	//	�@�����X�L�j���O
	Out.wNormal = MultiFloat3toFloat3x4(In.normal, skinningMatrix).xyz;
	Out.wNormal = mul(wMat, In.normal);

	//	���[���h���W
	Out.wPos = Out.pos.xyz;

	//	���[���h���W����ˉe���W
	Out.pos = mul(g_viewProjMat,	Out.pos);
	
	//	�J�����ւ̃x�N�g��
	Out.viewDir = normalize(g_light.viewPos - Out.wPos);

	//	���C�g�ւ̃x�N�g��
	Out.lightDir = normalize(g_light.lightPos - Out.wPos);

	//	�e�N�X�`�����W
	Out.tex = In.tex;

	//	�g�U���𔽉f
	Out.color.rgb = In.color.rgb * DiffuseColor(Out.wNormal);

	//	�����𔽉f
	Out.color.rgb += g_light.ambientColor.rgb;

	Out.color.a = 1.0f;

	
	return Out;
}

