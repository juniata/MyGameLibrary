#ifndef __DX_LIBRARY_H_
#define __DX_LIBRARY_H_


#define WIN32_LEAN_AND_MEAN	
//	DirectX11のマクロ再定義がうざいので消す
//#pragma warning(disable : 4005)
#include	<Windows.h>

// windows10
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<d3dcompiler.h>
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment( lib, "dxgi.lib" )

// windows 7
//#include	<D3DX11.h>
//#include	<D3DX10math.h>
#include	<XAudio2.h>
#include	<XInput.h>
//	型名定義
//typedef D3DXMATRIX		Matrix;
//typedef D3DXQUATERNION	Quaternion;
//typedef D3DXVECTOR4		Vector4;
//typedef D3DXVECTOR3		Vector3;
//typedef D3DXVECTOR2		Vector2;


//	マクロ
#define DELETE_OBJ(x)			if(x){	delete	x;		x = nullptr;	}	
#define DELETE_OBJ_ARRAY(x)		if(x){  delete[] x;		x = nullptr;	}	

//	キャストマクロ
#define CAST_C(x) static_cast<char>(x)
#define CAST_I(x) static_cast<int>(x)
#define CAST_F(x) static_cast<float>(x)
#define CAST_D(x) static_cast<double>(x)
#define CAST_L(x) static_cast<long>(x)

//	STL
#include	<list>
#include	<vector>
#include	<map>


//--------------------------------------------------
//	4頂点
//--------------------------------------------------
struct tagRect{
	float x, y, w, h;
	tagRect(const float x, const float y, const float w, const float h) :
		x(x), y(y), w(w), h(h)
	{}
};

//	Microsoft::WRL::ComPtrを使うため
#include	<wrl.h>
using namespace Microsoft::WRL;


#include	"DX_Scene.h"
#include	"DX_SceneManager.h"
#include	"DX_System.h"
#include	"DX_FrameWork.h"
#include	"DX_Debug.h"
#include	"DX_Graphics.h"
#include	"DX_RenderState.h"
#include	"DX_Shader.h"
#include	"DX_ShaderManager.h"
#include	"DX_ResourceManager.h"
#include	"DX_View.h"
#include	"DX_Buffer.h"
#include	"DX_Resource.h"
#include	"DX_Texture.h"
#include	"DX_GameObject.h"
#include	"DX_Collider.h"
#include	"DX_2DObject.h"
#include	"DX_Mesh.h"
#include	"DX_SkinMesh.h"
#include	"DX_InstanceMesh.h"
#include	"DX_InstanceSkinMesh.h"
#include	"DX_Lighting.h"
#include	"DX_Audio.h"
#include	"DX_Input.h"
#include	"DX_XInput.h"
#include	"DX_Particle.h"
#include	"DX_Font.h"



#include	"../user_helper_class/AutoProfile.h"
#endif // !__DX_LIBRARY_H_
