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

	// ��������͑S��ʕ��쐬����(TODO:�̂��ɃX�N���[���ǂݍ��݂ɑΉ�������)
	int m_chipNumX;
	int m_chipNumY;
	int** m_mapChips;
};
#endif // !__STAGE_H_
