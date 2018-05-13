
//	読み取り専用のUAV
StructuredBuffer<float3> InputBuffer : register(t0);

//	読み書き可能なUAV
RWStructuredBuffer<float3> OutputBuffer : register(u0);

/*	
	パーティクルの移動処理を行う
*/
[numthreads(128, 1, 1)]
void CS(uint3 did : SV_DispatchThreadID)
{
	float3 pos = OutputBuffer[did.x];

	//	座標を更新
	OutputBuffer[did.x] = pos + float3(1, 0, 0);


}

