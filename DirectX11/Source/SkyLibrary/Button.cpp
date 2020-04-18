#include	"SkyLibrary.h"

//-----------------------------------------------------------------------------------------
//
//  画像の上でクリックしたかどうか
//
//-----------------------------------------------------------------------------------------
bool Button::IsClick()const
{
	auto isClick = false;

	if (DX_Input::IsMouseButtonClick(DX_MOUSE_BUTTON_KIND::BUTTON_L))
	{
		isClick = IsInRect();
	}

	return isClick;
}

//-----------------------------------------------------------------------------------------
//
//  画像の上でクリックしている最中かどうか
//
//-----------------------------------------------------------------------------------------
bool Button::IsClickDown()const
{
	auto isClick = false;

	if (DX_Input::IsMouseButtonDown(DX_MOUSE_BUTTON_KIND::BUTTON_L))
	{
		isClick = IsInRect();
	}

	return isClick;
}

//-----------------------------------------------------------------------------------------
//
//  画像の上でクリックをしたあとかどうか
//
//-----------------------------------------------------------------------------------------
bool Button::IsClickRelease()const
{
	auto isClick = false;

	if (DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND::BUTTON_L))
	{
		isClick = IsInRect();
	}

	return isClick;
}

//-----------------------------------------------------------------------------------------
//
//  画像の上にマウスポインタがあるかどうか
//
//-----------------------------------------------------------------------------------------
bool Button::IsInRect() const
{
	auto isInRect = false;

	DirectX::XMFLOAT2 mousePos;
	DX_Input::GetMouseClientPos(&mousePos);

	if (m_rectPos.left <= mousePos.x && m_rectPos.right >= mousePos.x &&
		m_rectPos.top <= mousePos.y && m_rectPos.bottom >= mousePos.y)
	{
		isInRect = true;
	}

	return isInRect;
}