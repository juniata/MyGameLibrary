#include"DX_Library.h"
using namespace DirectX;

DX_Button::DX_Button() :
	m_pObject(new DX_2DObject())
{}
DX_Button::DX_Button(const char* pFilepath, XMFLOAT2& pos, XMFLOAT2& size) :
	m_pObject(new DX_2DObject(pFilepath)),
	m_pos(pos),
	m_size(size)
{
}
DX_Button::~DX_Button()
{
	DELETE_OBJ(m_pObject);
}
void DX_Button::LoadTexture(const char* pFilepath)
{
	m_pObject->LoadTexture(pFilepath);
}
bool DX_Button::Render()
{
	return m_pObject->Render(m_pos, m_size);
}

bool DX_Button::IsClick()const
{
	bool click = false;

	if (DX_Input::IsMouseButtonClick(DX_MOUSE_BUTTON_KIND::BUTTON_L)) {
		XMFLOAT2 mousePos;
		DX_Input::GetMouseClientPos(&mousePos);
		if (m_pos.x <= mousePos.x && (m_pos.x + m_size.x) >= mousePos.x &&
			m_pos.y <= mousePos.y && (m_pos.y + m_size.y) >= mousePos.y) {
			click = true;
		}
	}

	return click;
}
bool DX_Button::IsClickDown()const
{
	bool click = false;

	if (DX_Input::IsMouseButtonDown(DX_MOUSE_BUTTON_KIND::BUTTON_L)) {
		XMFLOAT2 mousePos;
		DX_Input::GetMouseClientPos(&mousePos);
		if (m_pos.x <= mousePos.x && (m_pos.x + m_size.x) >= mousePos.x &&
			m_pos.y <= mousePos.y && (m_pos.y + m_size.y) >= mousePos.y) {
			click = true;
		}
	}

	return click;
}
bool DX_Button::IsClickRelease()const
{
	bool click = false;

	if (DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND::BUTTON_L)) {
		XMFLOAT2 mousePos;
		DX_Input::GetMouseClientPos(&mousePos);
		if (m_pos.x <= mousePos.x && (m_pos.x + m_size.x) >= mousePos.x &&
			m_pos.y <= mousePos.y && (m_pos.y + m_size.y) >= mousePos.y) {
			click = true;
		}
	}

	return click;
}