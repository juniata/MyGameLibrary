//-------------------------------------------------------------
//	�p�[�e�B�N���̏��i���̏�񂪊i�[����Ă���j
//-------------------------------------------------------------
Buffer<float3>  g_particleResource : register(t0);

//-------------------------------------------------------------
//	�����e�N�X�`��(100*100 -1~1)
//-------------------------------------------------------------
Buffer<float3>  g_randomResource : register(t1);


//-------------------------------------------------------------
//	�p�[�e�B�N���̍��W���i�[
//-------------------------------------------------------------
Buffer<float3>  g_particlePos : register(t2);

//-------------------------------------------------------------
//	�r���[�Ɋւ���s��
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){

	//	��D��
	row_major float4x4	g_viewMat;
	row_major float4x4	g_projMat;
	row_major float4x4	g_viewProjMat;
};


//-------------------------------------------------------------
//	VertexShader����GeometryShader�ɑ����Ă�����
//-------------------------------------------------------------
struct INPUT_GS
{
	float4 pos			: SV_POSITION;
	float2 tex			: TEXCOORD;
	float4 color		: COLOR;
	int frame			: COLOR1;
	int instID			: COLOR2;

};

//-------------------------------------------------------------
//	GeometryShader����PixelShader�ɑ�����
//-------------------------------------------------------------
struct OUTPUT_PS
{
	float4 pos   : SV_POSITION;
	float2 tex   : TEXCOORD;
	float4 color : COLOR;

};



//--------------------------------------------------------------
//
//	�r���{�[�h�s����쐬
//
//--------------------------------------------------------------
//inline float4x4 CreateBillBord()
//{
//}

//-------------------------------------------------------------
//	GeometryShader
//-------------------------------------------------------------
[maxvertexcount(4)]
void GS(triangle INPUT_GS In[3],  inout TriangleStream< OUTPUT_PS > TriStream)
{
	OUTPUT_PS Out;

	//	�C���X�^���XID
	int instID = In[0].instID;

	//	���\�[�X����p�[�e�B�N���𐶐����鎞�Ԃ��擾����
	int createParticleFrame = (int)g_particleResource.Load(instID).x;

	//	���\�[�X����p�[�e�B�N�������ł��鎞�Ԃ��擾����
	int deleteParticleFrame = (int)g_particleResource.Load(instID).y;

	//	���݂̃t���[�����擾
	int nowFrame = In[0].frame;

	//	���݂̃t���[�������A�����t���[�����ɖ����Ȃ��ꍇreturn
	if (nowFrame < createParticleFrame){
		return;
	}

	//	���݂̃t���[�����A���Ńt���[�����𒴂����ꍇreturn
	if (nowFrame >deleteParticleFrame){
		return;
	}

	float3 randomPos = g_randomResource.Load(instID % uint(100 * 100));
	float3 pos = g_particlePos.Load(instID);

	for (int i = 0; i < 3; ++i){
		Out.pos = In[i].pos;
		Out.tex = In[i].tex;
		Out.color = In[i].color;
	//	Out.pos.xyz += pos;
		Out.pos.xyz += pos + randomPos;
		
		//	���e�ϊ����s��
		Out.pos = mul(Out.pos, g_viewProjMat);

		//	���_��ǉ�
		TriStream.Append(Out);
	}
	TriStream.RestartStrip();
}
