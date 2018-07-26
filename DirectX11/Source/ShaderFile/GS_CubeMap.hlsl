//-------------------------------------------------------------
//	キューブマップに関する行列
//-------------------------------------------------------------
cbuffer MatrixForTheCubeMapCBuffer	: register(b3){

	//	列優先
	column_major float4x4	viewMat[6];		//	view
	column_major float4x4	projMat;		//	projection
};


//-------------------------------------------------------------
//	頂点シェーダーから送られてくる情報
//-------------------------------------------------------------
typedef struct GS_INPUT
{
	float4 pos		: SV_POSITION;
}GS_OUTPUT;


//-------------------------------------------------------------
//	ピクセルシェーダーに送る情報
//-------------------------------------------------------------
struct PS_OUTPUT
{
	float4 pos				 : SV_POSITION;
	uint   renderTargetIndex : SV_RenderTargetArrayIndex;
};


//-------------------------------------------------------------
//	ジオメトリシェーダーの処理
//-------------------------------------------------------------
[maxvertexcount(16)] // 3頂点*6面
void GS_Main(
	triangle GS_INPUT In[3],
	inout TriangleStream< PS_OUTPUT > cubeMapStream
	)
{
	//	6面を計算する
	for (int i = 0; i < 6; ++i){

		//	各面毎に頂点座標を計算して描画先を設定
		PS_OUTPUT Out; 

		// このプリミティブを描画する描画ターゲット
		Out.renderTargetIndex = i;

		for (int j = 0; j < 3; ++j){
			Out.pos = mul(viewMat[i],	In[j].pos);	//	ビュー変換
			Out.pos = mul(projMat,		In[j].pos);	//	射影変換
			cubeMapStream.Append(Out);	//	頂点出力
		}

		//	1ストリップ終了
		cubeMapStream.RestartStrip();
	}
}