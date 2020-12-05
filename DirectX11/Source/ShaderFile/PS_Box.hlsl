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


/// <summary>
/// 環境光
/// </summary>
float3 Ambient(float3 material)
{
	return material * g_light.ambient;
}

/// <summary>
/// 拡散反射
/// </summary>
float3 Diffuse(float3 lightVec, float3 wNormal, float3 material)
{
	float d = saturate(dot(lightVec, wNormal));

	return float3(d * material *  g_light.diffuse.xyz);
}


/// <summary>
/// スぺキュラ
/// </summary>
float3 Specular(float3 lightVec, float3 viewVec, float3 wNormal, int power)
{
	//	ライトへのベクトルと視点へのベクトルを足してハーフベクトルを算出
	float3 halfVec = normalize(lightVec + viewVec);

	//	ハーフベクトルと法線ベクトルを内積
	float specularPower = saturate(dot(wNormal, halfVec));

	//	スペキュラの強さ決めて色を算出する
	return float3(1.0f, 1.0f, 1.0f) * pow(specularPower, power);
}
//-------------------------------------------------------------
//	ジオメトリーシェーダー
//-------------------------------------------------------------
float4 PS_Main(PS_INPUT In) : SV_TARGET
{
	clip(In.color.a - 0.1f);

	float4 outColor = In.color;

	float3 lightVec = normalize(g_light.pos - In.wPos);
	float3 viewVec = normalize(g_view.pos - In.wPos);

	float3 amibent = Ambient(In.color);
	float3 diffuse = Diffuse(lightVec, In.wNormal, In.color);
	float3 specular = Specular(lightVec, viewVec, In.wNormal, 10);
	outColor.rgb = saturate(amibent + diffuse + specular);

	return  outColor;
}