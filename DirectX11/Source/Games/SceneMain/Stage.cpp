#include	"DxLibrary/DX_Library.h"
#include	"Games/MapChip/MapChip.h"
#include	"Stage.h"

Stage::Stage() : 
	m_pMapChip(new MapChip()),
	m_pSkyMapChip(new MapChip())
{
	ZeroMemory(m_mapChips, sizeof(m_mapChips));
}

Stage::~Stage()
{
	DELETE_OBJ(m_pMapChip);
	DELETE_OBJ(m_pSkyMapChip);
}
bool Stage::Initialize()
{
	for (int y = 0; y < CHIP_Y_NUM - 3; ++y)
	{
		for (int x = 0; x < CHIP_X_NUM; ++x)
		{
			m_mapChips[y][x] = CAST_I(CHIP_TYPE::SKY);
		}
	}
	for (int y = CHIP_Y_NUM - 3; y < CHIP_Y_NUM; ++y)
	{
		for (int x = 0; x < CHIP_X_NUM; ++x)
		{
			m_mapChips[y][x] = CAST_I(CHIP_TYPE::BLOCK);
		}

	};

	int wallCount = 0;
	int sykCount = 0;
	for (int y = 0; y < CHIP_Y_NUM; ++y)
	{
		for (int x = 0; x < CHIP_X_NUM; ++x)
		{
			switch (m_mapChips[y][x]) {
			case CAST_I(CHIP_TYPE::SKY):
				++sykCount;
				break;
			case CAST_I(CHIP_TYPE::BLOCK):
				++wallCount;
				break;
			}
		}
	}
	if (false == m_pMapChip->Initialize("SceneMain\\wall.png", wallCount)) {
		return false;
	}

	if (false == m_pSkyMapChip->Initialize("SceneMain\\sky.png", sykCount)) {
		return false;
	}
	
	int wallIndex = 0;
	int skyIndex = 0;
	for (int y = 0; y < CHIP_Y_NUM; ++y)
	{
		for (int x = 0; x < CHIP_X_NUM; ++x)
		{
			switch (m_mapChips[y][x]) {
			case CAST_I(CHIP_TYPE::SKY):
				m_pSkyMapChip->SetPos(skyIndex++, y, x);
				break;
			case CAST_I(CHIP_TYPE::BLOCK):
				m_pMapChip->SetPos(wallIndex++, y, x);
				break;
			}
		}
	}

	return true;
}

bool Stage::Render()
{
	bool result = true;

	result = m_pMapChip->Render();
	result = m_pSkyMapChip->Render();
	
	return result;
}

DirectX::XMFLOAT2 Stage::GetInitPlayerPos() const
{
	DirectX::XMFLOAT2 pos = m_pMapChip->GetPos(0);;
	pos.y -= 32.0f;

	return pos;
}