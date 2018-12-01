#ifndef __MAP_CHIP_H_
#define __MAP_CHIP_H_

class MapChip
{
public:
	MapChip();
	~MapChip();

	bool Initialize(const char* pFilepath, const unsigned int instanceNum);

	bool Render();

	DX_Instance2DObject* GetInstance2DObject();
private:
	DX_Instance2DObject* m_pObjectList;
	unsigned int m_instanceNum;
};
#endif // !__MAP_CHIP_H_
