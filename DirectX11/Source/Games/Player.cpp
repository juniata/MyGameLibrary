#include	"DxLibrary\DX_Library.h"
#include	"BarrageLaser.h"
#include	"BarrageBeam.h"
#include	"BarrageManager.h"
#include	"Player.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
Player::Player()
{
	m_pObj = new DX_2DObject("players.png"); // テスト中
	m_size = DirectX::XMFLOAT2(SIZE, SIZE);
	float basePosX = (DX_System::GetWindowWidth() - m_size.x) * 0.5f;
	float basePosY = (DX_System::GetWindowHeight() - m_size.y);
	m_pos = DirectX::XMFLOAT2(basePosX, basePosY);

	// ステータスを設定する
	m_status.life = INIT_LIFE;

	m_pBarrageManager = new BarrageManager(2);
	m_pBarrageManager->AddBarrage(new BarrageLaser("kinu.png", 11, XMFLOAT2(32.0f, 32.0f)));
	m_pBarrageManager->AddBarrage(new BarrageBeam("kinu.png", 11, XMFLOAT2(32.0f, 32.0f)));

	// 2つ目の弾幕クラスのインスタンスを有効化する
	m_pBarrageManager->GetBarrage(0)->Enable();
}

//-----------------------------------------------------------------------------------------
//
//  解放
//
//-----------------------------------------------------------------------------------------
Player::~Player()
{
	DELETE_OBJ(m_pObj);
	DELETE_OBJ(m_pBarrageManager);
}

//-----------------------------------------------------------------------------------------
//
//  移動
//
//-----------------------------------------------------------------------------------------
void Player::Move()
{
	XMFLOAT2 move(0.0f,0.0f);
	
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_LEFT))
	{
		move.x = -Player::MOVE_SPEED;
	}

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RIGHT))
	{
		move.x = Player::MOVE_SPEED;
	}
	
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_UP))
	{
		move.y = -Player::MOVE_SPEED;
	}
	
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_DOWN))
	{
		move.y = Player::MOVE_SPEED;
	}

	// 斜め移動の移動量を補正する
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_LEFT) || DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RIGHT)) {
		if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_UP) || DX_Input::IsKeyDown(DX_INPUT_KEY::DX_DOWN)) {
			float diagonalCorr = 1.0f  / sqrtf(2.0f);
			move.x *= diagonalCorr;
			move.y *= diagonalCorr;
		} 
		
	}

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_SHIFT))
	{
		move.x *= 0.5f;
		move.y *= 0.5f;
	}
	m_pos.x += move.x;
	m_pos.y += move.y;
}

//-----------------------------------------------------------------------------------------
//
//  攻撃
//
//-----------------------------------------------------------------------------------------
void Player::Attack()
{
}

//-----------------------------------------------------------------------------------------
//
//  更新
//
//-----------------------------------------------------------------------------------------
bool Player::Update()
{
	bool result = true;

	Move();

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  描画
//
//-----------------------------------------------------------------------------------------
bool Player::Render()
{
	bool result = false;

	result = m_pBarrageManager->Render();

	result = m_pObj->Render(m_pos, m_size);

	return result;
}