#include	"DxLibrary\DX_Library.h"
#include	"CircularBarrage.h"
#include	"CircularBarrageManager.h"

//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
CircularBarrageManager::CircularBarrageManager()
{
	circle = new DX_2DObject("circle.png");

	// テスト描画
	m_pInstance2d = new DX_Instance2DObject("kinu.png", CircularBarrageManager::CIRCULARBARRAGE_MAX * 12);

	// determine new length, padding with _Val ele
	for (int i = 0; i < CircularBarrageManager::CIRCULARBARRAGE_MAX; i++)
	{
		circularBarrageList[i] = new CircularBarrage();
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
CircularBarrageManager::~CircularBarrageManager()
{
	for (int i = 0; i < CircularBarrageManager::CIRCULARBARRAGE_MAX; i++)
	{
		DELETE_OBJ(circularBarrageList[i]);
	}
	DELETE_OBJ(circle);
}

//-----------------------------------------------------------------------------------------
//
//  更新
//
//-----------------------------------------------------------------------------------------
void CircularBarrageManager::Update()
{
	interval++;
	if (interval == -1 || interval % 5 == 0)
	{
		interval = 0;
		circularBarrageList[enabledBulletIndex]->SetEnabled(true);
		circularBarrageList[enabledBulletIndex]->Response();

		enabledBulletIndex++;
		if (enabledBulletIndex >= CircularBarrageManager::CIRCULARBARRAGE_MAX)
		{
			enabledBulletIndex = 0;
		}		
	}

	for (int i = 0; i < CircularBarrageManager::CIRCULARBARRAGE_MAX; i++)
	{
		float _angle = (i % 2 == 0) ? 0.5f : -0.5f;
		circularBarrageList[i]->Update(_angle, 10.0f, m_pInstance2d->GetPosList(i * 12));
	}
}

//-----------------------------------------------------------------------------------------
//
//  描画
//
//-----------------------------------------------------------------------------------------
void CircularBarrageManager::Render()
{
	float circleSize = 720.0f;
	float circleX = (DX_System::GetWindowWidth() - circleSize) * 0.5f;
	float circleY = (DX_System::GetWindowHeight() - circleSize) * 0.5f;
	circle->Render(DirectX::XMFLOAT2(circleX, circleY), DirectX::XMFLOAT2(circleSize, circleSize));

	// テスト描画
	m_pInstance2d->Render();
}