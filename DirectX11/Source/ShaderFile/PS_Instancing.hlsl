//-------------------------------------------------------------
//	�e�N�X�`��
//-------------------------------------------------------------
Texture2D textureD : register(t0);	//	�ʏ�e�N�X�`��
Texture2D textureN : register(t1);	//	�@���e�N�X�`��
Texture2D textureS : register(t2);	//	�X�y�L�����e�N�X�`��
Texture2D textureH : register(t3);	//	�����e�N�X�`��


//-------------------------------------------------------------
//	�e�N�X�`���T���v���[
//-------------------------------------------------------------
SamplerState g_sampler : register(s0);


//-------------------------------------------------------------
//	���C�g���
//-------------------------------------------------------------
cbuffer LightBuffer : register(b0)
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 lightDirection;
};



//-------------------------------------------------------------
//	�W�I���g���[�V�F�[�_�[���瑗���Ă�����
//-------------------------------------------------------------
typedef struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
}PS_OUTPUT;


//-------------------------------------------------------------
//	�W�I���g���[�V�F�[�_�[
//-------------------------------------------------------------
float4 PS(PS_INPUT In) : SV_TARGET
{
	float4 l_tex_color = textureD.Sample(g_sampler, In.tex);

	clip(l_tex_color.a - 0.1);

	return  l_tex_color;
}