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

bool Stage::IsHit(const DirectX::XMFLOAT2& pos, DirectX::XMFLOAT2* pDiff)
{
	bool isHit = false;

	tagBlock* pBlock = nullptr;
	DX_Instance2DObject* pInstance2DObject = m_pMapChip->GetInstance2DObject();

	const float halfSizeX = RENDER_SIZE_X * 0.5f;
	const float halfSizeY = RENDER_SIZE_Y * 0.5f;

	DirectX::XMFLOAT2 centerPos(pos.x + halfSizeX, pos.y + halfSizeY);
	DirectX::XMFLOAT2 centerPos2(0.0f, 0.0f);
	DirectX::XMFLOAT2 vec(0.0f, 0.0f);
	float lenght = 0.0f;
	unsigned int index = 0;
	for (int y = 0; y < m_chipNumY; ++y)
	{
		for (int x = 0; x < m_chipNumX; ++x)
		{
			pBlock = &m_pMpaChips[y][x];

			if (pBlock->type == CHIP_TYPE::SKY) {
				continue;
			}

			index = y * m_chipNumX + x;
			centerPos2 = pInstance2DObject->GetPos(index);
			centerPos2.x += halfSizeX;
			centerPos2.y += halfSizeY;

			vec.x = centerPos2.x - centerPos.x;
			vec.y = centerPos2.y - centerPos.y;
			
			lenght = sqrtf(vec.x * vec.x + vec.y * vec.y);
			if (lenght <= CHIP_SIZE_XY) {

				// xまたはyどちらがめり込んでるか判断する必要がある。
				pDiff->x = vec.x;
				pDiff->y = 32.0f - vec.y;
				if (fabsf(pDiff->y) < FLT_EPSILON) {
					pDiff->y = 0.0f;
				}
				if (fabsf(pDiff->x) < FLT_EPSILON) {
					pDiff->x = 0.0f;
				}
 				// ジャンプして天井にあたったときのことも考慮しなければならない
				// 差分が0近い(FLT_EPSILON)なら0にする
				isHit = true;
				break;
			}
		}
		if (isHit) {
			break;
		}
	}

	return isHit;
}
