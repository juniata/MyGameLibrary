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

	Out.pos		= In.pos;
	Out.color	= In.color;
	Out.normal	= In.normal;

	return Out;
}

