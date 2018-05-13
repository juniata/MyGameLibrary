#ifndef __DX_PARTICLE_H_
#define __DX_PARTICLE_H_

struct tagParticleInfo{
	DirectX::XMFLOAT3 pos;	//	���W
	DirectX::XMFLOAT4 color;	//	�F
	float   size;	//	�傫��
};
//****************************************************************************************************
//
//	Particle
//
//****************************************************************************************************
class Particle
{
private:
	ID3D11Buffer*	m_pVeretxBuffer;
	ID3D11ShaderResourceView*	m_pShaderResourceView;

	DirectX::XMFLOAT3 m_pos;		//	�p�[�e�B�N�����W
	DirectX::XMFLOAT4 m_color;	//	���_�F
	float	m_size;		//	�p�[�e�B�N���T�C�Y

	Particle();
public:
};


//****************************************************************************************************
//
//	ParticleSystem
//
//****************************************************************************************************
class ParticleSystem
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
