#include	"SkyLibrary.h"

Button::Button() : m_input(DX_Input::GetInstance())
{

}

Button::~Button()
{

}

//-----------------------------------------------------------------------------------------
//
//  �摜�̏�ŃN���b�N�������ǂ���
//
//-----------------------------------------------------------------------------------------
bool Button::IsClick()const
{
	auto isClick = false;

	if (m_input->IsMouseButtonClick(DX_MOUSE_BUTTON_KIND::BUTTON_L))
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

	if (m_input->IsMouseButtonDown(DX_MOUSE_BUTTON_KIND::BUTTON_L))
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

	if (m_input->IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND::BUTTON_L))
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
	m_input->GetMouseClientPos(&mousePos);

	if (m_rectPos.left <= mousePos.x && m_rectPos.right >= mousePos.x &&
		m_rectPos.top <= mousePos.y && m_rectPos.bottom >= mousePos.y)
	{
		isInRect = true;
	}

	return isInRect;
}