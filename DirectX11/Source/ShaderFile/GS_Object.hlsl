
//-------------------------------------------------------------
//	頂点シェーダーから送られてくる情報
//-------------------------------------------------------------
struct GS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};


//-------------------------------------------------------------
//	ピクセルシェーダーに送る情報
//-------------------------------------------------------------
struct PS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float4 color	: COLOR;
};

//-------------------------------------------------------------
//	ジオメトリシェーダー
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
