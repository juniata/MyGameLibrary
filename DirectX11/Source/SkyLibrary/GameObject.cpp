#include "SkyLibrary.h"

GameObject::GameObject() : 
	m_system(DX_System::GetInstance()),
	m_input(DX_Input::GetInstance()),
	m_shaderManager(DX_ShaderManager::GetInstance()),
	m_textureManager(DX_TextureManager::GetInstance())
{
}

bool GameObject::Initialize()
{
	return true;
}

bool GameObject::Destroy()
{
	return true;
}