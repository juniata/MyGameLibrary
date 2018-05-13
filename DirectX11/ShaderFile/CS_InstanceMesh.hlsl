struct tagInstanceMeshInfo{
	float3 pos;
	float3 angle;
	float3 scale;
};

//	CPUから受け取ったバッファ
StructuredBuffer<tagInstanceMeshInfo> Input : register(t0);

//	データを出力するバッファ
RWStructuredBuffer<float4x4> Output : register(u0);

//-------------------------------------------------------------
//	縮小行列を作成
//-------------------------------------------------------------
inline void MatrixScaling(out float4x4 mat, float3 scale)
{
	mat = 0;
	mat._11 = scale.x;
	mat._22 = scale.y;
	mat._33 = scale.z;
	mat._44 = 1;
}

//-------------------------------------------------------------
//	回転行列+座標を作成
//-------------------------------------------------------------
inline void SetTransformMatrixXYZ(out float4x4 mat, float3 pos, float3 angle)
{
	mat = 0;

	float sinX = sin(angle.x);
	float sinY = sin(angle.y);
	float sinZ = sin(angle.z);

	float cosX = cos(angle.x);
	float cosY = cos(angle.y);
	float cosZ = cos(angle.z);

	mat._11 = ( cosY * cosZ) + (sinX * sinY * sinZ);
	mat._12 = ( cosX * sinZ);
	mat._13 = (-sinY * cosZ) + (sinX * cosY * sinZ);
	mat._14 = 0;

	mat._21 = (-cosY * sinZ) + (sinX * sinY * cosZ);
	mat._22 = ( cosX * cosZ);
	mat._23 = ( sinY * sinX) + (sinX * cosY * cosZ);
	mat._24 = 0;

	mat._31 = ( cosX * sinY);
	mat._32 = -sinX;
	mat._33 = ( cosX * cosY);
	mat._34 = 0;

	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;
	mat._44 = 1;

}

//-------------------------------------------------------------
//	CSの処理
//-------------------------------------------------------------
[numthreads(8, 1, 1)]
void CS_Main( 
	uint3 threadID	: SV_DispatchThreadID
	)
{
	float4x4 matScale;
	float4x4 transMatrix;
	
	MatrixScaling(matScale, Input[threadID.x].scale);
	SetTransformMatrixXYZ(transMatrix, Input[threadID.x].pos, Input[threadID.x].angle);

	Output[threadID.x] = mul(matScale, transMatrix);

}