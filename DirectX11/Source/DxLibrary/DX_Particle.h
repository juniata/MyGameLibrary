#ifndef __DX_PARTICLE_H_
#define __DX_PARTICLE_H_

struct tagParticleInfo{
	DirectX::XMFLOAT3 pos;	//	座標
	DirectX::XMFLOAT4 color;	//	色
	float   size;	//	大きさ
};
//****************************************************************************************************
//
//	Particle
//
//****************************************************************************************************
class DX_Particle
{
private:
	ID3D11Buffer*	m_pVeretxBuffer;
	ID3D11ShaderResourceView*	m_pShaderResourceView;

	DirectX::XMFLOAT3 m_pos;		//	パーティクル座標
	DirectX::XMFLOAT4 m_color;	//	頂点色
	float	m_size;		//	パーティクルサイズ

	DX_Particle();
public:
};


//****************************************************************************************************
//
//	ParticleSystem
//
//****************************************************************************************************
class DX_ParticleSystem
{
private:


public:
	static void Initialize();
	static void Render();

private:
	static void ParticleUpdate();
	static void ParticleRender();



};
#endif // !__DX_PARTICLE_H_
