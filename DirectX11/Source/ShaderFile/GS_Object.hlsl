
//-------------------------------------------------------------
//	���_�V�F�[�_�[���瑗���Ă�����
//-------------------------------------------------------------
struct GS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};


//-------------------------------------------------------------
//	�s�N�Z���V�F�[�_�[�ɑ�����
//-------------------------------------------------------------
struct PS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float4 color	: COLOR;
};

//-------------------------------------------------------------
//	�W�I���g���V�F�[�_�[
//-------------------------------------------------------------
[maxvertexcount(3)]
void GS_Main(triangle GS_INPUT input[3], inout TriangleStream<PS_OUTPUT> TriStream)
{
	PS_OUTPUT output;

	for (int i = 0; i < 3; ++i){
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}
