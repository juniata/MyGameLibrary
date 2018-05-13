//-------------------------------------------------------------
//	テクスチャ
//-------------------------------------------------------------
//Texture2D g_textureD : register(t0);	//	通常テクスチャ
//Texture2D g_textureN : register(t1);	//	法線テクスチャ
//Texture2D g_textureS : register(t2);	//	スペキュラテクスチャ
//Texture2D g_textureH : register(t3);	//	高さテクスチャ
Texture2D g_textures[4] : register(t0);
//-------------------------------------------------------------
//	テクスチャサンプラー
//-------------------------------------------------------------
SamplerState g_sampler : register(s0);

//-------------------------------------------------------------
//	ライティング
//-------------------------------------------------------------
cbuffer Lighting : register(b0){
	struct tagLight{
		float4 specular;
		float3 viewPos;
		float3 dir;
		float2 padding;
	}g_light;
};

//-------------------------------------------------------------
//	ジオメトリーシェーダーから送られてくる情報
//-------------------------------------------------------------
struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	//	自身で算出した値
	float3 wPos		: WPOS;
	float3 wNormal  : NORMAL;
	float3 viewDir  : VIEWDIR;
	float3 lightDir : LIGHTDIR;
};

//-------------------------------------------------------------
//	スペキュラを算出
//-------------------------------------------------------------
inline float3 Specular(float3 lightDir, float3 viewDir, float3 wNormal,float power)
{
	//	ライトへのベクトルと視点へのベクトルを足してハーフベクトルを算出
	float3 halfVec = normalize(viewDir + lightDir);

	//	ハーフベクトルと法線ベクトルを内積
	float specularPower = max(dot(wNormal, halfVec), 0);

	//	スペキュラの強さ決めて色を算出する
	return pow(specularPower, power) * g_light.specular.rgb;

}

//-------------------------------------------------------------
//	通常テクスチャの色を取得
//-------------------------------------------------------------
float4 GetDecalTextureColor(float2 tex)
{
	return g_textures[0].Sample(g_sampler, tex);
}

//-------------------------------------------------------------
//	法線テクスチャの色を取得
//-------------------------------------------------------------
float4 GetNormalTextureColor(float2 tex)
{
	return g_textures[1].Sample(g_sampler, tex);
}

//-------------------------------------------------------------
//	スペキュラテクスチャの色を取得
//-------------------------------------------------------------
float4 GetSpecularTextureColor(float2 tex)
{
	return g_textures[3].Sample(g_sampler, tex);
}

//-------------------------------------------------------------
//	高さテクスチャの色を取得
//-------------------------------------------------------------
float4 GetHeightTextureColor(float2 tex)
{
	return g_textures[3].Sample(g_sampler, tex);
}

//-------------------------------------------------------------
//	ジオメトリーシェーダー
//-------------------------------------------------------------
float4 PS_Main(PS_INPUT In) : SV_TARGET
{
	float4 outColor = GetDecalTextureColor(In.tex) * In.color;

	//	スペキュラ
	outColor.rgb += Specular(In.lightDir, In.viewDir, In.wNormal, 50);

	//	色を出力
	return  outColor;
}