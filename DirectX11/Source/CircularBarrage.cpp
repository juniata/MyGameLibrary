#include	"DX_LIBRARY\DX_Library.h"
#include	"Bullet.h"
#include	"CircularBarrage.h"

//-----------------------------------------------------------------------------------------
//
//  ������
//
//-----------------------------------------------------------------------------------------
CircularBarrage::CircularBarrage()
{
	//bulletList.resize(CircularBarrage::BULLET_MAX, new Bullet());
	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		bulletList.push_back(new Bullet());
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
	bulletList.clear();
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