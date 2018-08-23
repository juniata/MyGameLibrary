#ifndef __DX_LIBRARY_H_
#define __DX_LIBRARY_H_

/*
	windows10によるdirectxゲーム開発
	今のところwindwos7は対応させません。
	windows10でdirectx11を開発した後、directx12を導入するかもしれません。
*/

#include	<Windows.h>
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<d3dcompiler.h>
//#include	<d3d9.h>
#include	<XAudio2.h>
#include	<XInput.h>


//	マクロ
#define DELETE_OBJ(x)			if(x){	delete	x;		x = nullptr;	}	
#define DELETE_OBJ_ARRAY(x)		if(x){  delete[] x;		x = nullptr;	}	
#define SAFE_RELEASE(x)			if(x){  x->Release();	x = nullptr;	}

//	キャストマクロ
#define CAST_C(x) static_cast<char>(x)
#define CAST_I(x) static_cast<int>(x)
#define CAST_F(x) static_cast<float>(x)
#define CAST_D(x) static_cast<double>(x)
#define CAST_L(x) static_cast<long>(x)

typedef ID3D11ShaderResourceView	ID3D11SRV;
typedef ID3D11RenderTargetView		ID3D11RTV;
typedef ID3D11DepthStencilView		ID3D11DSV;
typedef ID3D11UnorderedAccessView	ID3D11UAV;

//--------------------------------------------------
//	4頂点
//--------------------------------------------------
struct tagRect{
	float x, y, w, h;
	tagRect(const float x, const float y, const float w, const float h) :
		x(x), y(y), w(w), h(h)
	{}
};

//--------------------------------------------------
//	Mesh用頂点情報
//--------------------------------------------------
struct tagVertex{
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

#include	"DxLibrary\DX_Scene.h"
#include	"DxLibrary\DX_SceneManager.h"
#include	"DxLibrary\DX_System.h"
#include	"DxLibrary\DX_FrameWork.h"
#include	"DxLibrary\DX_Debug.h"
#include	"DxLibrary\DX_Graphics.h"
#include	"DxLibrary\DX_RenderState.h"
#include	"DxLibrary\DX_Shader.h"
#include	"DxLibrary\DX_ShaderManager.h"
#include	"DxLibrary\DX_ResourceManager.h"
#include	"DxLibrary\DX_View.h"
#include	"DxLibrary\DX_Buffer.h"
#include	"DxLibrary\DX_Resource.h"
#include	"DxLibrary\DX_Texture.h"
#include	"DxLibrary\DX_GameObject.h"
#include	"DxLibrary\DX_2DObject.h"
#include	"DxLibrary\DX_Instance2DObject.h"
#include	"DxLibrary\DX_Mesh.h"
#include	"DxLibrary\DX_SkinMesh.h"
#include	"DxLibrary\DX_InstanceMesh.h"
#include	"DxLibrary\DX_InstanceSkinMesh.h"
#include	"DxLibrary\DX_Box.h"
#include	"DxLibrary\DX_Lighting.h"
#include	"DxLibrary\DX_Audio.h"
#include	"DxLibrary\DX_Input.h"
#include	"DxLibrary\DX_XInput.h"
#include	"DxLibrary\DX_Particle.h"
#include	"DxLibrary\DX_Font.h"



#include	"../user_helper_class/AutoProfile.h"
#endif // !__DX_LIBRARY_H_
