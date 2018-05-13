//-------------------------------------------------------------
//	ビューに関する行列
//-------------------------------------------------------------
cbuffer MatrixForTheViewCBuffer	: register(b0){

	float4x4	g_viewMat;
	float4x4	g_projMat;
	float4x4	g_viewProjMat;
};

//-------------------------------------------------------------
//	オブジェクトのワールド座標
//-------------------------------------------------------------
cbuffer WorldMatCBuffer : register(b1){
	float4x4	g_worldMat;
};

//-------------------------------------------------------------
//	ボーン行列
//-------------------------------------------------------------
Buffer<float4>  g_bones : register(t1);

//-------------------------------------------------------------
//	ライティング
//-------------------------------------------------------------
cbuffer Lighting : register(b3){
	struct tagLight{
		float4 ambientColor;
		float4 diffuseColor;
		float3 viewPos;
		float3 lightPos;
		float3 lightDir;
		float3 padding;
	}g_light;
};

//-------------------------------------------------------------
//	頂点シェーダーに送られてくる情報
//-------------------------------------------------------------
struct VS_INPUT
{
	//	startslot 0
	float3  pos			: POSITION;
	float2  tex			: TEXCOORD;
	float3	normal		: NORMAL;
	float4	color		: COLOR;
	
	//	startslot 1
	uint    skinIndex	: SKIN_INDEX;
	float4	skinWeight	: SKIN_WEIGHT;
};


//-------------------------------------------------------------
//	ジオメトリシェーダーに送る情報
//-------------------------------------------------------------
struct VS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD;
	float4 color	: COLOR;

	//	自身で算出して送る変数
	float3 wPos		: WPOS;		//	頂点座標をワールド座標にしたもの
	float3 wNormal  : NORMAL;	//	頂点法線をワールド座標にしたもの
	float3 viewDir  : VIEWDIR;	//	ワールド座標の頂点からカメラへのベクトル
	float3 lightDir : LIGHTDIR;	//	ワールド座標の頂点からライトへのベクトル

};

//-------------------------------------------------------------
//	ボーンインデックスを使いBuffer<float4> g_bonesから行列を取得
//-------------------------------------------------------------
float3x4 GetSkinningMatrix(uint boneIndex)
{
	float4 r0 = g_bones.Load(boneIndex * 4 + 0);
	float4 r1 = g_bones.Load(boneIndex * 4 + 1);
	float4 r2 = g_bones.Load(boneIndex * 4 + 2);

	//	4番目の要素は0,0,0,1なので要らない
	return float3x4(r0, r1, r2);
}

//-------------------------------------------------------------
//	スキニング処理
//-------------------------------------------------------------
inline float3x4 UpdateSkinMesh( const uint boneIndex, const float4 skinWeight)
{
	//	ボーン行列の単位行列
	float3x4 skinIdentity = 0;

	//	スキニング処理
	skinIdentity =  GetSkinningMatrix((boneIndex & 0x000000ff)		)	* skinWeight.x;
	skinIdentity += GetSkinningMatrix((boneIndex & 0x0000ff00) >> 8 )	* skinWeight.y;
	skinIdentity += GetSkinningMatrix((boneIndex & 0x00ff0000) >> 16)	* skinWeight.z;
	skinIdentity += GetSkinningMatrix((boneIndex & 0xff000000) >> 24)	* skinWeight.w;

	return skinIdentity;
}

//-------------------------------------------------------------
//	ボーンが影響を与える頂点の色を設定
//-------------------------------------------------------------
inline void SetBoneColor(inout float4 vertexColor, float3 color, uint skinIndex, int boneNum)
{
	if ((skinIndex & 0x000000ff) == boneNum){
		vertexColor.rgb = color;
	}else if (((skinIndex & 0x0000ff00) >>  8) == boneNum){
		vertexColor.rgb = color;
	}else if (((skinIndex & 0x00ff0000) >> 16) == boneNum){
		vertexColor.rgb = color;
	}else if (((skinIndex & 0xff000000) >> 24) == boneNum){
		vertexColor.rgb = color;
	}
}

//-------------------------------------------------------------
//	拡散光を算出
//-------------------------------------------------------------
inline float3 DiffuseColor(float3 wNormal)
{
	return (pow(dot(wNormal, -g_light.lightDir), 0) * g_light.diffuseColor).rgb;
}

//-------------------------------------------------------------
//	float4 = float3 * float3x4
//-------------------------------------------------------------
inline float4 MultiFloat3toFloat3x4(float3 pos, float3x4 mat)
{
	float4 outpos;

	//	dot(mat[0],pos) + mat._14でも計算可
	outpos.x = (pos.x * mat._11) + (pos.y * mat._12) + (pos.z * mat._13) + mat._14;
	outpos.y = (pos.x * mat._21) + (pos.y * mat._22) + (pos.z * mat._23) + mat._24;
	outpos.z = (pos.x * mat._31) + (pos.y * mat._32) + (pos.z * mat._33) + mat._34;
	outpos.w = 1;

	return outpos;
}

//-------------------------------------------------------------
//	頂点シェーダー
//-------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT In)
{
	VS_OUTPUT Out;

	float3x3 wMat = g_worldMat;

	//	スキニング行列
	float3x4 skinningMatrix = UpdateSkinMesh(In.skinIndex, In.skinWeight);

	//	頂点座標をスキニング
	Out.pos = MultiFloat3toFloat3x4(In.pos, skinningMatrix);
	
	//	ローカル座標からワールド座標
	//Out.pos = MultiFloat3toFloat3x4(Out.pos.xyz, g_worldMat);
	Out.pos = mul(g_worldMat, Out.pos);

	//	法線をスキニング
	Out.wNormal = MultiFloat3toFloat3x4(In.normal, skinningMatrix).xyz;
	Out.wNormal = mul(wMat, In.normal);

	//	ワールド座標
	Out.wPos = Out.pos.xyz;

	//	ワールド座標から射影座標
	Out.pos = mul(g_viewProjMat,	Out.pos);
	
	//	カメラへのベクトル
	Out.viewDir = normalize(g_light.viewPos - Out.wPos);

	//	ライトへのベクトル
	Out.lightDir = normalize(g_light.lightPos - Out.wPos);

	//	テクスチャ座標
	Out.tex = In.tex;

	//	拡散光を反映
	Out.color.rgb = In.color.rgb * DiffuseColor(Out.wNormal);

	//	環境光を反映
	Out.color.rgb += g_light.ambientColor.rgb;

	Out.color.a = 1.0f;

	
	return Out;
}

