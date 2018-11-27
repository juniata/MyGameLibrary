#ifndef __DX_DEFINE_H_
#define __DX_DEFINE_H_


//	�}�N��
#define DELETE_OBJ(x)			if(x){	delete	x;		x = nullptr;	}	
#define DELETE_OBJ_ARRAY(x)		if(x){  delete[] x;		x = nullptr;	}	
#define SAFE_RELEASE(x)			if(x){  x->Release();	x = nullptr;	}

//	�L���X�g�}�N��
#define CAST_C(x) static_cast<char>(x)
#define CAST_I(x) static_cast<int>(x)
#define CAST_UI(x) static_cast<unsigned int>(x)
#define CAST_F(x) static_cast<float>(x)
#define CAST_D(x) static_cast<double>(x)
#define CAST_L(x) static_cast<long>(x)

typedef ID3D11DeviceContext			ID3D11DC;
typedef ID3D11ShaderResourceView	ID3D11SRV;
typedef ID3D11RenderTargetView		ID3D11RTV;
typedef ID3D11DepthStencilView		ID3D11DSV;
typedef ID3D11UnorderedAccessView	ID3D11UAV;
typedef ID3D11InputLayout			ID3D11IL;
typedef ID3D11VertexShader			ID3D11VS;
typedef ID3D11HullShader			ID3D11HS;
typedef ID3D11DomainShader			ID3D11DS;
typedef ID3D11GeometryShader		ID3D11GS;
typedef ID3D11ComputeShader			ID3D11CS;
typedef ID3D11PixelShader			ID3D11PS;

namespace DX {
	struct tagRect {
		union { float x, left;	};
		union { float y, top;	};
		union { float w, right;	};
		union {	float h, bottom;};

		//------------------------------------------------------------------------------
		//
		//  @brief		�ϐ���������
		//
		//------------------------------------------------------------------------------
		tagRect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}

		//------------------------------------------------------------------------------
		//
		//  @brief		�ϐ��������ŏ�����
		//	@param[in]	x	�����W
		//	@param[in]	y	����W
		//	@param[in]	w	�E���W
		//	@param[in]	h	�����W
		//
		//------------------------------------------------------------------------------
		tagRect(const float x, const float y, const float w, const float h) : x(x), y(y), w(w), h(h) {}
	};
	//--------------------------------------------------
	//	2D�p���_���
	//--------------------------------------------------
	struct tagVertex2D {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//--------------------------------------------------
	//	�C���X�^���X2D���
	//--------------------------------------------------
	struct tagInstance2D : public DirectX::XMFLOAT3 {

		//------------------------------------------------------------------------------
		//
		//  @brief		�L��������(�`�悳���悤�ɂȂ�)
		//
		//------------------------------------------------------------------------------
		inline void Enable() { z = 0.0f; };

		//------------------------------------------------------------------------------
		//
		//  @brief		����������(�`�悳��Ȃ��悤�ɂȂ�)
		//
		//------------------------------------------------------------------------------
		inline void Disable() { z = 1.1f; };

		//------------------------------------------------------------------------------
		//
		//  @brief		�L�����ǂ���
		//	@return		�L���Ȃ�true
		//
		//------------------------------------------------------------------------------
		inline bool IsEnable()const { return z <= 1.0f; };

		//------------------------------------------------------------------------------
		//
		//  @brief		�L�����ǂ���
		//	@return		�����Ȃ�true
		//
		//------------------------------------------------------------------------------
		inline bool IsDisable()const { return z > 1.0f; };
	};

	//--------------------------------------------------
	//	Mesh�p���_���
	//--------------------------------------------------
	struct tagVertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;
	};

	struct tagObjectVertext {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;
	};

};

#endif // !__DX_DEFINE_H_
