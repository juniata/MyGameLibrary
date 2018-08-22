Texture2D g_textureD : register(t0);	//	通常テクスチャ

SamplerState g_sampler : register(s0);

typedef struct VS_BASIC_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
}VS_OUT;


float4 PS_Main(VS_BASIC_OUT In) : SV_TARGET
{
	return g_textureD.Sample(g_sampler, In.tex);
}