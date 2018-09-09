#include	"DxLibrary\DX_Library.h"
#include	"Bullet.h"
#include	"BarrageWay.h"
#include	"Player.h"

using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  �R���X�g���N�^
//
//-----------------------------------------------------------------------------------------
Player::Player() :
	m_pObj(new DX_2DObject()),
	m_size(XMFLOAT2(SIZE, SIZE)),
	m_pBarrageWay(new BarrageWay("SceneMain\\Bullet.png", 100, XMFLOAT2(32.0f, 32.0f)))
{
	float basePosX = (DX_System::GetWindowWidth() - m_size.x) * 0.5f;
	float basePosY = (DX_System::GetWindowHeight() - m_size.y);
	m_pos = DirectX::XMFLOAT2(basePosX, basePosY);

	// �X�e�[�^�X��ݒ肷��
	m_status.life = INIT_LIFE;
}

//-----------------------------------------------------------------------------------------
//
//  ���
//
//-----------------------------------------------------------------------------------------
Player::~Player()
{
	DELETE_OBJ(m_pObj);
	DELETE_OBJ(m_pBarrageWay);
}

//-----------------------------------------------------------------------------------------
//
//  ������
//
//-----------------------------------------------------------------------------------------
bool Player::Initialize()
{
	bool result = false;

	result = m_pObj->Initialize("SceneMain\\player.png");

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �ړ�
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

	// �΂߈ړ��̈ړ��ʂ�␳����
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

	DX_System* pSystem = DX_System::GetInstance();
	const unsigned int width	= pSystem->GetWindowWidth();
	const unsigned int height	= pSystem->GetWindowHeight();

	// �g����
	if (m_pos.x < 0.0f) { m_pos.x = 0.0f; }
	if (m_pos.y < 0.0f) { m_pos.y = 0.0f; }
	if (m_pos.x > width - m_size.x) { m_pos.x = width - m_size.x; }
	if (m_pos.y > height - m_size.y) { m_pos.y = height - m_size.y; }
}

//-----------------------------------------------------------------------------------------
//
//  �U��
//
//-----------------------------------------------------------------------------------------
void Player::Attack()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_Z)) {
		m_pBarrageWay->Set(m_pos, 10.0f, 3);
	}
	m_pBarrageWay->Update();
}

//-----------------------------------------------------------------------------------------
//
//  �X�V
//
//-----------------------------------------------------------------------------------------
bool Player::Update()
{
	bool result = true;

	// �ړ�
	Move();

	// �U��
	Attack();

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �`��
//
//-----------------------------------------------------------------------------------------
bool Player::Render()
{
	bool result = false;

	result = m_pObj->Render(m_pos, m_size);
	result = m_pBarrageWay->Render();

	return result;
}