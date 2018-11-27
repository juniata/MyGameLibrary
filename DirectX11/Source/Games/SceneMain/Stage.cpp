#include	"DxLibrary/DX_Library.h"
#include	"Games/MapChip/MapChip.h"
#include	"Stage.h"
#include	<time.h>

Stage::Stage() : 
	m_pMapChip(new MapChip()),
	m_pSkyMapChip(new MapChip()),
	m_chipNumX(0),
	m_chipNumY(0),
	m_mapChips(nullptr)
{
}

Stage::~Stage()
{
	DELETE_OBJ(m_pMapChip);
	DELETE_OBJ(m_pSkyMapChip);
	for (int y = 0; y < m_chipNumY; ++y)
	{
		DELETE_OBJ_ARRAY(m_mapChips[y]);
	}
	DELETE_OBJ_ARRAY(m_mapChips);
}
bool Stage::Initialize()
{
	unsigned int windowWidth = DX_System::GetWindowWidth();
	unsigned int windowHeight = DX_System::GetWindowHeight();

	// 描画サイズ 32 * 32
	// * num は何画面分作成するか
	int num = 2;
	m_chipNumX = (windowWidth / 32) * num;
	m_chipNumY = (windowHeight / 32) * num;

	m_mapChips = new int*[m_chipNumY];
	for (int y = 0; y < m_chipNumY; ++y)
	{
		m_mapChips[y] = new int[m_chipNumX];
	}
	// いったんは全画面分作成する
	// 一画面に描画するブロック数を設定する。(画面サイズの変更に対応する)
	for (int y = 0; y < m_chipNumY - 3; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			m_mapChips[y][x] = CAST_I(CHIP_TYPE::SKY);
		}
	}
	for (int y = m_chipNumY - 3; y < m_chipNumY; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			m_mapChips[y][x] = CAST_I(CHIP_TYPE::BLOCK);
		}
	};

	srand(CAST_UI(time(NULL)));
	for (int i = 0; i < 20; ++i) {
		m_mapChips[rand() % (m_chipNumY - 5)][rand() % (m_chipNumX - 1) + 1] = CAST_I(CHIP_TYPE::BLOCK);
	}

	int wallCount = 0;
	int sykCount = 0;
	for (int y = 0; y < m_chipNumY; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
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
	float posX = 0;
	float posY = windowHeight - 32.0f;
	for (int y = m_chipNumY - 1; y > 0; --y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			switch (m_mapChips[y][x]) {
			case CAST_I(CHIP_TYPE::SKY):
				m_pSkyMapChip->SetPos(skyIndex++, posY, posX);
				break;
			case CAST_I(CHIP_TYPE::BLOCK):
				m_pMapChip->SetPos(wallIndex++, posY, posX);
				break;
			}
			posX += 32.0f;
		}
		posX = 0.0f;
		posY -= 32.0f;
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
	//DirectX::XMFLOAT2 pos = m_pMapChip->GetPos(0);
	//pos.y -= 32.0f;
	DirectX::XMFLOAT2 pos(0.0f, DX_System::GetWindowHeight() - 32.0f * 4.0f);

	return pos;
}