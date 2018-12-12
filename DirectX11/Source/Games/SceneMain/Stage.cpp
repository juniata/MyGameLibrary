#include	"DxLibrary/DX_Library.h"
#include	"Games/MapChip/MapChip.h"
#include	"Stage.h"
#include	<time.h>
#include	"Games/GameDefine.h"
#include	<float.h>

Stage::Stage() : 
	m_pMpaChips(nullptr),
	m_pMapChip(nullptr),
	m_chipNumX(0),
	m_chipNumY(0)
{
}

Stage::~Stage()
{
	DELETE_OBJ(m_pMapChip);
	for (int y = 0; y < m_chipNumY; ++y)
	{
		DELETE_OBJ_ARRAY(m_pMpaChips[y]);
	}
	DELETE_OBJ_ARRAY(m_pMpaChips);
}
bool Stage::Initialize()
{
	unsigned int windowWidth = DX_System::GetWindowWidth();
	unsigned int windowHeight = DX_System::GetWindowHeight();

	// 描画サイズ 32 * 32
	// * num は何画面分作成するか
	int num = 2;
	m_chipNumX = CAST_I(windowWidth / CAST_UI(RENDER_SIZE_X)) * num;
	m_chipNumY = CAST_I(windowHeight / CAST_UI(RENDER_SIZE_Y)) * num;

	m_pMpaChips = new tagBlock*[m_chipNumY];
	for (int y = 0; y < m_chipNumY; ++y)
	{
		m_pMpaChips[y] = new tagBlock[m_chipNumX];
	}
	// いったんは全画面分作成する
	// 一画面に描画するブロック数を設定する。(画面サイズの変更に対応する)
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			m_pMpaChips[y][x].type = CHIP_TYPE::BLOCK;
		}
	}
	for (int y = 3; y < m_chipNumY; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			m_pMpaChips[y][x].type = CHIP_TYPE::SKY;
		}
	};

	srand(CAST_UI(time(NULL)));
	for (int i = 0; i < 20; ++i) {
		m_pMpaChips[rand() % (m_chipNumY - 5)][rand() % (m_chipNumX - 1) + 1].type = CHIP_TYPE::BLOCK;
	}

	m_pMapChip = new MapChip();
	if (false == m_pMapChip->Initialize("SceneMain\\mapchip.png", m_chipNumY * m_chipNumX)) {
		return false;
	}

	
	DX_Instance2DObject* pInstance2DObject = m_pMapChip->GetInstance2DObject();

	float posX = 0;
	float posY = windowHeight - RENDER_SIZE_Y;	// 描画サイズ分ひかないと画面下から描画してしまう
	unsigned int index = 0;
	for (int y = 0; y < m_chipNumY; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			index = CAST_UI(y * m_chipNumX + x);
			pInstance2DObject->SetPos(index, DirectX::XMFLOAT2(posX, posY));
			switch (m_pMpaChips[y][x].type) {
			case CHIP_TYPE::SKY:
				pInstance2DObject->SetUV(index, 1, 0);
				break;
			case CHIP_TYPE::BLOCK:
				pInstance2DObject->SetUV(index, 0, 0);
				break;
			}
			posX += RENDER_SIZE_X;
		}
		posX = 0.0f;
		posY -= RENDER_SIZE_Y;
	}

	return true;
}

void Stage::Update(const DirectX::XMFLOAT2& playerPos)
{
	Scroll(playerPos);
}

