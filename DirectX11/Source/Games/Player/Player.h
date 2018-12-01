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
		// é~Ç‹Ç¡ÇΩèÛë‘Ç≈çUåÇ
		WAIT_ATTACK1,
		WAIT_ATTACK2,
		// ï‡Ç´Ç»Ç™ÇÁçUåÇ
		WALK_ATTACK1,
		WALK_ATTACK2,
		MAX,
	};
	enum class ANIMATION_JUMP_TYPE : int {
		// é~Ç‹Ç¡ÇΩèÛë‘Ç≈ÉWÉÉÉìÉv
		WAIT_JUMP_1,
		MAX,
	};
	enum class ACTION_TYPE : int {

		// é~Ç‹ÇË
		WAIT,

		// ï‡Ç≠
		WALK_1,

		// é~Ç‹Ç¡ÇƒçUåÇ
		ATTACK_WAIT_1,

		// ï‡Ç´Ç»Ç™ÇÁçUåÇ
		ATTACK_WALK_1,

		// é~Ç‹Ç¡ÇƒÉWÉÉÉìÉv
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
	DirectX::XMFLOAT2 m_pos;	//	ç¿ïW
	DirectX::XMFLOAT2 m_initialVelocity;	//	èâë¨ìx m/s
	DirectX::XMFLOAT2 m_force;	// çáóÕ
	DirectX::XMFLOAT2 m_velocity;	// ë¨ìx s
	DirectX::XMFLOAT2 m_accelerator;	//	â¡ë¨ìx m/s^2
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