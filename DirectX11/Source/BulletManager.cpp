#include	"DX_LIBRARY\DX_Library.h"
#include	"CircularBarrage.h"
#include	"BulletManager.h"

//-----------------------------------------------------------------------------------------
//
//  èâä˙âª
//
//-----------------------------------------------------------------------------------------
BulletManager::BulletManager()
{
	circle = new DX_2DObject("circle.png");
	
	for (int i = 0; i < BulletManager::CIRCULARBARRAGE_MAX; i++)
	{
		circularBarrageList.push_back(new CircularBarrage());
		circularBarrageList[i]->SetEnabled(false);
	}

	interval = -1;
	enabledBulletIndex = 0;
}

//-----------------------------------------------------------------------------------------
//
//  âï˙
//
//-----------------------------------------------------------------------------------------
BulletManager::~BulletManager()
{
	std::vector<CircularBarrage*> vec;
	circularBarrageList.swap(vec);
	DELETE_OBJ(circle);
}

//-----------------------------------------------------------------------------------------
//
//  çXêV
//
//-----------------------------------------------------------------------------------------
void BulletManager::Update()
{
	interval++;
	if (interval == -1 || interval % 5 == 0)
	{
		interval = 0;
		circularBarrageList[enabledBulletIndex]->SetEnabled(true);
		circularBarrageList[enabledBulletIndex]->Response();

		enabledBulletIndex++;
		if (enabledBulletIndex >= BulletManager::CIRCULARBARRAGE_MAX)
		{
			enabledBulletIndex = 0;
		}		
	}

	for (int i = 0; i < BulletManager::CIRCULARBARRAGE_MAX; i++)
	{
		float _angle = (i % 2 == 0) ? 0.5f : -0.5f;
		circularBarrageList[i]->Update(_angle, 10.0f);
	}
}

//-----------------------------------------------------------------------------------------
//
//  ï`âÊ
//
//-----------------------------------------------------------------------------------------
void BulletManager::Render()
{
	for (int i = 0; i < BulletManager::CIRCULARBARRAGE_MAX; i++)
	{
		circularBarrageList[i]->Render();
	}
	float circleSize = 720.0f;
	float circleX = (DX_System::GetWindowWidth() - circleSize) * 0.5f;
	float circleY = (DX_System::GetWindowHeight() - circleSize) * 0.5f;
	circle->Render(DirectX::XMFLOAT2(circleX, circleY), DirectX::XMFLOAT2(circleSize, circleSize));
}