#pragma once

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() {};
	virtual bool Initialize();
	virtual bool Destroy();

protected:
	DX_System* m_system;
	DX_Input* m_input;
	DX_ShaderManager* m_shaderManager;
	DX_TextureManager* m_textureManager;
};