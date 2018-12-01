#include	"DxLibrary/DX_Library.h"
#include	"Player.h"
#include	"Games/SceneMain/Stage.h"
#include	"Games/MapChip/MapChip.h"
#include	"Games/GameDefine.h"
const float Player::GRAVITY = 9.8f * MASS;	// 2D���� -���� ��ɍs�����߉��ɂ���K�v������

Player::Player() :
	m_pos(DirectX::XMFLOAT2(0.0f,0.0f)),
	m_force(DirectX::XMFLOAT2(0.0f, 0.0f)),
	m_velocity(DirectX::XMFLOAT2(0.0f,0.0f)),
	m_initialVelocity(DirectX::XMFLOAT2(0.0f, 0.0f)),
	m_animationType(0),
	m_animationActionType(0),
	m_animationWalkInterval(0),
	m_animationAttackInterval(0),
	m_actionType(ACTION_TYPE::WAIT),
	m_pObjects(new DX_2DObject*[CAST_I(ANIMATION_TYPE::MAX)]),
	m_isAnimationMirror(false)
{
	m_pObjects[CAST_I(ANIMATION_TYPE::WAIT)]	= new DX_2DObject[CAST_I(ANIMATION_WAIT_TYPE::MAX)];
	m_pObjects[CAST_I(ANIMATION_TYPE::WALK)]	= new DX_2DObject[CAST_I(ANIMATION_WALK_TYPE::MAX)];
	m_pObjects[CAST_I(ANIMATION_TYPE::ATTACK)]	= new DX_2DObject[CAST_I(ANIMATION_ATTACK_TYPE::MAX)];
	m_pObjects[CAST_I(ANIMATION_TYPE::JUMP)]	= new DX_2DObject[CAST_I(ANIMATION_JUMP_TYPE::MAX)];
}
Player::~Player()
{
	for (int i = 0; i < CAST_I(ANIMATION_TYPE::MAX); ++i)
	{
		DELETE_OBJ_ARRAY(m_pObjects[i]);
	}
	DELETE_OBJ_ARRAY(m_pObjects);
}

