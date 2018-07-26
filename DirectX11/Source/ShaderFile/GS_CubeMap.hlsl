//-------------------------------------------------------------
//	�L���[�u�}�b�v�Ɋւ���s��
//-------------------------------------------------------------
cbuffer MatrixForTheCubeMapCBuffer	: register(b3){

	//	��D��
	column_major float4x4	viewMat[6];		//	view
	column_major float4x4	projMat;		//	projection
};


//-------------------------------------------------------------
//	���_�V�F�[�_�[���瑗���Ă�����
//-------------------------------------------------------------
typedef struct GS_INPUT
{
	float4 pos		: SV_POSITION;
}GS_OUTPUT;


//-------------------------------------------------------------
//	�s�N�Z���V�F�[�_�[�ɑ�����
//-------------------------------------------------------------
struct PS_OUTPUT
{
	float4 pos				 : SV_POSITION;
	uint   renderTargetIndex : SV_RenderTargetArrayIndex;
};


//-------------------------------------------------------------
//	�W�I���g���V�F�[�_�[�̏���
//-------------------------------------------------------------
[maxvertexcount(16)] // 3���_*6��
void GS_Main(
	triangle GS_INPUT In[3],
	inout TriangleStream< PS_OUTPUT > cubeMapStream
	)
{
	//	6�ʂ��v�Z����
	for (int i = 0; i < 6; ++i){

		//	�e�ʖ��ɒ��_���W���v�Z���ĕ`����ݒ�
		PS_OUTPUT Out; 

		// ���̃v���~�e�B�u��`�悷��`��^�[�Q�b�g
		Out.renderTargetIndex = i;

		for (int j = 0; j < 3; ++j){
			Out.pos = mul(viewMat[i],	In[j].pos);	//	�r���[�ϊ�
			Out.pos = mul(projMat,		In[j].pos);	//	�ˉe�ϊ�
			cubeMapStream.Append(Out);	//	���_�o��
		}

		//	1�X�g���b�v�I��
		cubeMapStream.RestartStrip();
	}
}