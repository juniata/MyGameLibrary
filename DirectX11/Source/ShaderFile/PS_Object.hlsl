typedef struct VS_BASIC_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
}VS_OUT;


float4 PS_Main(VS_BASIC_OUT In) : SV_TARGET
{
	return In.color;
}