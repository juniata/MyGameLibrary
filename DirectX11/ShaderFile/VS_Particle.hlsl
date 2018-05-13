//-------------------------------------------------------------
//	ParticleSystem情報
//-------------------------------------------------------------
cbuffer ParticleSystemInfo : register(b1)
{
	//	列優先
	row_major float4x4 g_particlySystemInfoMat;
	/*
	   [0][0] ~ [0][2]  : エミッタの座標
	   [0][3]			: パーティクルの大きさ

	   [1][0] ~ [1][2]  : エミッターの向き
	   [1][3]			: パーティクルの数(int)

	   [2][0] ~ [2][2]  : エミッターの大きさ
	   [2][3]			: 現在のフレーム(int)
		
       [3][0] ~ [3][3]  : パーティクルの色
	*/
}

//-------------------------------------------------------------
//	CPUから送られてくる頂点情報
//-------------------------------------------------------------
struct InputVS
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
};

//-------------------------------------------------------------
//	VertexShaderからGeometryShaderに送る情報
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
//	g_particlySystemInfoMatからさまざまな情報を取得
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//	エミッターの座標を取得
//--------------------------------------------------------------
inline float3 GetEmitterPosFormMatrix()
{
	return g_particlySystemInfoMat[0].xyz;
}

//--------------------------------------------------------------
//	エミッターの向きを取得
//--------------------------------------------------------------
inline float3 GetEmitterAngleFormMatrix()
{
	return g_particlySystemInfoMat[1].xyz;
}

//--------------------------------------------------------------
//	エミッターの大きさを取得
//--------------------------------------------------------------
inline float3 GetEmitterScaleFormMatrix()
{
	return g_particlySystemInfoMat[2].xyz;
}

//--------------------------------------------------------------
//	パーティクルの大きさを取得
//--------------------------------------------------------------
inline float GetParticleScaleFromMatrix()
{
	return g_particlySystemInfoMat[0].w;
}

//--------------------------------------------------------------
//	パーティクルの数を取得
//--------------------------------------------------------------
inline int GetParticleCountFromMatrix()
{
	return (int)g_particlySystemInfoMat[1].w;
}
//--------------------------------------------------------------
//	現在のフレーム数を取得
//--------------------------------------------------------------
inline int GetNowFrameFromMatrix()
{
	return (int)g_particlySystemInfoMat[2].w;
}

//--------------------------------------------------------------
//	パーティクルの色情報を取得
//--------------------------------------------------------------
inline float4 GetParticleColorFromMatrix()
{
	return g_particlySystemInfoMat[3];
}


//--------------------------------------------------------------
//
//	ワールド座標に変換する
//
//--------------------------------------------------------------
inline float3 WorldTransformation(float3 pos)
{
	float3 outPos;

	//	パーティクルの大きさを掛ける
	outPos = pos * GetParticleScaleFromMatrix();

	//	エミッターの座標をパーティクルの座標に足す
	outPos += GetEmitterPosFormMatrix(); 

	return outPos;
}

//--------------------------------------------------------------
//
//	頂点シェーダー
//
//--------------------------------------------------------------
OUTPUT_GS VS(InputVS In, uint instID : SV_InstanceID)
{
	OUTPUT_GS Out;
	Out.pos = In.pos;
	Out.tex = In.tex;


	//	ワールド変換する
	Out.pos.xyz = WorldTransformation(Out.pos.xyz);

	//	パーティクルの色を取得し、設定する
	Out.color = GetParticleColorFromMatrix();

	//	現在のフレームをGSに送る
	Out.frame = GetNowFrameFromMatrix();
	
	//	インスタンス番号をGSに送る
	Out.instID = (int)instID;

	return Out;
}

