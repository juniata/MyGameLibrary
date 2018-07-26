//-------------------------------------------------------------
//	パーティクルの情報（一つ一つの情報が格納されている）
//-------------------------------------------------------------
Buffer<float3>  g_particleResource : register(t0);

//-------------------------------------------------------------
//	乱数テクスチャ(100*100 -1~1)
//-------------------------------------------------------------
Buffer<float3>  g_randomResource : register(t1);


//-------------------------------------------------------------
//	パーティクルの座標を格納
//-------------------------------------------------------------
Buffer<float3>  g_particlePos : register(t2);

//-------------------------------------------------------------
//	ビューに関する行列
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){

	//	列優先
	row_major float4x4	g_viewMat;
	row_major float4x4	g_projMat;
	row_major float4x4	g_viewProjMat;
};


//-------------------------------------------------------------
//	VertexShaderからGeometryShaderに送られてくる情報
//-------------------------------------------------------------
struct INPUT_GS
{
	float4 pos			: SV_POSITION;
	float2 tex			: TEXCOORD;
	float4 color		: COLOR;
	int frame			: COLOR1;
	int instID			: COLOR2;

};

//-------------------------------------------------------------
//	GeometryShaderからPixelShaderに送る情報
//-------------------------------------------------------------
struct OUTPUT_PS
{
	float4 pos   : SV_POSITION;
	float2 tex   : TEXCOORD;
	float4 color : COLOR;

};



//--------------------------------------------------------------
//
//	ビルボード行列を作成
//
//--------------------------------------------------------------
//inline float4x4 CreateBillBord()
//{
//}

//-------------------------------------------------------------
//	GeometryShader
//-------------------------------------------------------------
[maxvertexcount(4)]
void GS(triangle INPUT_GS In[3],  inout TriangleStream< OUTPUT_PS > TriStream)
{
	OUTPUT_PS Out;

	//	インスタンスID
	int instID = In[0].instID;

	//	リソースからパーティクルを生成する時間を取得する
	int createParticleFrame = (int)g_particleResource.Load(instID).x;

	//	リソースからパーティクルが消滅する時間を取得する
	int deleteParticleFrame = (int)g_particleResource.Load(instID).y;

	//	現在のフレームを取得
	int nowFrame = In[0].frame;

	//	現在のフレーム数が、生成フレーム数に満たない場合return
	if (nowFrame < createParticleFrame){
		return;
	}

	//	現在のフレームが、消滅フレーム数を超えた場合return
	if (nowFrame >deleteParticleFrame){
		return;
	}

	float3 randomPos = g_randomResource.Load(instID % uint(100 * 100));
	float3 pos = g_particlePos.Load(instID);

	for (int i = 0; i < 3; ++i){
		Out.pos = In[i].pos;
		Out.tex = In[i].tex;
		Out.color = In[i].color;
	//	Out.pos.xyz += pos;
		Out.pos.xyz += pos + randomPos;
		
		//	投影変換を行う
		Out.pos = mul(Out.pos, g_viewProjMat);

		//	頂点を追加
		TriStream.Append(Out);
	}
	TriStream.RestartStrip();
}
