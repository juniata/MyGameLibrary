#pragma once

class Stage;
class Player
{
public:
	Player();
	~Player();

	bool Initialize(const DirectX::XMFLOAT2& initPos);
	bool Update(Stage* pStage);
	bool Render();

private:
	enum class ANIMATION_TYPE : int {
		WAIT,
		WALK,
		ATTACK,
		JUMP,
		MAX,
	};
	enum class ANIMATION_WAIT_TYPE : int {
		WAIT1,
		MAX,
	};
	enum class ANIMATION_WALK_TYPE : int {
		WALK1,
		WALK2,
		MAX,
	};
	enum class ANIMATION_ATTACK_TYPE : int {
		// 止まった状態で攻撃
		WAIT_ATTACK1,
		WAIT_ATTACK2,
		// 歩きながら攻撃
		WALK_ATTACK1,
		WALK_ATTACK2,
		MAX,
	};
	enum class ANIMATION_JUMP_TYPE : int {
		// 止まった状態でジャンプ
		WAIT_JUMP_1,
		MAX,
	};
	enum class ACTION_TYPE : int {

		// 止まり
		WAIT,

		// 歩く
		WALK_1,

		// 止まって攻撃
		ATTACK_WAIT_1,

		// 歩きながら攻撃
		ATTACK_WALK_1,

		// 止まってジャンプ
		JUMP_WAIT_1,
	};
	int m_animationType;
	int m_animationActionType;

	int m_animationWalkInterval;
	int m_animationAttackInterval;
	bool m_isAnimationMirror;
	ACTION_TYPE m_actionType;

	DX_2DObject** m_pObjects;

	static const int MASS = 150;
	DirectX::XMFLOAT2 m_pos;	//	座標
	DirectX::XMFLOAT2 m_initialVelocity;	//	初速度 m/s
	DirectX::XMFLOAT2 m_force;	// 合力
	DirectX::XMFLOAT2 m_velocity;	// 速度 s
	DirectX::XMFLOAT2 m_accelerator;	//	加速度 m/s^2
	static const float GRAVITY;	// G =m/s^2

	void Wait();
	void Walk();
	void WaitAttack();
	void WalkAttack();
	void WaitJump();
	void Move();
	void Collision(Stage* pStage);

	ANIMATION_TYPE GetAnimationType();
	void SetAnimationType(ANIMATION_TYPE type, ANIMATION_WAIT_TYPE type2);
	void SetAnimationType(ANIMATION_TYPE type, ANIMATION_WALK_TYPE type2);
	void SetAnimationType(ANIMATION_TYPE type, ANIMATION_ATTACK_TYPE type2);
	void SetAnimationType(ANIMATION_TYPE type, ANIMATION_JUMP_TYPE type2);
};