#ifndef __DX_BOX_H_
#define __DX_BOX_H_

class DX_Box
{
public:
	DX_Box();
	~DX_Box();

	void SetPos(const DirectX::XMFLOAT3 pos);
	void SetScale(const DirectX::XMFLOAT3 scale);
	void SetAngle(const DirectX::XMFLOAT3 angle);
private:
	ID3D11Buffer*	m_pVertexBuffer;
	ID3D11Buffer*	m_pIndexBuffer;

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_angle;
	DirectX::XMFLOAT4X4 m_worldMat;

	bool m_bChanged;

	void Update();
	void Render();
};
#endif // !__DX_BOX_H_