bool Player::Initialize(const DirectX::XMFLOAT2& initPos)
{
	
	m_pObjects[CAST_I(ANIMATION_TYPE::WAIT)][CAST_I(ANIMATION_WAIT_TYPE::WAIT1)].Initialize("SceneMain\\player\\player_wait.png");

	m_pObjects[CAST_I(ANIMATION_TYPE::WALK)][CAST_I(ANIMATION_WALK_TYPE::WALK1)].Initialize("SceneMain\\player\\player_walk1.png");
	m_pObjects[CAST_I(ANIMATION_TYPE::WALK)][CAST_I(ANIMATION_WALK_TYPE::WALK2)].Initialize("SceneMain\\player\\player_walk2.png");

	m_pObjects[CAST_I(ANIMATION_TYPE::ATTACK)][CAST_I(ANIMATION_ATTACK_TYPE::WAIT_ATTACK1)].Initialize("SceneMain\\player\\player_attack_wait_1.png");
	m_pObjects[CAST_I(ANIMATION_TYPE::ATTACK)][CAST_I(ANIMATION_ATTACK_TYPE::WAIT_ATTACK2)].Initialize("SceneMain\\player\\player_attack_wait_2.png");
	m_pObjects[CAST_I(ANIMATION_TYPE::ATTACK)][CAST_I(ANIMATION_ATTACK_TYPE::WALK_ATTACK1)].Initialize("SceneMain\\player\\player_attack_walk_1.png");
	m_pObjects[CAST_I(ANIMATION_TYPE::ATTACK)][CAST_I(ANIMATION_ATTACK_TYPE::WALK_ATTACK2)].Initialize("SceneMain\\player\\player_attack_walk_2.png");

	m_pObjects[CAST_I(ANIMATION_TYPE::JUMP)][CAST_I(ANIMATION_JUMP_TYPE::WAIT_JUMP_1)].Initialize("SceneMain\\player\\player_jump_wait.png");

	SetAnimationType(ANIMATION_TYPE::WAIT, ANIMATION_WAIT_TYPE::WAIT1);
	m_pos = initPos;

	DX_2DObject* aaa = m_pObjects[0];
	return true;
}
bool Player::Update(Stage* pStage)
{
	ACTION_TYPE pastActionType = m_actionType;
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_D) || DX_Input::IsKeyDown(DX_INPUT_KEY::DX_A))
	{
		if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_Z)) {
			m_actionType = ACTION_TYPE::ATTACK_WALK_1;
		}
		else {
			m_actionType = ACTION_TYPE::WALK_1;
		}
	}
	else if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_Z)) {
		m_actionType = ACTION_TYPE::ATTACK_WAIT_1;
	}
	else if (DX_Input::IsKeyHit(DX_INPUT_KEY::DX_C)){
		m_actionType = ACTION_TYPE::JUMP_WAIT_1;
	}
	else {
		m_actionType = ACTION_TYPE::WAIT;
	}

	if (pastActionType != m_actionType) {
		m_animationAttackInterval = 0;
		m_animationWalkInterval = 0;
	}

	switch (m_actionType) {
		// �~�܂�
	case ACTION_TYPE::WAIT:
		Wait();
		break;

		// ����
	case ACTION_TYPE::WALK_1:
		Walk();
		break;
		// �~�܂��čU��
	case ACTION_TYPE::ATTACK_WAIT_1:
		WaitAttack();
		break;
		// �����Ȃ���U��
	case ACTION_TYPE::ATTACK_WALK_1:
		WalkAttack();
		break;
	case ACTION_TYPE::JUMP_WAIT_1:
		WaitJump();
		break;
	}

	Move();

	Collision(pStage);

	return true;
}
void Player::Move()
{
	float t = DX_FrameWork::GetInstance()->GetDeltaTime();

	DirectX::XMFLOAT2 pastPos = m_pos;

	// ���� = ����(m) * �����xa(m/s~2)
	// �����x = ���� / ����
	m_accelerator.x += (m_force.x / MASS);
	m_accelerator.y += (m_force.y / MASS);

	// �d�͂����Z(�����蔻������{���Ǝg��
	m_accelerator.y += GRAVITY;

	// TODO: �����x��萔�ł��Ɠ������p�Ȃ�B ���߂��ق�����������
	//DirectX::XMFLOAT2 initialVelocity(100.0f, 0.0f);
	DirectX::XMFLOAT2 initialVelocity(0.0f, 0.0f);

	// �ŏI���x = �����x + �����x * ����
	// m/s = m/s + m/s(t)^2 * s(t)
	m_velocity.x = m_initialVelocity.x + m_accelerator.x * t;
	m_velocity.y = m_initialVelocity.y + m_accelerator.y * t;

	// �ړ��� m = m/s ^ s
	DirectX::XMFLOAT2 move(m_velocity.x * t, m_velocity.y * t);

	// ���W�Z�o
	m_pos.x += move.x;
	m_pos.y += move.y;

	// ���߂����x��0�Ȃ�A�����x��0�ɏ���������B
	if (m_velocity.x == 0.0f) {
		m_initialVelocity.x = 0.0f;
	}
}
void Player::Wait()
{
	SetAnimationType(ANIMATION_TYPE::WAIT, ANIMATION_WAIT_TYPE::WAIT1);
}
void Player::Collision(Stage* pStage)
{
	DirectX::XMFLOAT2 diff(0.0f,0.0f);
	if (pStage->IsHit(m_pos, &diff)) {
	//	m_pos.x -= diff.x;
		m_pos.y -= diff.y;
		m_accelerator.y = 0.0f;
	}
}
void Player::Walk()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_D)) {
		m_force.x = 1000.0f;
		if (m_initialVelocity.x == .0f) {
		//	m_initialVelocity.x = 200.0f;
		}
		m_isAnimationMirror = false;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_A)) {
		m_force.x = -1000.0f;
		if (m_initialVelocity.x == .0f) {
		//	m_initialVelocity.x = -200.0f;
		}
		m_isAnimationMirror = true;
	}

	if (GetAnimationType() == ANIMATION_TYPE::WALK) {
		if (++m_animationWalkInterval > 5) {
			ANIMATION_WALK_TYPE walkType = m_animationActionType == CAST_I(ANIMATION_WALK_TYPE::WALK1) ? ANIMATION_WALK_TYPE::WALK2 : ANIMATION_WALK_TYPE::WALK1;
			SetAnimationType(ANIMATION_TYPE::WALK, walkType);
			m_animationWalkInterval = 0;
		}
	}
	else {
		SetAnimationType(ANIMATION_TYPE::WALK, ANIMATION_WALK_TYPE::WALK1);
	}
}
void Player::WaitAttack()
{
	if (++m_animationAttackInterval > 5) {
		m_animationAttackInterval = 0;
		SetAnimationType(ANIMATION_TYPE::ATTACK, ANIMATION_ATTACK_TYPE::WAIT_ATTACK2);
	}
	else {
		SetAnimationType(ANIMATION_TYPE::ATTACK, ANIMATION_ATTACK_TYPE::WAIT_ATTACK1);
	}
}
void Player::WalkAttack()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_D)) {
		m_isAnimationMirror = false;
		m_force.x = 1000.0f;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_A)) {
		m_isAnimationMirror = true;
		m_force.x = -1000.0f;
	}
	if (++m_animationAttackInterval > 5) {
		m_animationAttackInterval = 0;
		SetAnimationType(ANIMATION_TYPE::ATTACK, ANIMATION_ATTACK_TYPE::WALK_ATTACK2);
	}
	else {
		SetAnimationType(ANIMATION_TYPE::ATTACK, ANIMATION_ATTACK_TYPE::WALK_ATTACK1);
	}
}