void Stage::SearchIndex(const DirectX::XMFLOAT2& pos, int* xIndex, int* yIndex)
{
	unsigned int windowWidth = DX_System::GetWindowWidth();
	unsigned int windowHeight = DX_System::GetWindowHeight();

	float posX = 0;
	float posY = windowHeight - RENDER_SIZE_Y;	// 描画サイズ分ひかないと画面下から描画してしまう
	unsigned int index = 0;
	for (int y = 0; y < m_chipNumY; ++y)
	{
		if (posY <= pos.y && pos.y <= (posY + RENDER_SIZE_Y)) {
			*yIndex = y;
			break;
		}
		posY -= RENDER_SIZE_Y;
	}
	for (int x = 0; x < m_chipNumX; ++x)
	{
		if (posX <= pos.x && pos.x <= (posX + RENDER_SIZE_X)) {
			*xIndex = x;
			break;
		}
		posX += RENDER_SIZE_Y;
	}
}
void Stage::Scroll(const DirectX::XMFLOAT2& playerPos)
{
	const unsigned int widonwHeight = DX_System::GetWindowHeight();
	const unsigned int widonwWidth	= DX_System::GetWindowWidth();
	const float halfWidonwHeight	= CAST_F(widonwHeight) * 0.5f;
	const float halfWindowWidth		= CAST_F(widonwWidth) * 0.5f;
	
	unsigned int index = 0;
	for (int y = 0; y < m_chipNumY; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			index = CAST_UI(y * m_chipNumX + x);
		}
	}
}
bool Stage::Render()
{
	bool result = true;

	result = m_pMapChip->Render();
	
	return result;
}

DirectX::XMFLOAT2 Stage::GetInitPlayerPos() const
{
	// TODO:今はちょせつ調整してプレイヤーの初期座標を設定している
	DirectX::XMFLOAT2 pos(0.0f, DX_System::GetWindowHeight() - 32.0f * 4.0f);

	return pos;
}

bool Stage::IsHit(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& vec, DirectX::XMFLOAT2* pDiff)
{
	bool isHit = false;

	int indexX = 0;
	int indexY = 0;
	tagBlock* pBlock = nullptr;
	DX_Instance2DObject* pInstance2DObject = m_pMapChip->GetInstance2DObject();

	DirectX::XMFLOAT2 playerCenterPos(pos.x + CHIP_HALF_SIZE_XY, pos.y + CHIP_HALF_SIZE_XY);
	DirectX::XMFLOAT2 chipPos(0.0f, 0.0f);
	DirectX::XMFLOAT2 chipCenter(0.0f, 0.0f);
	unsigned int index = 0;
	
	DirectX::XMFLOAT2 chipPosies[4];
	
	for (int y = 0; y < m_chipNumY; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			pBlock = &m_pMpaChips[y][x];

			if (pBlock->type == CHIP_TYPE::SKY) {
				continue;
			}

			index = y * m_chipNumX + x;

			chipPos = pInstance2DObject->GetPos(index);

			chipCenter.x = chipPos.x + CHIP_HALF_SIZE_XY;
			chipCenter.y = chipPos.y + CHIP_HALF_SIZE_XY;
			
			// 左上の頂点
			chipPosies[0] = chipPos;
		
			// 右上の頂点
			chipPosies[1] = chipPos;
			chipPosies[1].x += CHIP_SIZE_XY;

			// 左下の頂点
			chipPosies[2] = chipPos;
			chipPosies[2].y += CHIP_SIZE_XY;

			// 右下の頂点
			chipPosies[3].x = chipPosies[1].x;
			chipPosies[3].y = chipPosies[2].y;

			for (int i = 0; i < 4; ++i)
			{
				if (pos.x <= chipPosies[i].x && chipPosies[i].x <= (pos.x + CHIP_SIZE_XY) &&
					pos.y <= chipPosies[i].y && chipPosies[i].y <= (pos.y + CHIP_SIZE_XY)) {
			
					///pDiff->x = (CHIP_SIZE_XY - fabsf(chipCenter.x - playerCenterPos.x)) * vec.x;
					pDiff->y = (CHIP_SIZE_XY - fabsf(chipCenter.y - playerCenterPos.y)) * vec.y;
					isHit = true;
					break;
				}
			}
			if (isHit) {
				break;
			}
		}
		if (isHit) {
			break;
		}
	}

	return isHit;
}
