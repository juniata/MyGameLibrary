#include	"DxLibrary/DX_Library.h"
#include	"Player.h"
#include	"Games/SceneMain/Stage.h"
#include	"Games/MapChip/MapChip.h"
#include	"Games/GameDefine.h"

//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
Player::Player() :
	m_pos(DirectX::XMFLOAT2(0.0f,0.0f)),
	m_move(DirectX::XMFLOAT2(0.0f, 0.0f)),
	m_animationType(0),
	m_animationActionType(0),
	m_animationWalkInterval(0),
	m_animationAttackInterval(0),
	m_actionType(ACTION_TYPE::WAIT),
	m_pObjects(new DX_2DObject*[CAST_I(ANIMATION_TYPE::MAX)]),
	m_isAnimationMirror(false)
{
	m_jump.interval = 0;
	m_jump.isJump	= false;
	m_jump.moveY	= 0.0f;

	m_pObjects[CAST_I(ANIMATION_TYPE::WAIT)]	= new DX_2DObject[CAST_I(ANIMATION_WAIT_TYPE::MAX)];
	m_pObjects[CAST_I(ANIMATION_TYPE::WALK)]	= new DX_2DObject[CAST_I(ANIMATION_WALK_TYPE::MAX)];
	m_pObjects[CAST_I(ANIMATION_TYPE::ATTACK)]	= new DX_2DObject[CAST_I(ANIMATION_ATTACK_TYPE::MAX)];
	m_pObjects[CAST_I(ANIMATION_TYPE::JUMP)]	= new DX_2DObject[CAST_I(ANIMATION_JUMP_TYPE::MAX)];
}


//-----------------------------------------------------------------------------------------
//
//  メンバ変数を解放する
//
//-----------------------------------------------------------------------------------------
Player::~Player()
{
	for (int i = 0; i < CAST_I(ANIMATION_TYPE::MAX); ++i)
	{
		DELETE_OBJ_ARRAY(m_pObjects[i]);
	}
	DELETE_OBJ_ARRAY(m_pObjects);
}

//-----------------------------------------------------------------------------------------
//
//  オブジェクト等の生成を行う
//
//-----------------------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------------------
//
//  座標、行動を更新する
//
//-----------------------------------------------------------------------------------------
bool Player::Update(Stage* pStage)
{
	ACTION_TYPE pastActionType = m_actionType;

	KeyUpdate();
	
	if (pastActionType != m_actionType) {
		m_animationAttackInterval = 0;
		m_animationWalkInterval = 0;
	}

	switch (m_actionType) {
		// 止っているときの行動
	case ACTION_TYPE::WAIT:				Wait();			break;	// 待機
	case ACTION_TYPE::WAIT_JUMP_1:		WaitJump();		break;	// その場でジャンプ
	case ACTION_TYPE::WAIT_ATTACK_1:	WaitAttack();	break;	// その場で攻撃

		// 歩いてる時の行動
	case ACTION_TYPE::WALK:				Walk();			break;	//	歩く
	case ACTION_TYPE::WALK_ATTACK_1:	WalkAttack();	break;	//	歩きながら攻撃
	case ACTION_TYPE::WALK_JUMP:		WalkJump();		break;	//	歩きながらジャンプ
	}
	
	Collision(pStage);

	Move();

	return true;
}


//-----------------------------------------------------------------------------------------
//
//  プレイヤーを描画する
//
//-----------------------------------------------------------------------------------------
bool Player::Render()
{
	bool result = false;

	result = m_pObjects[m_animationType][m_animationActionType].Render(m_pos.x, m_pos.y, m_pos.x + RENDER_SIZE_X, m_pos.y + RENDER_SIZE_Y, m_isAnimationMirror);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  移動処理
//
//-----------------------------------------------------------------------------------------
void Player::Move()
{
	// 座標算出
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	m_move.x = m_move.y = 0.0f;
}

//-----------------------------------------------------------------------------------------
//
//  衝突判定
//
//-----------------------------------------------------------------------------------------
void Player::Collision(Stage* pStage)
{
	DirectX::XMFLOAT2 diff(0.0f, 0.0f);
	if (pStage->IsHit(m_pos, &diff)) {
		// 座標を調整する
		m_pos.x -= diff.x;
		m_pos.y -= diff.y;
	}
}

//-----------------------------------------------------------------------------------------
//
//  プレイヤーを描画する
//
//-----------------------------------------------------------------------------------------
void Player::KeyUpdate()
{
	// ジャンプ中ではない場合
	if (m_jump.isJump == false)
	{
		// ADなら移動
		if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RIGHT) || DX_Input::IsKeyDown(DX_INPUT_KEY::DX_LEFT))
		{
			m_actionType = ACTION_TYPE::WALK;
			
			// Xなら移動攻撃
			if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_X)) {
				m_actionType = ACTION_TYPE::WALK_ATTACK_1;
			}
			else if(DX_Input::IsKeyDown(DX_INPUT_KEY::DX_Z)){
				// Zなら移動ジャンプ
				m_actionType = ACTION_TYPE::WALK_JUMP;
				m_jump.isJump = true;
			}
		}
		// Xならその場で攻撃
		else if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_X)) {
			m_actionType = ACTION_TYPE::WAIT_ATTACK_1;
		}
		// Zならその場でジャンプ
		else if (DX_Input::IsKeyHit(DX_INPUT_KEY::DX_Z)) {
			m_actionType = ACTION_TYPE::WAIT_JUMP_1;
			m_jump.isJump = true;
		}
		else {
			m_actionType = ACTION_TYPE::WAIT;
		}
	}
	// ジャンプ中の場合
	else {
		// AD押したなら左右移動でできるようにする
		// ジャンプ歩き関数を作成する必要がある
		// する?
	}
}