void Player::WaitJump()
{
	// ��x��������Ђ������������`���悤�ɒn�ʂɒ��n����B
	// TOD* update�ōŏ��ɏ��������Ă邩��A�������ǂ��ɂ�����K�v������
	if (GetAnimationType() == ANIMATION_TYPE::JUMP) {

	}
	else {
		SetAnimationType(ANIMATION_TYPE::JUMP, ANIMATION_JUMP_TYPE::WAIT_JUMP_1);
	}
}

bool Player::Render()
{
	if (m_isAnimationMirror) {
		// TODO:�~���[�����O�@�\���ł���΃R�����g�A�E�g���O��
		//m_pObjects[m_animationType][m_animationActionType].Render(m_pos.x, m_pos.y, m_pos.x - 32.0f, m_pos.y + 32.0f , true);
		m_pObjects[m_animationType][m_animationActionType].Render(m_pos.x, m_pos.y, m_pos.x + RENDER_SIZE_X, m_pos.y + RENDER_SIZE_Y);
	}
	else {
		m_pObjects[m_animationType][m_animationActionType].Render(m_pos.x, m_pos.y, m_pos.x + RENDER_SIZE_X, m_pos.y + RENDER_SIZE_Y);
	}

	return true;
}
void Player::SetAnimationType(Player::ANIMATION_TYPE type, Player::ANIMATION_WAIT_TYPE type2)
{
	m_animationType = CAST_I(type);
	m_animationActionType = CAST_I(type2);
}
void Player::SetAnimationType(Player::ANIMATION_TYPE type, Player::ANIMATION_WALK_TYPE type2)
{
	m_animationType = CAST_I(type);
	m_animationActionType = CAST_I(type2);
}
void Player::SetAnimationType(Player::ANIMATION_TYPE type, Player::ANIMATION_ATTACK_TYPE type2)
{
	m_animationType = CAST_I(type);
	m_animationActionType = CAST_I(type2);
}
void Player::SetAnimationType(ANIMATION_TYPE type, ANIMATION_JUMP_TYPE type2)
{
	m_animationType = CAST_I(type);
	m_animationActionType = CAST_I(type2);
}



Player::ANIMATION_TYPE Player::GetAnimationType()
{
	return (Player::ANIMATION_TYPE)m_animationType;
}