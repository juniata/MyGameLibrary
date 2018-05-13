//-------------------------------------------------------------
//	ビューに関する行列
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){

	//	列優先
	column_major float4x4	viewMat;
	column_major float4x4	projMat;
	column_major float4x4	viewProjMat;

};

//-------------------------------------------------------------
//	オブジェクトのワールド座標
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1){

	//	列優先
	column_major float4x4	worldMat;

};

//-------------------------------------------------------------
//	CPUから送られてくる情報
//-------------------------------------------------------------
typedef struct VS_BASIC_IN
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	
}VS_IN;

//-------------------------------------------------------------
//	頂点シェーダーからピクセルシェーダーに送る情報
//-------------------------------------------------------------
typedef struct VS_BASIC_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;

}VS_OUT;

//-------------------------------------------------------------
//	頂点シェーダー
//-------------------------------------------------------------
VS_OUT VS(VS_BASIC_IN In,uint instID : SV_InstanceID)
{
	VS_OUT Out;

	Out.pos = In.pos;
	Out.pos.x += 10.0 * (instID % 10000);
	Out.pos = mul(worldMat, Out.pos);
	Out.pos = mul(viewProjMat, Out.pos);
	Out.tex = In.tex;
	Out.normal = normalize(mul((float3x3)worldMat, In.normal));
	return Out;
}

