
//-------------------------------------------------------------
//	ビューに関する行列
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){
	float4x4	g_viewMat;
	float4x4	g_projMat;
	float4x4	g_viewProjMat;
};

//-------------------------------------------------------------
//	オブジェクトのワールド座標
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1){
	float4x4	g_worldMat;
};

//-------------------------------------------------------------
//	ライティング
//-------------------------------------------------------------
cbuffer Lighting : register(b3){
	struct tagLight{
		float4 ambientColor;
		float4 diffuseColor;
		float3 viewPos;
		float3 lightPos;	
		float3 lightDir;
		float3 padding;
	}g_light;
};

//-------------------------------------------------------------
//	頂点シェーダーに送られてくる情報
//-------------------------------------------------------------
struct VS_INPUT
{
	float3 pos		: POSITION;
	float2 tex		: TEXCOORD;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};


//-------------------------------------------------------------
//	ジオメトリシェーダーに送る情報
//-------------------------------------------------------------
struct VS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	//	自身で算出して送る変数
	float3 wPos		: WPOS;		//	頂点座標をワールド座標にしたもの
	float3 wNormal  : NORMAL;	//	頂点法線をワールド座標にしたもの
	float3 viewDir  : VIEWDIR;	//	ワールド座標の頂点からカメラへのベクトル
	float3 lightDir : LIGHTDIR;	//	ワールド座標の頂点からライトへのベクトル
};

//-------------------------------------------------------------
//	拡散光を算出
//-------------------------------------------------------------
inline float3 DiffuseColor(float3 wNormal)
{
	return (pow(dot(wNormal, -g_light.lightDir), 0) * g_light.diffuseColor).rgb;
}

//-------------------------------------------------------------
//	頂点シェーダー
//-------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT In)
{
	VS_OUTPUT Out;

	//	3x3のワールド行列
	float3x3 wMat = (float3x3)g_worldMat;


	Out.pos = mul(g_worldMat, float4(In.pos, 1));
	//Out.pos		= float4(mul(g_worldMat, In.pos),1);
	Out.wPos	= Out.pos.xyz;
	Out.pos		= mul(g_viewProjMat,	Out.pos);
	Out.tex		= In.tex;

	//	カメラへのベクトル
	Out.viewDir = normalize(g_light.viewPos - Out.wPos);

	//	ライトへのベクトル
	Out.lightDir = normalize(g_light.lightPos - Out.wPos);

	//	法線をワールド座標へ(座標はいらないので3x3matで計算)
	Out.wNormal = mul(wMat, In.normal);

	//	拡散光を反映
	Out.color.rgb =  In.color.rgb * DiffuseColor(Out.wNormal);

	//	環境光を反映
	Out.color.rgb += g_light.ambientColor.rgb;

	//	頂点色の非透明度を1にして出力
	Out.color.a = 1.0f;
	return Out;
}

