#ifndef __DX_PIXEL_SHADER_H_
#define __DX_PIXEL_SHADER_H_

class DX_PixelShader : public DX_Shader
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を初期化
	//
	//------------------------------------------------------------------------------
	DX_PixelShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		実体があれば解放
	//
	//------------------------------------------------------------------------------
	~DX_PixelShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーを作成する
	//	@param[in]	pDevice		DirectX11のデバイス
	//	@param[in]	pFilepath	シェーダーファイルのパス
	//
	//------------------------------------------------------------------------------
	virtual void CreateShader(ID3D11Device* pDevice, const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの使用を開始
	//	@param[in]	classInstanceCount	クラスインスタンスの数
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	void Begin(ID3D11DeviceContext*	pDeviceContext, const unsigned int classInstanceCount = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの利用を終える
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	void End(ID3D11DeviceContext*	pDeviceContext);

private:
	ID3D11PixelShader*	m_pPixelShader;

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーオブジェクトを作成する
	//	@param[in]	pDevice	DirectX11のデバイス
	//
	//------------------------------------------------------------------------------
	void CreateShaderObject(ID3D11Device* pDevice);
};
#endif // !__DX_PIXEL_SHADER_H_
