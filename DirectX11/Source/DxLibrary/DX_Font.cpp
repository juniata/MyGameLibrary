#include "DxLibrary\\DX_Library.h"

DX_Font::DX_Font() : m_pFont(nullptr)
{
	m_pFont = DX_TextureManager::GetTexture("font.png");
}