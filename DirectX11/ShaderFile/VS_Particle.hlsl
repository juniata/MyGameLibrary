//-------------------------------------------------------------
//	ParticleSystem���
//-------------------------------------------------------------
cbuffer ParticleSystemInfo : register(b1)
{
	//	��D��
	row_major float4x4 g_particlySystemInfoMat;
	/*
	   [0][0] ~ [0][2]  : �G�~�b�^�̍��W
	   [0][3]			: �p�[�e�B�N���̑傫��

	   [1][0] ~ [1][2]  : �G�~�b�^�[�̌���
	   [1][3]			: �p�[�e�B�N���̐�(int)

	   [2][0] ~ [2][2]  : �G�~�b�^�[�̑傫��
	   [2][3]			: ���݂̃t���[��(int)
		
       [3][0] ~ [3][3]  : �p�[�e�B�N���̐F
	*/
}

//-------------------------------------------------------------
//	CPU���瑗���Ă��钸�_���
//-------------------------------------------------------------
struct InputVS
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
};

//-------------------------------------------------------------
//	VertexShader����GeometryShader�ɑ�����
//-------------------------------------------------------------
struct OUTPUT_GS
{
	float4 pos			: SV_POSITION;
	float2 tex			: TEXCOORD;
	float4 color		: COLOR;
	int frame : COLOR1;
	int instID : COLOR2;

};
//--------------------------------------------------------------
//
//	g_particlySystemInfoMat���炳�܂��܂ȏ����擾
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//	�G�~�b�^�[�̍��W���擾
//--------------------------------------------------------------
inline float3 GetEmitterPosFormMatrix()
{
	return g_particlySystemInfoMat[0].xyz;
}

//--------------------------------------------------------------
//	�G�~�b�^�[�̌������擾
//--------------------------------------------------------------
inline float3 GetEmitterAngleFormMatrix()
{
	return g_particlySystemInfoMat[1].xyz;
}

//--------------------------------------------------------------
//	�G�~�b�^�[�̑傫�����擾
//--------------------------------------------------------------
inline float3 GetEmitterScaleFormMatrix()
{
	return g_particlySystemInfoMat[2].xyz;
}

//--------------------------------------------------------------
//	�p�[�e�B�N���̑傫�����擾
//--------------------------------------------------------------
inline float GetParticleScaleFromMatrix()
{
	return g_particlySystemInfoMat[0].w;
}

//--------------------------------------------------------------
//	�p�[�e�B�N���̐����擾
//--------------------------------------------------------------
inline int GetParticleCountFromMatrix()
{
	return (int)g_particlySystemInfoMat[1].w;
}
//--------------------------------------------------------------
//	���݂̃t���[�������擾
//--------------------------------------------------------------
inline int GetNowFrameFromMatrix()
{
	return (int)g_particlySystemInfoMat[2].w;
}

//--------------------------------------------------------------
//	�p�[�e�B�N���̐F�����擾
//--------------------------------------------------------------
inline float4 GetParticleColorFromMatrix()
{
	return g_particlySystemInfoMat[3];
}


//--------------------------------------------------------------
//
//	���[���h���W�ɕϊ�����
//
//--------------------------------------------------------------
inline float3 WorldTransformation(float3 pos)
{
	float3 outPos;

	//	�p�[�e�B�N���̑傫�����|����
	outPos = pos * GetParticleScaleFromMatrix();

	//	�G�~�b�^�[�̍��W���p�[�e�B�N���̍��W�ɑ���
	outPos += GetEmitterPosFormMatrix(); 

	return outPos;
}

//--------------------------------------------------------------
//
//	���_�V�F�[�_�[
//
//--------------------------------------------------------------
OUTPUT_GS VS(InputVS In, uint instID : SV_InstanceID)
{
	OUTPUT_GS Out;
	Out.pos = In.pos;
	Out.tex = In.tex;


	//	���[���h�ϊ�����
	Out.pos.xyz = WorldTransformation(Out.pos.xyz);

	//	�p�[�e�B�N���̐F���擾���A�ݒ肷��
	Out.color = GetParticleColorFromMatrix();

	//	���݂̃t���[����GS�ɑ���
	Out.frame = GetNowFrameFromMatrix();
	
	//	�C���X�^���X�ԍ���GS�ɑ���
	Out.instID = (int)instID;

	return Out;
}

