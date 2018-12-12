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

	void Update(const DirectX::XMFLOAT2& playerPos);
	bool Render();
	DirectX::XMFLOAT2 GetInitPlayerPos() const;
	
	bool IsHit(const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& vec, DirectX::XMFLOAT2* pDiff);
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

	void SearchIndex(const DirectX::XMFLOAT2& pos, int* xIndex, int* yIndex);
	void Scroll(const DirectX::XMFLOAT2& playerPos);
};
#endif // !__STAGE_H_
