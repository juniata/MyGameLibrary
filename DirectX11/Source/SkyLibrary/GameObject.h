#pragma once

class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() {};
	virtual void Initialize() {}
	virtual void Destroy() {}
};