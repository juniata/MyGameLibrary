#ifndef __DX_BUTTON_H_
#define __DX_BUTTON_H_

class DX_Button
{
public:
	DX_Button();
	DX_Button(const char* pFilepath, DirectX::XMFLOAT2& pos, DirectX::XMFLOAT2& size);
	~DX_Button();

	void LoadTexture(const char* pFilepath);
	bool Render();

	bool IsClick()const;
	bool IsClickDown()const;
	bool IsClickRelease()const;
private:
	DX_2DObject*	m_pObject;
	DirectX::XMFLOAT2 m_pos;
	DirectX::XMFLOAT2 m_size;
};
#endif // !__DX_BUTTON_H_
