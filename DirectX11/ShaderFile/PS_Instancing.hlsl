//-------------------------------------------------------------
//	テクスチャ
//-------------------------------------------------------------
Texture2D textureD : register(t0);	//	通常テクスチャ
Texture2D textureN : register(t1);	//	法線テクスチャ
Texture2D textureS : register(t2);	//	スペキュラテクスチャ
Texture2D textureH : register(t3);	//	高さテクスチャ


//-------------------------------------------------------------
//	テクスチャサンプラー
//-------------------------------------------------------------
SamplerState g_sampler : register(s0);


//-------------------------------------------------------------
//	ライト情報
//-------------------------------------------------------------
cbuffer LightBuffer : register(b0)
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 lightDirection;
};



//-------------------------------------------------------------
//	ジオメトリーシェーダーから送られてくる情報
//-------------------------------------------------------------
typedef struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
}PS_OUTPUT;


//-------------------------------------------------------------
//	ジオメトリーシェーダー
//-------------------------------------------------------------
float4 PS(PS_INPUT In) : SV_TARGET
{
	float4 l_tex_color = textureD.Sample(g_sampler, In.tex);

	clip(l_tex_color.a - 0.1);

	return  l_tex_color;
}