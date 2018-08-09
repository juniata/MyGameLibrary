#include	"DxLibrary\DX_Library.h"
#include	"CircularBarrage.h"
#include	"CircularBarrageManager.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
CircularBarrageManager::CircularBarrageManager() :
	m_pCircle(new DX_2DObject("circle.png")),
	m_pInstance2d(new DX_Instance2DObject("kinu.png", CircularBarrageManager::CIRCULARBARRAGE_MAX * CircularBarrage::GetBulletNum(), XMFLOAT2(32.0f, 32.0f))),
	m_interval(0),
	m_enabledBulletIndex(0)
{
	// determine new length, padding with _Val ele
	for (int i = 0; i < CircularBarrageManager::CIRCULARBARRAGE_MAX; i++)
	{
		m_pCircularBarrageList[i] = new CircularBarrage();
	}
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
		DELETE_OBJ(m_pCircularBarrageList[i]);
	}
	DELETE_OBJ(m_pCircle);
	DELETE_OBJ(m_pInstance2d);
}

//-----------------------------------------------------------------------------------------
//
//  更新
//
//-----------------------------------------------------------------------------------------
void CircularBarrageManager::Update()
{
	// 5フレーム毎に円形弾幕を生成する
	if (++m_interval == CREATE_CIRCULAR_BARRAGE_FRAME)
	{
		m_interval = 0;
		m_pCircularBarrageList[m_enabledBulletIndex]->Response();

		if (++m_enabledBulletIndex >= CircularBarrageManager::CIRCULARBARRAGE_MAX)
		{
			m_enabledBulletIndex = 0;
		}		
	}

	const int bulletNum = CircularBarrage::GetBulletNum();
	for (int i = 0; i < CircularBarrageManager::CIRCULARBARRAGE_MAX; i++)
	{
		float _angle = (i % 2 == 0) ? 0.5f : -0.5f;
		m_pCircularBarrageList[i]->Update(_angle, 10.0f, m_pInstance2d->GetPosList(i * bulletNum));
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
	m_pCircle->Render(DirectX::XMFLOAT2(circleX, circleY), DirectX::XMFLOAT2(circleSize, circleSize));

	// テスト描画
	m_pInstance2d->Render();
}