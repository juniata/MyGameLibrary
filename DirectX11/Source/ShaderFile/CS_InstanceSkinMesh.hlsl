struct tagBoneInfo{
	short word;	//	親
	float4x4 offsetMatrix;	//	基準化行列
	float3 orgPos;	//	標準座標
	float4 orgPose;	//	標準姿勢

};
//	CPUから受け取ったバッファ
StructuredBuffer<tagBoneInfo> Input : register(t0);

//	データを出力するバッファ
RWStructuredBuffer<float> Output : register(u0);

//-------------------------------------------------------------
//	フレームによって姿勢を更新する
//-------------------------------------------------------------
void UpdateSkinMeshFrame(float dwFrame)
{
	for (int i = 0; i < boneCount; ++i){

		//	姿勢を設定
		UpdateBonePoseByFrame(dwFrame);

		//	座標を設定
		UpdateBonePosByFrame(dwFrame);
	}
}

//-------------------------------------------------------------
//	フレームによって姿勢を更新する
//-------------------------------------------------------------
void UpdateBonePoseByFrame(float dwFrame)
{
	switch (rotCount)
	{
	case 0:break;
	case 1:break;
	default:
		break;
	}
}

//-------------------------------------------------------------
//	フレームによって姿勢を更新する
//-------------------------------------------------------------
void UpdateBonePosByFrame(float dwFrame)
{

}
//-------------------------------------------------------------
//	ボーン行列を更新
//-------------------------------------------------------------
void UpdateBoneMatrix()
{

}

//-------------------------------------------------------------
//	CSの処理
//-------------------------------------------------------------
[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	
	if (Input[DTid.x].frame != Input[DTid.x].dwFrame){

		//	フレームによって姿勢を更新する
		UpdateSkinMeshFrame(Input[Dtid.x].dwFrame);

		//	パレット行列を作成
		UpdateBoneMatrix();
	}

	Output[DTid] = Input[DTid.x].dwFrame;
}