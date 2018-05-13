#ifndef __DX_LIGHTING_H_
#define __DX_LIGHTING_H_

//	頂点シェーダーで行うライティング情報
struct tagVertexLighting{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT3 viewPos;
	DirectX::XMFLOAT3 lightPos;
	DirectX::XMFLOAT3 lightDir;
	DirectX::XMFLOAT3	padding;	//	16byte alignment
};

//	ピクセルシェーダーで行うlighting情報
struct tagPixelLighting{
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 viewPos;
	DirectX::XMFLOAT3 lightDir;
	DirectX::XMFLOAT2 padding;	//	16byte alignment
};
//****************************************************************************************************
//
//	Lighting
//
//****************************************************************************************************
class Lighting
{
private:
	static DirectX::XMFLOAT4 m_ambient;	//	拡散光
	static DirectX::XMFLOAT4 m_diffuse;	//	環境光
	static DirectX::XMFLOAT4 m_specular;	//	反射光
	static DirectX::XMFLOAT3 m_viewPos;	//	視点
	static DirectX::XMFLOAT3 m_lightPos;	//	ライトの座標
	static DirectX::XMFLOAT3 m_lightDir;	//	ライトの向き

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	Lighting() = delete;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	~Lighting(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を初期化
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		環境光を設定
	//	@param[in]	ambient	環境光
	//	@note		rgbの順番 0~1の値
	//
	//------------------------------------------------------------------------------
	static void SetAmbient(const DirectX::XMFLOAT3& ambient);

	//------------------------------------------------------------------------------
	//
	//  @brief		拡散光を設定
	//	@param[in]	diffuse	拡散光
	//	@note		rgbの順番 0~1の値
	//
	//------------------------------------------------------------------------------
	static void SetDiffuse(const DirectX::XMFLOAT3& diffuse);

	//------------------------------------------------------------------------------
	//
	//  @brief		反射光を設定
	//	@param[in]	specular	反射光
	//	@note		rgbの順番 0~1の値
	//
	//------------------------------------------------------------------------------
	static void SetSpecular(const DirectX::XMFLOAT3& specular);

	//------------------------------------------------------------------------------
	//
	//  @brief		ライトの座標を設定
	//	@param[in]	pos	ライト座標
	//
	//------------------------------------------------------------------------------
	static void SetLightPos(const DirectX::XMFLOAT3& pos);

	//------------------------------------------------------------------------------
	//
	//  @brief		光の向きを設定
	//	@param[in]	dir	光の向き
	//	@note		正規化してから入れてください
	//
	//------------------------------------------------------------------------------
	static void SetLightDir(const DirectX::XMFLOAT3& dir);

	//------------------------------------------------------------------------------
	//
	//  @brief		視点を設定
	//	@param[in]	pos	視点座標
	//
	//------------------------------------------------------------------------------
	static void SetViewPos(const DirectX::XMFLOAT3& pos);

	//------------------------------------------------------------------------------
	//
	//  @brief		頂点シェーダーで行うライト情報を設定
	//
	//------------------------------------------------------------------------------
	static void SetLightVertexShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		ピクセルシェーダーで行うライト情報を設定
	//
	//------------------------------------------------------------------------------
	static void SetLightPixelShader();
};
#endif // !__DX_LIGHTING_H_
