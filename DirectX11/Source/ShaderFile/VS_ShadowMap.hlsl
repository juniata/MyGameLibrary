
//-------------------------------------------------------------
//	�r���[�Ɋւ���s��
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){

	//	��D��
	column_major float4x4	viewMat;		//	view
	column_major float4x4	projMat;		//	projection
	column_major float4x4	viewProjMat;	//	view * projection
};


//-------------------------------------------------------------
//	�I�u�W�F�N�g�̃��[���h���W
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1){

	//	��D��
	column_major float4x4	worldMat;
};

//-------------------------------------------------------------
//	���C�g���猩���s��
//-------------------------------------------------------------
cbuffer MatrixForTheLightCBuffer : register(b2)
{
	column_major float4x4	lightViewProjection;

}

//-------------------------------------------------------------
//	���_�V�F�[�_�[�ɑ����Ă�����
//-------------------------------------------------------------
typedef struct VS_BASIC_IN
{
	float4 pos : POSITION;
}VS_IN;

typedef struct VS_BASIC_OUT
{
	float4 pos : SV_POSITION;
}VS_OUT;


VS_OUT VS_Main(VS_BASIC_IN In)
{
	VS_OUT Out;

	Out.pos = mul(worldMat,In.pos);
	Out.pos = mul(lightViewProjection, In.pos);
	return Out;
}

