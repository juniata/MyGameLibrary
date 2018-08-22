//-------------------------------------------------------------
//	CPUから送られてくる頂点情報
//-------------------------------------------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

//-------------------------------------------------------------
//	PSに送る頂点情報
//-------------------------------------------------------------
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

//-------------------------------------------------------------
//	頂点シェーダー処理
//-------------------------------------------------------------
VS_OUT VS_Main(VS_INPUT In)
{
	VS_OUT Out;

	Out.pos		= In.pos;
	Out.color	= In.color;
	Out.normal	= In.normal;

	return Out;
}

