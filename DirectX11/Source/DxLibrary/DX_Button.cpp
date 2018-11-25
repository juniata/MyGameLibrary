#include"DX_Library.h"

DX_Button::DX_Button() :
	m_pObject(new DX_2DObject()),
	m_pos(DirectX::XMFLOAT2(0.0f, 0.0f)),
	m_size(DirectX::XMFLOAT2(0.0f, 0.0f))
{}

DX_Button::~DX_Button()
{
	DELETE_OBJ(m_pObject);
}
bool DX_Button::Initialize(const char* pFilepath, DirectX::XMFLOAT2& pos, DirectX::XMFLOAT2& size)
{
	m_pos = pos;
	m_size = size;

	return m_pObject->Initialize(pFilepath);
}
bool DX_Button::Render()
{
	return m_pObject->Render(m_pos, m_size);
}

bool DX_Button::IsClick()const
{
	bool click = false;

	if (DX_Input::IsMouseButtonClick(DX_MOUSE_BUTTON_KIND::BUTTON_L)) {
		DirectX::XMFLOAT2 mousePos;
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
		DirectX::XMFLOAT2 mousePos;
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
		DirectX::XMFLOAT2 mousePos;
		DX_Input::GetMouseClientPos(&mousePos);
		if (m_pos.x <= mousePos.x && (m_pos.x + m_size.x) >= mousePos.x &&
			m_pos.y <= mousePos.y && (m_pos.y + m_size.y) >= mousePos.y) {
			click = true;
		}
	}

	return click;
}