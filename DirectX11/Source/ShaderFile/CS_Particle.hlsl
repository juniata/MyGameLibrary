
//	�ǂݎ���p��UAV
StructuredBuffer<float3> InputBuffer : register(t0);

//	�ǂݏ����\��UAV
RWStructuredBuffer<float3> OutputBuffer : register(u0);

/*	
	�p�[�e�B�N���̈ړ��������s��
*/
[numthreads(128, 1, 1)]
void CS(uint3 did : SV_DispatchThreadID)
{
	float3 pos = OutputBuffer[did.x];

	//	���W���X�V
	OutputBuffer[did.x] = pos + float3(1, 0, 0);


}

