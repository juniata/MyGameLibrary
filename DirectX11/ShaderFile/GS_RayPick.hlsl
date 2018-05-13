
//-------------------------------------------------------------
//	���_�V�F�[�_�[���瑗���Ă�����
//-------------------------------------------------------------
struct GS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	float3 wPos		: WPOS;
	float3 wNormal  : NORMAL;
	float3 viewDir  : VIEWDIR;
	float3 lightDir : LIGHTDIR;
};



//-------------------------------------------------------------
//	�s�N�Z���V�F�[�_�[�ɑ�����
//-------------------------------------------------------------
struct PS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	float3 wPos		: WPOS;
	float3 wNormal  : NORMAL;
	float3 viewDir  : VIEWDIR;
	float3 lightDir : LIGHTDIR;

	float4 outpos	 : OUT_POS;	//	�����������W(x,y,z�͍��W,w��1�Ȃ�hit)
	float3 outnormal : OUT_NORMAL;	//	�@��
};

//-------------------------------------------------------------
//	���C�s�b�N�ɕK�v�ȏ��
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){
	float4 rayPos;			//	16byte alignment�̂���float4�ɂ���
	float3 rayVec;			//	���C�̌���
	float  rayDist;			//	���C�̒���
}

//-------------------------------------------------------------
//	�ʖ@�����v�Z����
//-------------------------------------------------------------
inline float3 CalcuFaceNormal(float3 vecA, float3 vecB, float3 vecC)
{
	float3 vecBA = vecB - vecA;
	float3 vecCB = vecC - vecB;

	return normalize(cross(vecBA, vecCB));
}

//-------------------------------------------------------------
//	�f�^�[�~�i���g f3 f3 f3
//-------------------------------------------------------------
float det(float3 v1, float3 v2, float3 v3)
{
	return  (
		(v1.x * v2.y * v3.z) +
		(v1.y * v2.z * v3.x) +
		(v1.z * v2.x * v3.y) -
		(v1.x * v2.z * v3.y) -
		(v1.y * v2.x * v3.z) -
		(v1.z * v2.y * v3.x)
		);
}

//-------------------------------------------------------------
//	���C�s�b�N�i�N�������̌������g���������j
//	
//	T����_�Ƃ����
//	T = v1  + edge1 * u + edge2 * v
//	T = origin + ray * t
//  edge1 * u + edge2 * v - ray * t = origin - v1
//	��L�̎����N�������̌�����p����u,v,t�̒l�����߂�
//	0 <= u <= 1,	0 <= v <= 1,	0 <= u + v <= 1�𖞂����Ă����
//	�O�p�`���ђʂ���
//
//-------------------------------------------------------------
bool RayPick(out float3 hitPos,float3 v1,float3 v2,float3 v3)
{
	//	���C�̋t�x�N�g��
	float3 invRay = -normalize(rayVec);
	float3 origin = rayPos.xyz;
	float3 edge1 = v2 - v1;
	float3 edge2 = v3 - v1;

	//	�N�������̌����̕���
	float denominator = det(edge1, edge2, invRay);
		
	//	���C�����ʂƕ��s�łȂ������`�F�b�N
	if (denominator > 0)
	{
		//	���_�P���烌�C���W�ւ̃x�N�g��
		float3 orgVec = origin - v1;

		//	u�����߂�
		float u = det(orgVec, edge2, invRay);

		if ((u >= 0) && (u <= 1 * denominator))
		{
			//	v�����߂�
			float v = det(edge1, orgVec, invRay);

			if ((v >= 0) && (v + u) <= 1 * denominator)
			{
				//	t�����߂�
				float t = det(edge1, edge2, orgVec) / denominator;

				//	���C�����������ꏊ
				hitPos = origin + (-invRay) * t;
				return true;
			}
		}
	}
	return false;
}

//-------------------------------------------------------------
//	�W�I���g���V�F�[�_�[
//-------------------------------------------------------------
[maxvertexcount(3)]
void GS_Main(triangle GS_INPUT input[3], inout TriangleStream<PS_OUTPUT> TriStream)
{
	PS_OUTPUT output;

	float3 hitPos = 0;

	//	���C�s�b�N����
	bool bRayhit = RayPick(hitPos,input[0].wPos, input[1].wPos, input[2].wPos);

	//	�ʖ@��
	float3 wFaceNormal = CalcuFaceNormal(input[0].wPos, input[1].wPos, input[2].wPos);

	for (int i = 0; i < 3; ++i){
		output.outpos	= 0;
		output.pos		= input[i].pos;
		output.tex		= input[i].tex;
		output.color	= input[i].color;
		output.wPos		= input[i].wPos;
		output.wNormal	= input[i].wNormal;
		output.viewDir	= input[i].viewDir;
		output.lightDir = input[i].lightDir;

		//	���C���������Ă��Ȃ���΁A���_�F�����ɂ���
		if (bRayhit == true){
			output.color.rgb	= 0.0f;
			output.outpos		= float4(hitPos, 1);
			output.outnormal	= wFaceNormal;
		}
		

		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}
