
//-------------------------------------------------------------
//	���_�V�F�[�_�[���瑗���Ă�����
//-------------------------------------------------------------
struct GS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	float3 wPos		: WPOS;
	float3 wNormal  : NORMAL;
	float3 viewDir  : VIEWDIR;
	float3 lightDir : LIGHTDIR;
};


//-------------------------------------------------------------
//	�s�N�Z���V�F�[�_�[�ɑ�����
//-------------------------------------------------------------
struct PS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	float3 wPos		: WPOS;
	float3 wNormal  : NORMAL;
	float3 viewDir  : VIEWDIR;
	float3 lightDir : LIGHTDIR;
};

//-------------------------------------------------------------
//	�W�I���g���V�F�[�_�[
//-------------------------------------------------------------
[maxvertexcount(3)]
void GS_Main(triangle GS_INPUT input[3], inout TriangleStream<PS_OUTPUT> TriStream)
{
	PS_OUTPUT output;

	for (int i = 0; i < 3; ++i){
		output.pos		= input[i].pos;
		output.tex		= input[i].tex;
		output.color	= input[i].color;
		output.wPos		= input[i].wPos;
		output.wNormal	= input[i].wNormal;
		output.viewDir	= input[i].viewDir;
		output.lightDir = input[i].lightDir;
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}
