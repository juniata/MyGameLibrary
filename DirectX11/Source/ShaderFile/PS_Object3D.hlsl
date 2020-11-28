//-------------------------------------------------------------
//	�e�N�X�`��
//-------------------------------------------------------------
Texture2D g_textureD : register(t0);	//	�ʏ�e�N�X�`��
//Texture2D g_textureN : register(t1);	//	�@���e�N�X�`��
//Texture2D g_textureS : register(t2);	//	�X�y�L�����e�N�X�`��
//Texture2D g_textureH : register(t3);	//	�����e�N�X�`��
//Texture2D g_textures[4] : register(t0);

//-------------------------------------------------------------
//	�e�N�X�`���T���v���[
//-------------------------------------------------------------
SamplerState g_sampler : register(s0);

//-------------------------------------------------------------
//	���C�e�B���O���
//-------------------------------------------------------------
cbuffer Lighting : register(b0) {
	struct tagLight {
		float4 ambient;
		float4 diffuse;
		float3 pos;
		float3 padding;
	}g_light;
};


//-------------------------------------------------------------
//	���_�V�F�[�_�[���瑗���Ă�����
//-------------------------------------------------------------
struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
	float2 tex		: TEXCOORD;

};
//
////-------------------------------------------------------------
////	�X�y�L�������Z�o
////-------------------------------------------------------------
//inline float3 Specular(float3 lightDir, float3 viewDir, float3 wNormal,float power)
//{
//	//	���C�g�ւ̃x�N�g���Ǝ��_�ւ̃x�N�g���𑫂��ăn�[�t�x�N�g�����Z�o
//	float3 halfVec = normalize(viewDir + lightDir);
//
//	//	�n�[�t�x�N�g���Ɩ@���x�N�g�������
//	float specularPower = max(dot(wNormal, halfVec), 0);
//
//	//	�X�y�L�����̋������߂ĐF���Z�o����
//	return pow(specularPower, power) * g_light.specular.rgb;
//
//}
//
////-------------------------------------------------------------
////	�ʏ�e�N�X�`���̐F���擾
////-------------------------------------------------------------
//float4 GetDecalTextureColor(float2 tex)
//{
//	return g_textures[0].Sample(g_sampler, tex);
//}
//
////-------------------------------------------------------------
////	�@���e�N�X�`���̐F���擾
////-------------------------------------------------------------
//float4 GetNormalTextureColor(float2 tex)
//{
//	return g_textures[1].Sample(g_sampler, tex);
//}
//
////-------------------------------------------------------------
////	�X�y�L�����e�N�X�`���̐F���擾
////-------------------------------------------------------------
//float4 GetSpecularTextureColor(float2 tex)
//{
//	return g_textures[3].Sample(g_sampler, tex);
//}
//
////-------------------------------------------------------------
////	�����e�N�X�`���̐F���擾
////-------------------------------------------------------------
//float4 GetHeightTextureColor(float2 tex)
//{
//	return g_textures[3].Sample(g_sampler, tex);
//}

//-------------------------------------------------------------
//	�W�I���g���[�V�F�[�_�[
//-------------------------------------------------------------
float4 PS_Main(PS_INPUT In) : SV_TARGET
{
	//float4 outColor = GetDecalTextureColor(In.tex) * In.color;

	////	�X�y�L����
	//outColor.rgb += Specular(In.lightDir, In.viewDir, In.wNormal, 50);
	float4 outColor = g_textureD.Sample(g_sampler, In.tex);
	clip(outColor.a - 0.1f);

	//	�F���o��
	return  outColor;
}