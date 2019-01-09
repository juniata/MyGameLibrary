#include"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  画像の上でクリックしたかどうか
//
//-----------------------------------------------------------------------------------------
bool DX_Button::IsClick()const
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
bool DX_Button::IsClickDown()const
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
bool DX_Button::IsClickRelease()const
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
bool DX_Button::IsInRect() const
{
	auto isInRect = false;

	DirectX::XMFLOAT2 mousePos;
	DX_Input::GetMouseClientPos(&mousePos);

	if (m_rectPos.left	<= mousePos.x && m_rectPos.right	>= mousePos.x && 
		m_rectPos.top	<= mousePos.y && m_rectPos.bottom	>= mousePos.y)
	{
		isInRect = true;
	}

	return isInRect;
}