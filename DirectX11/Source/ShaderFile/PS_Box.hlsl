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
//	VS���瑗���Ă�����
//-------------------------------------------------------------
struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float4 color	: COLOR;
	float3 normal	: NORMAL;
	float3 wPos		: POSITION0;
	float3 wNormal	: NORMAL1;
};


//-------------------------------------------------------------
//	�X�y�L�������Z�o
//-------------------------------------------------------------
inline float3 Specular(float3 lightDir, float3 viewDir, float3 wNormal, float power)
{
	//	���C�g�ւ̃x�N�g���Ǝ��_�ւ̃x�N�g���𑫂��ăn�[�t�x�N�g�����Z�o
	float3 halfVec = normalize(viewDir + lightDir);

	//	�n�[�t�x�N�g���Ɩ@���x�N�g�������
	float specularPower = saturate(dot(wNormal, halfVec));

	//	�X�y�L�����̋������߂ĐF���Z�o����
	return pow(specularPower, power);

}

//-------------------------------------------------------------
//	�g�U����
//-------------------------------------------------------------
inline float3 Diffuse(float3 lightDir, float3 wNormal, float3 rgb)
{
	return saturate(dot(lightDir, wNormal)) * rgb;
}

//-------------------------------------------------------------
//	����
//-------------------------------------------------------------
inline float3 Ambient(float3 rgb)
{
	return rgb * g_light.ambient.rgb;
}
//-------------------------------------------------------------
//	�W�I���g���[�V�F�[�_�[
//-------------------------------------------------------------
float4 PS_Main(PS_INPUT In) : SV_TARGET
{
	clip(In.color.a - 0.1f);

	float4 outColor = In.color;

	// ���C�g�ւ̃x�N�g��
	float3 lightDir = g_light.pos - In.wPos;


	outColor.rgb = Diffuse(lightDir, In.wNormal, outColor.rgb);
	outColor.rgb = Ambient(outColor.rgb);
	//	�F���o��
	return  outColor;
}