float2 perScreen : register(b0);

//-------------------------------------------------------------
//	CPUから送られてくる頂点情報
//-------------------------------------------------------------
struct VS_INPUT
{
	float4 pos			: POSITION;
	float2 tex			: TEXCOORD;
	float3 instancePos	: INSTANCE_POS;
};

//-------------------------------------------------------------
//	PSに送る頂点情報
//-------------------------------------------------------------
struct VS_OUT
{
	float4 pos			: SV_POSITION;
	float2 tex			: TEXCOORD;
};

//-------------------------------------------------------------
//	頂点シェーダー処理
//-------------------------------------------------------------
VS_OUT VS_Main(VS_INPUT In)
{
	VS_OUT Out;

	
	Out.pos.x = In.pos.x + (In.instancePos.x * perScreen.x);
	Out.pos.y = In.pos.y - (In.instancePos.y * perScreen.y);
	Out.pos.z = In.pos.z;
	Out.pos.w = 1.0f;

	Out.tex = In.tex;

	return Out;
}

