
//-------------------------------------------------------------
//	頂点シェーダーから送られてくる情報
//-------------------------------------------------------------
struct GS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	float3 wPos		: WPOS;
	float3 wNormal  : NORMAL;
	float3 viewDir  : VIEWDIR;
	float3 lightDir : LIGHTDIR;
};



//-------------------------------------------------------------
//	ピクセルシェーダーに送る情報
//-------------------------------------------------------------
struct PS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	float3 wPos		: WPOS;
	float3 wNormal  : NORMAL;
	float3 viewDir  : VIEWDIR;
	float3 lightDir : LIGHTDIR;

	float4 outpos	 : OUT_POS;	//	あたった座標(x,y,zは座標,wは1ならhit)
	float3 outnormal : OUT_NORMAL;	//	法線
};

//-------------------------------------------------------------
//	レイピックに必要な情報
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){
	float4 rayPos;			//	16byte alignmentのためfloat4にする
	float3 rayVec;			//	レイの向き
	float  rayDist;			//	レイの長さ
}

//-------------------------------------------------------------
//	面法線を計算する
//-------------------------------------------------------------
inline float3 CalcuFaceNormal(float3 vecA, float3 vecB, float3 vecC)
{
	float3 vecBA = vecB - vecA;
	float3 vecCB = vecC - vecB;

	return normalize(cross(vecBA, vecCB));
}

//-------------------------------------------------------------
//	デターミナント f3 f3 f3
//-------------------------------------------------------------
float det(float3 v1, float3 v2, float3 v3)
{
	return  (
		(v1.x * v2.y * v3.z) +
		(v1.y * v2.z * v3.x) +
		(v1.z * v2.x * v3.y) -
		(v1.x * v2.z * v3.y) -
		(v1.y * v2.x * v3.z) -
		(v1.z * v2.y * v3.x)
		);
}

//-------------------------------------------------------------
//	レイピック（クラメルの公式を使ったやり方）
//	
//	Tを交点とすると
//	T = v1  + edge1 * u + edge2 * v
//	T = origin + ray * t
//  edge1 * u + edge2 * v - ray * t = origin - v1
//	上記の式をクラメルの公式を用いてu,v,tの値を求める
//	0 <= u <= 1,	0 <= v <= 1,	0 <= u + v <= 1を満たしていれば
//	三角形を貫通する
//
//-------------------------------------------------------------
bool RayPick(out float3 hitPos,float3 v1,float3 v2,float3 v3)
{
	//	レイの逆ベクトル
	float3 invRay = -normalize(rayVec);
	float3 origin = rayPos.xyz;
	float3 edge1 = v2 - v1;
	float3 edge2 = v3 - v1;

	//	クラメルの公式の分母
	float denominator = det(edge1, edge2, invRay);
		
	//	レイが平面と平行でないかをチェック
	if (denominator > 0)
	{
		//	頂点１からレイ座標へのベクトル
		float3 orgVec = origin - v1;

		//	uを求める
		float u = det(orgVec, edge2, invRay);

		if ((u >= 0) && (u <= 1 * denominator))
		{
			//	vを求める
			float v = det(edge1, orgVec, invRay);

			if ((v >= 0) && (v + u) <= 1 * denominator)
			{
				//	tを求める
				float t = det(edge1, edge2, orgVec) / denominator;

				//	レイが当たった場所
				hitPos = origin + (-invRay) * t;
				return true;
			}
		}
	}
	return false;
}

//-------------------------------------------------------------
//	ジオメトリシェーダー
//-------------------------------------------------------------
[maxvertexcount(3)]
void GS_Main(triangle GS_INPUT input[3], inout TriangleStream<PS_OUTPUT> TriStream)
{
	PS_OUTPUT output;

	float3 hitPos = 0;

	//	レイピック処理
	bool bRayhit = RayPick(hitPos,input[0].wPos, input[1].wPos, input[2].wPos);

	//	面法線
	float3 wFaceNormal = CalcuFaceNormal(input[0].wPos, input[1].wPos, input[2].wPos);

	for (int i = 0; i < 3; ++i){
		output.outpos	= 0;
		output.pos		= input[i].pos;
		output.tex		= input[i].tex;
		output.color	= input[i].color;
		output.wPos		= input[i].wPos;
		output.wNormal	= input[i].wNormal;
		output.viewDir	= input[i].viewDir;
		output.lightDir = input[i].lightDir;

		//	レイが当たっていなければ、頂点色を黒にする
		if (bRayhit == true){
			output.color.rgb	= 0.0f;
			output.outpos		= float4(hitPos, 1);
			output.outnormal	= wFaceNormal;
		}
		

		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}
