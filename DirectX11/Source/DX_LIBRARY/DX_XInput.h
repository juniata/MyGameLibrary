#ifndef __DX_X_INPUT_H_
#define __DX_X_INPUT_H_

enum class XINPUT_KEY{
	UP,			
	DOWN,		
	LEFT,
	RIGHT,
	START,
	BACK,
	LEFT_THUMB,
	RIGHT_THUMB,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	A,
	B,
	X,
	Y
};
//****************************************************************************************************
//
//	XInput
//
//****************************************************************************************************
class XInput
{
private:
	static const int KEY_MAX = 14;
	struct tagInputInfo{
		XINPUT_STATE state;
		int id;
		bool keys[KEY_MAX];
		bool prevKeys[KEY_MAX];
	}m_players[XUSER_MAX_COUNT];

	int m_connectCount;	
	int m_buttonBitJuage[KEY_MAX];
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ��̏�����
	//
	//------------------------------------------------------------------------------
	XInput();

	//------------------------------------------------------------------------------
	//
	//  @brief		��n��
	//
	//------------------------------------------------------------------------------
	~XInput();

	//------------------------------------------------------------------------------
	//
	//  @brief		�����ĂȂ����ȊO�̔����𓾂�
	//	@param[in]	key		DX_XINPUT_KEY
	//	@param[in]	id		�ڑ�ID
	//	@return		true:������	false:�����ĂȂ�
	//
	//------------------------------------------------------------------------------
	bool IsKey(dx_library::XINPUT_KEY key,const int id = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		�����Ă���Ԃ̔����𓾂�
	//	@param[in]	key		DX_XINPUT_KEY
	//	@param[in]	id		�ڑ�ID
	//	@return		true:�����Ă���		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	bool IsKeyDown(dx_library::XINPUT_KEY key, const int id = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		�������u�Ԃ̔����𓾂�
	//	@param[in]	key		DX_XINPUT_KEY
	//	@return		true:������		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	bool IsKeyRelease(dx_library::XINPUT_KEY key, const int id = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		�������u�Ԃ̔����𓾂�
	//	@param[in]	key		DX_XINPUT_KEY
	//	@return		true:������		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	bool IsKeyHit(dx_library::XINPUT_KEY key, const int id = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		���ׂẴ{�^���̓��͂���t
	//
	//------------------------------------------------------------------------------
	void Update();


private:
	void Connect(const int playerNum);
};
#endif // !__DX_X_INPUT_H_
