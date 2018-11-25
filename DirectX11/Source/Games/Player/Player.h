#pragma once

class Player
{
public:
	Player();
	~Player();

	bool Initialize(const DirectX::XMFLOAT2& initPos);
	bool Update();
	bool Render();

private:
	DX_2DObject* m_pObjects[3];
	DirectX::XMFLOAT2 m_pos;
	int m_animationIndex;
	int m_animationInterval;
	bool m_IsMove;

	void Move();
};