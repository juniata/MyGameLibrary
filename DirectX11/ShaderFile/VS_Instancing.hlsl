//-------------------------------------------------------------
//	�r���[�Ɋւ���s��
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){

	//	��D��
	column_major float4x4	viewMat;
	column_major float4x4	projMat;
	column_major float4x4	viewProjMat;

};

//-------------------------------------------------------------
//	�I�u�W�F�N�g�̃��[���h���W
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1){

	//	��D��
	column_major float4x4	worldMat;

};

//-------------------------------------------------------------
//	CPU���瑗���Ă�����
//-------------------------------------------------------------
typedef struct VS_BASIC_IN
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	
}VS_IN;

//-------------------------------------------------------------
//	���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ɑ�����
//-------------------------------------------------------------
typedef struct VS_BASIC_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;

}VS_OUT;

//-------------------------------------------------------------
//	���_�V�F�[�_�[
//-------------------------------------------------------------
VS_OUT VS(VS_BASIC_IN In,uint instID : SV_InstanceID)
{
	VS_OUT Out;

	Out.pos = In.pos;
	Out.pos.x += 10.0 * (instID % 10000);
	Out.pos = mul(worldMat, Out.pos);
	Out.pos = mul(viewProjMat, Out.pos);
	Out.tex = In.tex;
	Out.normal = normalize(mul((float3x3)worldMat, In.normal));
	return Out;
}

