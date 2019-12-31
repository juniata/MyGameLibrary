#ifndef __DX_LIBRARY_H_
#define __DX_LIBRARY_H_

/*
	DirectXを用いたゲームエンジンです。
	Windwos10に対応。
	DreictX11で作成中。

	使いやすさ重視で設計します。

*/

#define DIRECTINPUT_VERSION  0x0800

#include	<Windows.h>
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<d3dcompiler.h>
#include	<XAudio2.h>
#include	<XInput.h>
#include	<dinput.h>
#include	<wrl/client.h>

#include	"DX_Singleton.h"
#include	"DX_Define.h"
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
#include	"DX_TextureManager.h"
#include	"DX_GameObject.h"
#include	"DX_2DObject.h"
#include	"DX_Button.h"
#include	"DX_Instance2DObject.h"
#include	"DX_MaptipObject.h"
#include	"DX_Mesh.h"
#include	"DX_SkinMesh.h"
#include	"DX_InstanceMesh.h"
#include	"DX_InstanceSkinMesh.h"
#include	"DX_Box.h"
#include	"DX_Lighting.h"
#include	"DX_Audio.h"
#include	"DX_Input.h"
#include	"DX_XInput.h"
#include	"DX_Particle.h"
#include	"DX_Font.h"

#endif // !__DX_LIBRARY_H_
