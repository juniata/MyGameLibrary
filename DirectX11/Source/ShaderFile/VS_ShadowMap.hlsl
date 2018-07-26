
//-------------------------------------------------------------
//	ビューに関する行列
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){

	//	列優先
	column_major float4x4	viewMat;		//	view
	column_major float4x4	projMat;		//	projection
	column_major float4x4	viewProjMat;	//	view * projection
};


//-------------------------------------------------------------
//	オブジェクトのワールド座標
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1){

	//	列優先
	column_major float4x4	worldMat;
};

//-------------------------------------------------------------
//	ライトから見た行列
//-------------------------------------------------------------
cbuffer MatrixForTheLightCBuffer : register(b2)
{
	column_major float4x4	lightViewProjection;

}

//-------------------------------------------------------------
//	頂点シェーダーに送られてくる情報
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

