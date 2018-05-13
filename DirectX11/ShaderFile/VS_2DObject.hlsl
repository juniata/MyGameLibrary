//-------------------------------------------------------------
//	CPUから送られてくる頂点情報
//-------------------------------------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
};

//-------------------------------------------------------------
//	PSに送る頂点情報
//-------------------------------------------------------------
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

//-------------------------------------------------------------
//	頂点シェーダー処理
//-------------------------------------------------------------
VS_OUT VS_Main(VS_INPUT In)
{
	VS_OUT Out;

	Out.pos = In.pos;
	Out.tex = In.tex;

	return Out;
}

