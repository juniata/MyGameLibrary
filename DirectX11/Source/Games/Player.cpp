#include	"DxLibrary\DX_Library.h"
#include	"Player.h"

//-----------------------------------------------------------------------------------------
//
//  ‰Šú‰»
//
//-----------------------------------------------------------------------------------------
Player::Player()
{
	obj = new DX_2DObject("player.png");
	size = DirectX::XMFLOAT2(SIZE, SIZE);
	float basePosX = (DX_System::GetWindowWidth() - size.x) * 0.5f;
	float basePosY = (DX_System::GetWindowHeight() - size.y);
	pos = DirectX::XMFLOAT2(basePosX, basePosY);
}

//-----------------------------------------------------------------------------------------
//
//  ‰ð•ú
//
//-----------------------------------------------------------------------------------------
Player::~Player()
{
	DELETE_OBJ(obj);
}

//-----------------------------------------------------------------------------------------
//
//  ˆÚ“®
//
//-----------------------------------------------------------------------------------------
void Player::Move()
{
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_LEFT))
	{
		pos.x -= Player::MOVE_SPEED;
	}

	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_RIGHT))
	{
		pos.x += Player::MOVE_SPEED;
	}
	
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_UP))
	{
		pos.y -= Player::MOVE_SPEED;
	}
	
	if (DX_Input::IsKeyDown(DX_INPUT_KEY::DX_DOWN))
	{
		pos.y += Player::MOVE_SPEED;
	}
}

//-----------------------------------------------------------------------------------------
//
//  XV
//
//-----------------------------------------------------------------------------------------
void Player::Update()
{
	Move();
}

//-----------------------------------------------------------------------------------------
//
//  •`‰æ
//
//-----------------------------------------------------------------------------------------
void Player::Render()
{
	obj->Render(pos, size);
}