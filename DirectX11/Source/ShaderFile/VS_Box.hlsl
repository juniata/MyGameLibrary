
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
//	���C�e�B���O���
//-------------------------------------------------------------
cbuffer Lighting : register(b2) {
	struct tagLight {
		float4 ambient;
		float4 diffuse;
		float3 pos;
		float padding;
	}g_light;
};


//-------------------------------------------------------------
//	�J�������
//-------------------------------------------------------------
cbuffer View : register(b3) {
	struct tagView {
		float3 pos;
		float1 padding;
	}g_view;
}

//-------------------------------------------------------------
//	CPU���瑗���Ă��钸�_���
//-------------------------------------------------------------
struct VS_INPUT
{
	float4 pos		: POSITION;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

//-------------------------------------------------------------
//	PS�ɑ��钸�_���
//-------------------------------------------------------------
struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float4 color	: COLOR;
	float3 normal	: NORMAL;
	float3 wPos		: POSITION0;
	float3 wNormal	: NORMAL1;
};

//-------------------------------------------------------------
//	���_�V�F�[�_�[����
//-------------------------------------------------------------
VS_OUT VS_Main(VS_INPUT In)
{
	VS_OUT Out;

	float4 wPos		= mul(g_worldMat, In.pos);
	float3 wNormal	= normalize(mul(g_worldMat, In.normal));
	
	Out.wPos	= wPos;
	Out.wNormal = wNormal;
	Out.color	= In.color;
	Out.normal	= In.normal;
	Out.pos		= mul(g_viewProjMat, wPos);

	return Out;
}

