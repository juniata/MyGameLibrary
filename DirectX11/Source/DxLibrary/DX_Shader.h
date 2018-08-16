#ifndef __DX_SHADER_H_
#define __DX_SHADER_H_

//	各シェーダーのエントリーポイント
#define VS_ENTRY_POINT "VS_Main"
#define HS_ENTRY_POINT "HS_Main"
#define DS_ENTRY_POINT "DS_Main"
#define GS_ENTRY_POINT "GS_Main"
#define PS_ENTRY_POINT "PS_Main"
#define CS_ENTRY_POINT "CS_Main"

//	各シェーダーのバージョン
#define VS_VERSION "vs_5_0"
#define HS_VERSION "hs_5_0"
#define DS_VERSION "ds_5_0"
#define GS_VERSION "gs_5_0"
#define PS_VERSION "ps_5_0"
#define CS_VERSION "cs_5_0"

//****************************************************************************************************
//
//	Shader
//
//****************************************************************************************************
class DX_Shader
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を初期化
	//
	//------------------------------------------------------------------------------
	DX_Shader();

	//------------------------------------------------------------------------------
	//
	//  @brief		実体があれば解放
	//
	//------------------------------------------------------------------------------
	virtual ~DX_Shader();

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーを作成する
	//	@param[in]	pDevice		DirectX11のデバイス
	//	@param[in]	pFilepath	シェーダーファイルのパス
	//
	//------------------------------------------------------------------------------
	virtual void CreateShader(ID3D11Device* pDevice, const char* pFilepath) = 0;

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの使用を開始
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//	@param[in]	classInstanceCount	クラスインスタンスの数
	//
	//------------------------------------------------------------------------------
	virtual void Begin(ID3D11DeviceContext*	pDeviceContext, const unsigned int classInstanceCount = 0) = 0;

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの利用を終える
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	virtual void End(ID3D11DeviceContext*	pDeviceContext) = 0;

	//------------------------------------------------------------------------------
	//
	//  @brief		コンパイルした時のバイトコードを取得
	//	@return		コンパイルした時のバイトコード
	//
	//------------------------------------------------------------------------------
	ID3DBlob* GetByteCord();

protected:
	ID3D11ClassLinkage*		m_pClassLinkage;
	ID3D11ClassInstance*	m_pClassInstance;
	ID3D11InputLayout*		m_pInputLayout;
	ID3DBlob*				m_pBytecord;

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーファイルをコンパイルする
	//	@param[in]	pFilepath		シェーダーファイルのパス
	//	@param[in]	pEntryPoint		シェーダーのエントリーポイント
	//	@param[in]	pShaderVersion	シェーダーのバージョン
	//
	//------------------------------------------------------------------------------
	void CompileFromFile(const char* pFilepath, const char* pEntryPoint, const char* pShaderVersion);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーオブジェクトを作成する
	//	@param[in]	pDevice	DirectX11のデバイス
	//
	//------------------------------------------------------------------------------
	virtual void CreateShaderObject(ID3D11Device* pDevice) = 0;

	//------------------------------------------------------------------------------
	//
	//  @brief		動的シェーダーを有効にするリンクを作成
	//	@param[in]	pDevice	DirectX11のデバイス
	//
	//------------------------------------------------------------------------------
	void CreateClassLinkage(ID3D11Device* pDevice);
};

//	各シェーダーヘッダーをインクルード
#include	"DX_VertexShader.h"
#include	"DX_GeometryShader.h"
#include	"DX_PixelShader.h"
#include	"DX_ComputeShader.h"
#endif // !__DX_SHADER_H_
