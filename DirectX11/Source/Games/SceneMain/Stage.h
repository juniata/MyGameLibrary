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
private:
	MapChip* m_pMapChip;
	MapChip* m_pSkyMapChip;

	static const int CHIP_X_NUM = 120;
	static const int CHIP_Y_NUM = 23;
	int m_mapChips[CHIP_Y_NUM][CHIP_X_NUM];
};
#endif // !__STAGE_H_
