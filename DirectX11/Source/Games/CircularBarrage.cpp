#include	"DxLibrary\DX_Library.h"
#include	"Bullet.h"
#include	"CircularBarrage.h"

//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
CircularBarrage::CircularBarrage() :
	m_isEnabled(false)
{
	for (int i = 0; i < BULLET_MAX; ++i)
	{
		m_pBulletList[i] = new Bullet();
	}
}

//-----------------------------------------------------------------------------------------
//
//  解放
//
//-----------------------------------------------------------------------------------------
CircularBarrage::~CircularBarrage()
{
	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		DELETE_OBJ(m_pBulletList[i]);
	}
}

//-----------------------------------------------------------------------------------------
//
//  更新
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::Update(float angle, float distance, DirectX::XMFLOAT3* pPosList)
{
	if (m_isEnabled)
	{
		for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
		{
			// 前回の角度と距離にひたすら加算していき、円形の弾が外側に動くようにする。
			// 弾が画面外に行くと更新と描画が行われなくなる。
			// 更新後の座標を2Dインスタンスオブジェクトの座標に設定する。
			m_pBulletList[i]->SetAngle(m_pBulletList[i]->GetAngle() + angle);
			m_pBulletList[i]->SetDistance(m_pBulletList[i]->GetDistance() + distance);
			m_pBulletList[i]->Update();
			pPosList[i] = m_pBulletList[i]->GetPos();
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//  リスポーン
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::Response()
{
	m_isEnabled = true;
	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		m_pBulletList[i]->SetEnabled(true);
		m_pBulletList[i]->SetAngle(i * 30.0f);
		m_pBulletList[i]->SetDistance(0.0f);
	}
}


//-----------------------------------------------------------------------------------------
//
//  弾の数を取得する
//
//-----------------------------------------------------------------------------------------
int CircularBarrage::GetBulletNum()
{
	return BULLET_MAX;
}

//-----------------------------------------------------------------------------------------
//
//  有効フラグセット
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::SetEnabled(bool isEnabled)
{
	m_isEnabled = isEnabled;
}