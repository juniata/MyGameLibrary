#include	"DX_LIBRARY\DX_Library.h"
#include	"CircularBarrage.h"
#include	"BulletManager.h"

//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
BulletManager::BulletManager()
{
	circle = new DX_2DObject("circle.png");
	
	//circularBarrageList.resize(CIRCULARBARRAGE_MAX, new CircularBarrage());
	// determine new length, padding with _Val ele
	for (int i = 0; i < BulletManager::CIRCULARBARRAGE_MAX; i++)
	{
		circularBarrageList.push_back(new CircularBarrage());
		circularBarrageList[i]->SetEnabled(true);
	}

	interval = -1;
	enabledBulletIndex = 0;
}

//-----------------------------------------------------------------------------------------
//
//  解放
//
//-----------------------------------------------------------------------------------------
BulletManager::~BulletManager()
{
	for (int i = 0; i < BulletManager::CIRCULARBARRAGE_MAX; i++)
	{
		DELETE_OBJ(circularBarrageList[i]);
	}
	circularBarrageList.clear();
	DELETE_OBJ(circle);
}

//-----------------------------------------------------------------------------------------
//
//  更新
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
//  描画
//
//-----------------------------------------------------------------------------------------
void BulletManager::Render()
{
	//PROFILE(" BulletManager::Render")
	for (int i = 0; i < BulletManager::CIRCULARBARRAGE_MAX; i++)
	{
		circularBarrageList[i]->Render();
	}
	float circleSize = 720.0f;
	float circleX = (DX_System::GetWindowWidth() - circleSize) * 0.5f;
	float circleY = (DX_System::GetWindowHeight() - circleSize) * 0.5f;
	circle->Render(DirectX::XMFLOAT2(circleX, circleY), DirectX::XMFLOAT2(circleSize, circleSize));
}