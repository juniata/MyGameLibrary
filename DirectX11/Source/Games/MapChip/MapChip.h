#ifndef __MAP_CHIP_H_
#define __MAP_CHIP_H_

class MapChip
{
public:
	MapChip();
	~MapChip();

	bool Initialize(const char* pFilepath, const unsigned int instanceNum);

	bool Render();

	void SetPos(const unsigned index, const unsigned int y, const unsigned int x);
	DirectX::XMFLOAT2 GetPos(const unsigned index)const;
	void Disable(const unsigned index);
	void Enable(const unsigned index);

private:
	DX_Instance2DObject* m_pObjectList;
	DirectX::XMFLOAT3* m_pPosList;
	unsigned int m_instanceNum;
};
#endif // !__MAP_CHIP_H_
