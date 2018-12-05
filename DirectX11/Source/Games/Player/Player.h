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
	// 基盤となるアニメーションタイプ
	enum class ANIMATION_TYPE : int {
		WAIT,
		WALK,
		ATTACK,
		JUMP,
		MAX,
	};
	// 待機モーショ
	enum class ANIMATION_WAIT_TYPE : int {
		WAIT1,
		MAX,
	};
	// 歩きモーション
	enum class ANIMATION_WALK_TYPE : int {
		WALK1,
		WALK2,
		MAX,
	};
	// 攻撃モーション
	enum class ANIMATION_ATTACK_TYPE : int {
		// 止まった状態で攻撃
		WAIT_ATTACK1,
		WAIT_ATTACK2,
		// 歩きながら攻撃
		WALK_ATTACK1,
		WALK_ATTACK2,
		MAX,
	};

	// ジャンプモーション
	enum class ANIMATION_JUMP_TYPE : int {
		// 止まった状態でジャンプ
		WAIT_JUMP_1,
		MAX,
	};
	enum class ACTION_TYPE : int {

		// 止まってる時のアクション
		WAIT,			// 待機
		WAIT_ATTACK_1,	// 止まって攻撃
		WAIT_JUMP_1,	// 止まってジャンプ

		// 歩いてる時のアクション
		WALK,			// 歩く
		WALK_ATTACK_1,	// 歩きながら攻撃
		WALK_JUMP,		// 歩きながらジャンプ
	};
	int m_animationType;
	int m_animationActionType;

	int m_animationWalkInterval;
	int m_animationAttackInterval;
	bool m_isAnimationMirror;
	ACTION_TYPE m_actionType;

	DX_2DObject** m_pObjects;

	DirectX::XMFLOAT2 m_pos;
	DirectX::XMFLOAT2 m_move;

	struct tagJump{
		float moveY;
		bool isJump;
		int interval;
	};
	tagJump m_jump;

	void KeyUpdate();
	void Wait();
	void WaitAttack();
	void WaitJump();

	void Walk();
	void WalkAttack();
	void WalkJump();

	void Move();
	void Collision(Stage* pStage);

	ANIMATION_TYPE GetAnimationType();
	template<class TYPE> void SetAnimationType(ANIMATION_TYPE type, TYPE type2);

};