//-----------------------------------------------------------------------------------------
//
//  待機
//
//-----------------------------------------------------------------------------------------
void Player::Wait()
{
	SetAnimationType(ANIMATION_TYPE::WAIT, ANIMATION_WAIT_TYPE::WAIT1);
}

//-----------------------------------------------------------------------------------------
//
//  待機攻撃
//
//-----------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------
//
//  待機ジャンプ
//
//-----------------------------------------------------------------------------------------
void Player::WaitJump()
{
	// 一度押したらひたすら放物線を描くように地面に着地する。
	// TOD* updateで最初に初期化してるから、そこをどうにかする必要がある
	if (GetAnimationType() == ANIMATION_TYPE::JUMP) {
		m_jump.moveY -= 0.2f;
		++m_jump.interval;
	}
	else {
		m_jump.interval = 0;
		m_jump.moveY = -5.0f;
		SetAnimationType(ANIMATION_TYPE::JUMP, ANIMATION_JUMP_TYPE::WAIT_JUMP_1);
	}

	// ジャンプ時間を設定
	const int JUMP_TIME = 10;

	if (m_jump.interval > JUMP_TIME) {
		m_jump.moveY = 0.0f;
		m_jump.isJump = false;

		//TODO:重力がないから。。。。
	}

	m_move.y = m_jump.moveY;
}

//-----------------------------------------------------------------------------------------
//
//  歩き
//
//-----------------------------------------------------------------------------------------
void Player::Walk()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RIGHT)) {
		m_move.x = 5.0f;
		m_isAnimationMirror = false;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_LEFT)) {
		m_move.x = -5.0f;
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


//-----------------------------------------------------------------------------------------
//
//  歩き攻撃
//
//-----------------------------------------------------------------------------------------
void Player::WalkAttack()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RIGHT)) {
		m_isAnimationMirror = false;
		m_move.x = 5.0f;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_LEFT)) {
		m_isAnimationMirror = true;
		m_move.x = -5.0f;
	}
	if (++m_animationAttackInterval > 5) {
		m_animationAttackInterval = 0;
		SetAnimationType(ANIMATION_TYPE::ATTACK, ANIMATION_ATTACK_TYPE::WALK_ATTACK2);
	}
	else {
		SetAnimationType(ANIMATION_TYPE::ATTACK, ANIMATION_ATTACK_TYPE::WALK_ATTACK1);
	}
}

//-----------------------------------------------------------------------------------------
//
//  歩きジャンプ
//
//-----------------------------------------------------------------------------------------
void Player::WalkJump()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RIGHT)) {
		m_move.x = 5.0f;
		m_isAnimationMirror = false;
	}
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_LEFT)) {
		m_move.x = -5.0f;
		m_isAnimationMirror = true;
	}

	// 一度押したらひたすら放物線を描くように地面に着地する。
	// TOD* updateで最初に初期化してるから、そこをどうにかする必要がある
	if (GetAnimationType() == ANIMATION_TYPE::JUMP) {
		m_jump.moveY -= 0.2f;
		++m_jump.interval;
	}
	else {
		m_jump.interval = 0;
		m_jump.moveY = -5.0f;
		// TODO:歩きジャンプ作ったらそっちを設定する
		SetAnimationType(ANIMATION_TYPE::JUMP, ANIMATION_JUMP_TYPE::WAIT_JUMP_1);
	}

	// ジャンプ時間を設定
	const int JUMP_TIME = 10;

	if (m_jump.interval > JUMP_TIME) {
		m_jump.moveY = 0.0f;
		m_jump.isJump = false;

		//TODO:重力がないから。。。。
	}
	m_move.y = m_jump.moveY;
}


//-----------------------------------------------------------------------------------------
//
//  アニメーションタイプを取得する
//
//-----------------------------------------------------------------------------------------
Player::ANIMATION_TYPE Player::GetAnimationType()
{
	return (Player::ANIMATION_TYPE)m_animationType;
}

//-----------------------------------------------------------------------------------------
//
//  アニメーションタイプを取得する
//
//-----------------------------------------------------------------------------------------
template<class TYPE> void Player::SetAnimationType(ANIMATION_TYPE type, TYPE type2)
{
	m_animationType			= CAST_I(type);
	m_animationActionType	= CAST_I(type2);
}
template void Player::SetAnimationType(Player::ANIMATION_TYPE type, Player::ANIMATION_WAIT_TYPE);
template void Player::SetAnimationType(Player::ANIMATION_TYPE type, Player::ANIMATION_WALK_TYPE);
template void Player::SetAnimationType(Player::ANIMATION_TYPE type, Player::ANIMATION_ATTACK_TYPE);
template void Player::SetAnimationType(Player::ANIMATION_TYPE type, Player::ANIMATION_JUMP_TYPE);

