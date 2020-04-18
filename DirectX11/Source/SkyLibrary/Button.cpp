#include	"SkyLibrary.h"

//-----------------------------------------------------------------------------------------
//
//  �摜�̏�ŃN���b�N�������ǂ���
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
//  �摜�̏�ŃN���b�N���Ă���Œ����ǂ���
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
//  �摜�̏�ŃN���b�N���������Ƃ��ǂ���
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
//  �摜�̏�Ƀ}�E�X�|�C���^�����邩�ǂ���
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