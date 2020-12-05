//-------------------------------------------------------------
//	���C�e�B���O���
//-------------------------------------------------------------
cbuffer Lighting : register(b2) {
	struct tagLight {
		float4 ambient;
		float4 diffuse;
		float3 pos;
		float padding;
	}g_light;
};


//-------------------------------------------------------------
//	�J�������
//-------------------------------------------------------------
cbuffer View : register(b3) {
	struct tagView {
		float3 pos;
		float1 padding;
	}g_view;
}


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


/// <summary>
/// ����
/// </summary>
float3 Ambient(float3 material)
{
	return material * g_light.ambient;
}

/// <summary>
/// �g�U����
/// </summary>
float3 Diffuse(float3 lightVec, float3 wNormal, float3 material)
{
	float d = saturate(dot(lightVec, wNormal));

	return float3(d * material *  g_light.diffuse.xyz);
}


/// <summary>
/// �X�؃L����
/// </summary>
float3 Specular(float3 lightVec, float3 viewVec, float3 wNormal, int power)
{
	//	���C�g�ւ̃x�N�g���Ǝ��_�ւ̃x�N�g���𑫂��ăn�[�t�x�N�g�����Z�o
	float3 halfVec = normalize(lightVec + viewVec);

	//	�n�[�t�x�N�g���Ɩ@���x�N�g�������
	float specularPower = saturate(dot(wNormal, halfVec));

	//	�X�y�L�����̋������߂ĐF���Z�o����
	return float3(1.0f, 1.0f, 1.0f) * pow(specularPower, power);
}
//-------------------------------------------------------------
//	�W�I���g���[�V�F�[�_�[
//-------------------------------------------------------------
float4 PS_Main(PS_INPUT In) : SV_TARGET
{
	clip(In.color.a - 0.1f);

	float4 outColor = In.color;

	float3 lightVec = normalize(g_light.pos - In.wPos);
	float3 viewVec = normalize(g_view.pos - In.wPos);

	float3 amibent = Ambient(In.color);
	float3 diffuse = Diffuse(lightVec, In.wNormal, In.color);
	float3 specular = Specular(lightVec, viewVec, In.wNormal, 10);
	outColor.rgb = saturate(amibent + diffuse + specular);

	return  outColor;
}