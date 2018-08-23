
//-------------------------------------------------------------
//	ビューに関する行列
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0) {
	float4x4	g_viewMat;
	float4x4	g_projMat;
	float4x4	g_viewProjMat;
};

//-------------------------------------------------------------
//	オブジェクトのワールド座標
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1) {
	float4x4	g_worldMat;
};

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

	Out.color	= In.color;
	Out.normal	= In.normal;

	Out.pos = mul(g_worldMat, In.pos);
	Out.pos = mul(g_viewProjMat, Out.pos);

	return Out;
}

