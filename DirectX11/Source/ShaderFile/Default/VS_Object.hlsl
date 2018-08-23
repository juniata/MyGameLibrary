
//-------------------------------------------------------------
//	�r���[�Ɋւ���s��
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0) {
	float4x4	g_viewMat;
	float4x4	g_projMat;
	float4x4	g_viewProjMat;
};

//-------------------------------------------------------------
//	�I�u�W�F�N�g�̃��[���h���W
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1) {
	float4x4	g_worldMat;
};

//-------------------------------------------------------------
//	CPU���瑗���Ă��钸�_���
//-------------------------------------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

//-------------------------------------------------------------
//	PS�ɑ��钸�_���
//-------------------------------------------------------------
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

//-------------------------------------------------------------
//	���_�V�F�[�_�[����
//-------------------------------------------------------------
VS_OUT VS_Main(VS_INPUT In)
{
	VS_OUT Out;

	Out.color	= In.color;
	Out.normal	= In.normal;

	Out.pos = mul(g_worldMat, In.pos);
	Out.pos = mul(g_viewProjMat, Out.pos);

	return Out;
}

