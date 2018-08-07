#include	"DxLibrary\DX_Library.h"
#include	"Bullet.h"
#include	"CircularBarrage.h"

//-----------------------------------------------------------------------------------------
//
//  ������
//
//-----------------------------------------------------------------------------------------
CircularBarrage::CircularBarrage()
{
	for (int i = 0; i < BULLET_MAX; ++i)
	{
		bulletList[i] = new Bullet();
		bulletList[i]->SetEnabled(true);
		bulletList[i]->SetAngle(i * 10);
	}
	//bulletList[0]->SetEnabled(true);
	
	isEnabled = true;
}

//-----------------------------------------------------------------------------------------
//
//  ���
//
//-----------------------------------------------------------------------------------------
CircularBarrage::~CircularBarrage()
{
	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		DELETE_OBJ(bulletList[i]);
	}
}

//-----------------------------------------------------------------------------------------
//
//  �X�V
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::Update(float angle, float distance, DirectX::XMFLOAT3* pPosList)
{
	if (!isEnabled)
	{
		return;
	}
	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		if (bulletList[i]->isEnabled) {
			bulletList[i]->Update();
			bulletList[i]->SetAngle(bulletList[i]->GetAngle() + angle);
			bulletList[i]->SetDistance(bulletList[i]->GetDistance() + distance);
			pPosList[i] = bulletList[i]->GetPos();
		}
		else {
			pPosList[i] = bulletList[i]->GetPos();
			pPosList[i].z = 1.0f;
		}

	}
}

//-----------------------------------------------------------------------------------------
//
//  ���X�|�[��
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::Response()
{
	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		bulletList[i]->SetEnabled(true);
		bulletList[i]->SetAngle(i * 30.0f);
		bulletList[i]->SetDistance(0.0f);
	}
}

//-----------------------------------------------------------------------------------------
//
//  �L���t���O�Z�b�g
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::SetEnabled(bool isEnabled)
{
	this->isEnabled = isEnabled;
}