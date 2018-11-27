#ifndef __MAP_CHIP_H_
#define __MAP_CHIP_H_

class MapChip
{
public:
	MapChip();
	~MapChip();

	bool Initialize(const char* pFilepath, const unsigned int instanceNum);

	bool Render();

	void SetPos(const unsigned index, const float y, const float x);
	DirectX::XMFLOAT2 GetPos(const unsigned index)const;
	void Disable(const unsigned index);
	void Enable(const unsigned index);

private:
	DX_Instance2DObject* m_pObjectList;
	DX::tagInstance2D*	m_pInstance2DList;
	unsigned int m_instanceNum;
};
#endif // !__MAP_CHIP_H_
