#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  パーティクル情報
//
//-----------------------------------------------------------------------------------------
DX_Particle::DX_Particle()
{
}


//-----------------------------------------------------------------------------------------
//
//  全パーティクルを初期化
//
//-----------------------------------------------------------------------------------------
void DX_ParticleSystem::Initialize()
{

}

//-----------------------------------------------------------------------------------------
//
//  全パーティクルを描画
//
//-----------------------------------------------------------------------------------------
void DX_ParticleSystem::Render()
{
}

//-----------------------------------------------------------------------------------------
//
//  全パーティクルを更新
//
//-----------------------------------------------------------------------------------------
void DX_ParticleSystem::ParticleUpdate()
{
	//	頂点シェーダーはただ出力

	//	CPUで設定した座標をSRVに書き出す
}

//-----------------------------------------------------------------------------------------
//
//  全パーティクルを描画
//
//-----------------------------------------------------------------------------------------
void DX_ParticleSystem::ParticleRender()
{
	//	ParticleUpdateで更新した座標を頂点シェーダーが受け取り、描画
}