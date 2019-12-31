#ifndef __DX_DEFINE_H_
#define __DX_DEFINE_H_


//	�}�N��
#define DELETE_OBJ(x)			if(x){	delete	x;		x = nullptr;	}	
#define DELETE_OBJ_ARRAY(x)		if(x){  delete[] x;		x = nullptr;	}	
#define SAFE_RELEASE(x)			if(x){  x->Release();	x = nullptr;	}

typedef bool dx_bool;
typedef void dx_void;

// 1byte
typedef unsigned char dx_uchar8;
typedef char dx_char8;

// 2byte
typedef unsigned short dx_uint16;
typedef short dx_int16;

// 4byte
typedef unsigned long dx_uint32;
typedef long dx_int32;

// 8byte
typedef unsigned long int dx_uint64;
typedef long int dx_int64;

// 4byte
typedef float dx_float;

// 8byte
typedef double dx_double;


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

	namespace CAST {
		template<class Type> unsigned int UI(Type t)
		{
			return static_cast<unsigned int>(t);
		}

		template<class Type>  int I(Type t)
		{
			return static_cast<int>(t);
		}

		template<class Type>  char C(Type t)
		{
			return static_cast<char>(t);
		}

		template<class Type>  float F(Type t)
		{
			return static_cast<float>(t);
		}

		template<class Type>  double D(Type t)
		{
			return static_cast<double>(t);
		}
		template <class Type> long L(Type t)
		{
			return static_cast<long>(t);
		}
	}
	
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
	//	�e�N�X�`�����
	//--------------------------------------------------
	struct tagTextureInfo : public D3D11_TEXTURE2D_DESC {
		char filepath[_MAX_PATH];
		ID3D11ShaderResourceView* pSrv;
	};

	//--------------------------------------------------
	//	2D�p���_���
	//--------------------------------------------------
	struct tagVertex2D {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
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

	//--------------------------------------------------
	//	�ʏ��
	//--------------------------------------------------
	struct tagFace {
		tagVertex			vertex[3];		//	���_���
		DirectX::XMFLOAT3	normal;			//	�ʖ@��
		DWORD				materialNum;	//	�}�e���A���ԍ�
		WORD				index[3];		//	�C���f�b�N�X�ԍ�
	};


};

#endif // !__DX_DEFINE_H_
