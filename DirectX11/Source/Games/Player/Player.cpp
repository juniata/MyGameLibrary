#include	"DxLibrary/DX_Library.h"
#include	"Player.h"

Player::Player() :
	m_pos(DirectX::XMFLOAT2(0.0f,0.0f)),
	m_animationIndex(0),
	m_animationInterval(0),
	m_IsMove(false)
{

}
Player::~Player()
{
	DELETE_OBJ(m_pObjects[0]);
	DELETE_OBJ(m_pObjects[1]);
}

bool Player::Initialize(const DirectX::XMFLOAT2& initPos)
{
	m_pObjects[0] = new DX_2DObject();
	m_pObjects[1] = new DX_2DObject();
	m_pObjects[0]->Initialize("SceneMain\\player\\player_wait.png");
	m_pObjects[1]->Initialize("SceneMain\\player\\player_walk1.png");
	m_pos = initPos;

	return true;
}
bool Player::Update()
{
	m_IsMove = false;
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_D) || DX_Input::IsKeyDown(DX_INPUT_KEY::DX_A)) {
		Move();
	}
	return true;
}
void Player::Move()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_D)) {
		m_IsMove = true;
		m_pos.x += 6.0f;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_A)) {
		m_IsMove = true;
		m_pos.x -= 6.0f;
	}
}
bool Player::Render()
{
	if (m_IsMove)
	{
		if (++m_animationInterval > 3) {
			m_animationInterval = 0;
			++m_animationIndex;
		}
		if (m_animationIndex > 1) {
			m_animationIndex = 0;
		}
	}
	else {
		m_animationIndex = 0;
	}
	m_pObjects[m_animationIndex]->Render(m_pos.x, m_pos.y, m_pos.x + 32, m_pos.y + 32);

	return true;
}
