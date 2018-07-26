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
void CircularBarrage::Update(float angle, float distance)
{
	if (!isEnabled)
	{
		return;
	}

	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		bulletList[i]->Update();
		bulletList[i]->SetAngle(bulletList[i]->GetAngle() + angle);
		bulletList[i]->SetDistance(bulletList[i]->GetDistance() + distance);
	}
}

//-----------------------------------------------------------------------------------------
//
//  �`��
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::Render()
{
	if (!isEnabled)
	{
		return;
	}

	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		bulletList[i]->Render();
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