Texture2D g_tex : register(t0);
SamplerState g_sampler : register(s0);

typedef struct VS_BASIC_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;

}VS_OUT;


float4 PS(VS_BASIC_OUT In) : SV_TARGET
{
	float4 color = g_tex.Sample(g_sampler, In.tex);
	color *= In.color;
	clip(color.a - 0.0001f);
	return color;
}