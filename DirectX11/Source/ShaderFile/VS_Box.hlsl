
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
//	ライティング情報
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
//	カメラ情報
//-------------------------------------------------------------
cbuffer View : register(b3) {
	struct tagView {
		float3 pos;
		float1 padding;
	}g_view;
}

//-------------------------------------------------------------
//	CPUから送られてくる頂点情報
//-------------------------------------------------------------
struct VS_INPUT
{
	float4 pos		: POSITION;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

//-------------------------------------------------------------
//	PSに送る頂点情報
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
//	頂点シェーダー処理
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

