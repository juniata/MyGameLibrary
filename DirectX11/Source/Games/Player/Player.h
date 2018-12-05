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
	// ��ՂƂȂ�A�j���[�V�����^�C�v
	enum class ANIMATION_TYPE : int {
		WAIT,
		WALK,
		ATTACK,
		JUMP,
		MAX,
	};
	// �ҋ@���[�V��
	enum class ANIMATION_WAIT_TYPE : int {
		WAIT1,
		MAX,
	};
	// �������[�V����
	enum class ANIMATION_WALK_TYPE : int {
		WALK1,
		WALK2,
		MAX,
	};
	// �U�����[�V����
	enum class ANIMATION_ATTACK_TYPE : int {
		// �~�܂�����ԂōU��
		WAIT_ATTACK1,
		WAIT_ATTACK2,
		// �����Ȃ���U��
		WALK_ATTACK1,
		WALK_ATTACK2,
		MAX,
	};

	// �W�����v���[�V����
	enum class ANIMATION_JUMP_TYPE : int {
		// �~�܂�����ԂŃW�����v
		WAIT_JUMP_1,
		MAX,
	};
	enum class ACTION_TYPE : int {

		// �~�܂��Ă鎞�̃A�N�V����
		WAIT,			// �ҋ@
		WAIT_ATTACK_1,	// �~�܂��čU��
		WAIT_JUMP_1,	// �~�܂��ăW�����v

		// �����Ă鎞�̃A�N�V����
		WALK,			// ����
		WALK_ATTACK_1,	// �����Ȃ���U��
		WALK_JUMP,		// �����Ȃ���W�����v
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