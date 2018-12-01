#ifndef __STAGE_H_
#define __STAGE_H_

class MapChip;

class Stage
{
public:
	enum class CHIP_TYPE : int {
		SKY,
		BLOCK,
	};
	Stage();
	~Stage();
	bool Initialize();

	bool Render();
	DirectX::XMFLOAT2 GetInitPlayerPos() const;
	
	bool IsHit(const DirectX::XMFLOAT2& pos, DirectX::XMFLOAT2* pDiff);
private:
	MapChip* m_pMapChip;

	struct tagBlock {
		//DX::tagInstance2D* pInstance2D;
		CHIP_TYPE type;
	};

	tagBlock** m_pMpaChips;
	// いったんは全画面分作成する(TODO:のちにスクロール読み込みに対応させる)
	int m_chipNumX;
	int m_chipNumY;
};
#endif // !__STAGE_H_
