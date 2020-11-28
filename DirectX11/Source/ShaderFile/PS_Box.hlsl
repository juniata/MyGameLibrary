//-------------------------------------------------------------
//	ライティング情報
//-------------------------------------------------------------
cbuffer Lighting : register(b0) {
	struct tagLight {
		float4 ambient;
		float4 diffuse;
		float3 pos;
		float3 padding;
	}g_light;
};

//-------------------------------------------------------------
//	VSから送られてくる情報
//-------------------------------------------------------------
struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float4 color	: COLOR;
	float3 normal	: NORMAL;
	float3 wPos		: POSITION0;
	float3 wNormal	: NORMAL1;
};


//-------------------------------------------------------------
//	スペキュラを算出
//-------------------------------------------------------------
inline float3 Specular(float3 lightDir, float3 viewDir, float3 wNormal, float power)
{
	//	ライトへのベクトルと視点へのベクトルを足してハーフベクトルを算出
	float3 halfVec = normalize(viewDir + lightDir);

	//	ハーフベクトルと法線ベクトルを内積
	float specularPower = saturate(dot(wNormal, halfVec));

	//	スペキュラの強さ決めて色を算出する
	return pow(specularPower, power);

}

//-------------------------------------------------------------
//	拡散反射
//-------------------------------------------------------------
inline float3 Diffuse(float3 lightDir, float3 wNormal, float3 rgb)
{
	return saturate(dot(lightDir, wNormal)) * rgb;
}

//-------------------------------------------------------------
//	環境光
//-------------------------------------------------------------
inline float3 Ambient(float3 rgb)
{
	return rgb * g_light.ambient.rgb;
}
//-------------------------------------------------------------
//	ジオメトリーシェーダー
//-------------------------------------------------------------
float4 PS_Main(PS_INPUT In) : SV_TARGET
{
	clip(In.color.a - 0.1f);

	float4 outColor = In.color;

	// ライトへのベクトル
	float3 lightDir = g_light.pos - In.wPos;


	outColor.rgb = Diffuse(lightDir, In.wNormal, outColor.rgb);
	outColor.rgb = Ambient(outColor.rgb);
	//	色を出力
	return  outColor;
}