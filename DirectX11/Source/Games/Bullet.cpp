#include	"DxLibrary\DX_Library.h"
#include	"Bullet.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
Bullet::Bullet() :
	m_size(32.0f,32.0f),
	m_distance(0.0f),
	m_angle(0.0f),
	m_isEnabled(false)
{
	float basePosX = (DX_System::GetWindowWidth() - m_size.x) * 0.5f;
	float basePosY = (DX_System::GetWindowHeight() - m_size.y) * 0.5f;
	m_pos = m_basePos = DirectX::XMFLOAT2(basePosX, basePosY);
}

//-----------------------------------------------------------------------------------------
//
//  解放
//
//-----------------------------------------------------------------------------------------
Bullet::~Bullet()
{
}

//-----------------------------------------------------------------------------------------
//
//  移動
//
//-----------------------------------------------------------------------------------------
void Bullet::Move()
{
	//distance += 5.0f;
	//if (radius >= 360.0f)radius = 360.0f;
	//angle += 1.0f;
	// m_dis
	float x = m_basePos.x + m_distance * cosf(m_angle * atanf(1.0f) * (4.0f / 180.0f));
	float y = m_basePos.y + m_distance * sinf(m_angle * atanf(1.0f) * (4.0f / 180.0f));
	m_pos = DirectX::XMFLOAT2(x, y);
}

//-----------------------------------------------------------------------------------------
//
//  更新
//
//-----------------------------------------------------------------------------------------
void Bullet::Update(DirectX::XMFLOAT3* pPos)
{
	if (m_isEnabled)
	{
		DeathRecord();
		Move();
		*pPos = XMFLOAT3(m_pos.x, m_pos.y, m_isEnabled ? 0.0f : 1.1f);
	}
}


//-----------------------------------------------------------------------------------------
//
//  有効フラグセット
//
//-----------------------------------------------------------------------------------------
void Bullet::SetEnabled(bool isEnabled)
{
	m_isEnabled = isEnabled;
}

//-----------------------------------------------------------------------------------------
//
//  リスポーン
//
//-----------------------------------------------------------------------------------------
void Bullet::Response()
{
	m_pos		= m_basePos;
	m_distance	= 0.0f;
	m_angle		= 0.0f;
	m_isEnabled = false;
}

//-----------------------------------------------------------------------------------------
//
//  死亡記録
//
//-----------------------------------------------------------------------------------------
void Bullet::DeathRecord()
{
	if (m_pos.x <= 0.0f || m_pos.x >= CAST_F(DX_System::GetWindowWidth()) || 
		m_pos.y <= 0.0f || m_pos.y >= CAST_F(DX_System::GetWindowHeight()))
	{
		Response();
	}
}

//-----------------------------------------------------------------------------------------
//
//  角度をセットする
//
//-----------------------------------------------------------------------------------------
void Bullet::SetAngle(float angle)
{
	m_angle = angle;
}

//-----------------------------------------------------------------------------------------
//
//  中心点からの距離をセットする
//
//-----------------------------------------------------------------------------------------
void Bullet::SetDistance(float distance)
{
	m_distance = distance;
}

//-----------------------------------------------------------------------------------------
//
//  角度取得
//
//-----------------------------------------------------------------------------------------
float Bullet::GetAngle()
{
	return m_angle;
}

//-----------------------------------------------------------------------------------------
//
//  距離取得
//
//-----------------------------------------------------------------------------------------
float Bullet::GetDistance()
{
	return m_distance;
}