#ifndef __DX_VIEW_H_
#define __DX_VIEW_H_

//	デフォルト用
#ifndef PI
	#define PI CAST_F(3.14159265358979323846f) 
#endif

#define VIEW_DEFAULT_ASPECT  CAST_F(PI * 0.25f)
#define VIEW_DEFAULT_ZNEAR  CAST_F(1.0f)
#define VIEW_DEFAULT_ZFAR	CAST_F(1000.0f)

//****************************************************************************************************
//
//	View
//
//****************************************************************************************************
class DX_View
{
protected:
	static D3D11_VIEWPORT m_viewPort;
	static DirectX::XMFLOAT4X4 m_matView;
	static DirectX::XMFLOAT4X4 m_matProj;
	static DirectX::XMFLOAT4X4 m_matViewProj;
	
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_target;
	DirectX::XMFLOAT3 m_upDirection;

	struct tagViewInfo{
		float fovY;
		float znear;
		float zfar;
		float aspect;
	}m_viewInfo;

	bool m_bChanged;

	//	視錐台の面
	DirectX::XMFLOAT4 m_plane[6];

	//	視錐台の面を何フレームおきに作成するかを設定
	int m_updateFrameNum;

public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を初期化
	//
	//------------------------------------------------------------------------------
	DX_View();

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	virtual ~DX_View(){};
	
	//------------------------------------------------------------------------------
	//
	//  @brief		行列を計算し、RSにViewportを設定する
	//
	//------------------------------------------------------------------------------
	void Active();

	//------------------------------------------------------------------------------
	//
	//  @brief		RenderTargetViewとDepthStencileViewをクリアする
	//	@param[in]	bZClear			trueなら深度をクリアする(白色でクリア)
	//	@param[in]	bStencilClear	trueならステンシルをクリアする(0でクリア)
	//
	//------------------------------------------------------------------------------
	static void Clear(ID3D11DeviceContext* pContext, const bool bZClear = true, const bool bStencilClear	= true, ID3D11RenderTargetView* pRtv = nullptr, ID3D11DepthStencilView* pDsv = nullptr);

	//------------------------------------------------------------------------------
	//
	//  @brief		ViewPortを設定する
	//
	//------------------------------------------------------------------------------
	void SetViewPort();

	//------------------------------------------------------------------------------
	//
	//  @brief		プロジェクションをを設定する
	//	@param[in]	fovY	画角
	//	@param[in]	znear	試錐台の手前
	//	@param[in]	zfar	試錐台の奥
	//
	//------------------------------------------------------------------------------
	void SetProjection(
		const float fovY,
		const float znear,
		const float zfar
		);


	//------------------------------------------------------------------------------
	//
	//  @brief		viewに関する行列を送る
	//
	//------------------------------------------------------------------------------
	static void SetMatrixForTheView();

	static D3D11_VIEWPORT* GetViewPort() { return &m_viewPort; }

	//------------------------------------------------------------------------------
	//
	//  @brief		座標を設定
	//	@param[in]	pos	座標
	//
	//------------------------------------------------------------------------------
	void SetPos(const DirectX::XMFLOAT3& pos);

	//------------------------------------------------------------------------------
	//
	//  @brief		ターゲットを設定
	//	@param[in]	target	ターゲット
	//
	//------------------------------------------------------------------------------
	void SetTarget(const DirectX::XMFLOAT3& target);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		座標を取得
	//	@return		m_pos	座標
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetPos()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		ターゲットを取得
	//	@return		m_target	ターゲット
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetTarget()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		視錐台の6つの面を作成
	//	@param[in]	updateFrameNum	指定したフレームおきに、視錐台の6つの面を算出する
	//
	//------------------------------------------------------------------------------
	void CreateFrustum(
		const int updateFrameNum = 0	
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		点が視錐台の中にいるかをチェック
	//	@param[in]	pos	点座標
	//	@return		true:入っている	false:入っていない
	//
	//------------------------------------------------------------------------------
	bool IsCheckPointInFrustum(
		const DirectX::XMFLOAT3& pos
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		ボックスが視錐台の中にいるかをチェック
	//	@param[in]	center	ボックスの中心座標
	//	@param[in]	radius	ボックスの半径
	//	@return		true:入っている	false:入っていない
	//
	//------------------------------------------------------------------------------
	bool IsCheckCubeInFrustum(
		const DirectX::XMFLOAT3&	center,
		const float		radius	
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		球体が視錐台の中にいるかをチェック
	//	@param[in]	center	球体の中心座標
	//	@param[in]	radius	球体の半径
	//	@return		true:入っている	false:入っていない
	//
	//------------------------------------------------------------------------------
	bool IsCheckSphereInFrustum(
		const DirectX::XMFLOAT3&	center,
		const float		radius	
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		矢印キーでカメラを移動
	//	@param[in]	moveSpeed	カメラの移動速度
	//
	//------------------------------------------------------------------------------
	void	FreeCamera(const float moveSpeed = 1.0f);
};
#endif // !__DX_VIEW_H_
