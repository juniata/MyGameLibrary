#pragma once


class Object3D
{
public:
	Object3D();
	~Object3D();
	
	bool Initialize(const char* pFilepath);
	void Update();
	bool Render();
	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetAngle();
	void SetPos(const DirectX::XMFLOAT3& pos);
	void SetAngle(const DirectX::XMFLOAT3& angle);
private:
	FBXLoader*		m_pFbx;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_wolrdMatrixBuffer;
	DirectX::XMFLOAT4X4  m_worldMatrix;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_angle;

	bool m_bChanged;
};
