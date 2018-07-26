struct tagBoneInfo{
	short word;	//	�e
	float4x4 offsetMatrix;	//	����s��
	float3 orgPos;	//	�W�����W
	float4 orgPose;	//	�W���p��

};
//	CPU����󂯎�����o�b�t�@
StructuredBuffer<tagBoneInfo> Input : register(t0);

//	�f�[�^���o�͂���o�b�t�@
RWStructuredBuffer<float> Output : register(u0);

//-------------------------------------------------------------
//	�t���[���ɂ���Ďp�����X�V����
//-------------------------------------------------------------
void UpdateSkinMeshFrame(float dwFrame)
{
	for (int i = 0; i < boneCount; ++i){

		//	�p����ݒ�
		UpdateBonePoseByFrame(dwFrame);

		//	���W��ݒ�
		UpdateBonePosByFrame(dwFrame);
	}
}

//-------------------------------------------------------------
//	�t���[���ɂ���Ďp�����X�V����
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
//	�t���[���ɂ���Ďp�����X�V����
//-------------------------------------------------------------
void UpdateBonePosByFrame(float dwFrame)
{

}
//-------------------------------------------------------------
//	�{�[���s����X�V
//-------------------------------------------------------------
void UpdateBoneMatrix()
{

}

//-------------------------------------------------------------
//	CS�̏���
//-------------------------------------------------------------
[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	
	if (Input[DTid.x].frame != Input[DTid.x].dwFrame){

		//	�t���[���ɂ���Ďp�����X�V����
		UpdateSkinMeshFrame(Input[Dtid.x].dwFrame);

		//	�p���b�g�s����쐬
		UpdateBoneMatrix();
	}

	Output[DTid] = Input[DTid.x].dwFrame;
}