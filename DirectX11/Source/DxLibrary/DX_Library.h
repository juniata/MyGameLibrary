#ifndef __DX_LIBRARY_H_
#define __DX_LIBRARY_H_

/*
	DirectXを用いたゲームエンジンです。
	Windwos10に対応。
	DreictX11で作成中。
	のちにDirectX12に対応させます。
	DirectX APIをラップして使いやすくしてクラス群があります。

	使いやすさ重視で設計します。


	コーティング規約
	・メンバ変数はm_をつける。
	理由：クラス内で使用する際、メンバ変数に何があるかが把握しやすいから！
	
	・｛｝は改行してやる。
	例)
	if ()
	{
	}
	理由：クラスを定義するときもカッコは改行してるから統制している。

	・条件式では否定系を使うときは、falseを使おう！
	例)
	if (hogehoge == false)
	理由：！を使うとたまに見落としてしまう目ん玉してるから


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
#include	"DX_System.h"
#include	"DX_Debug.h"
#include	"DX_Graphics.h"
#include	"DX_RenderState.h"
#include	"DX_Shader.h"
#include	"DX_ShaderManager.h"
#include	"DX_BufferCreater.h"
#include	"DX_Resource.h"
#include	"DX_TextureManager.h"
#include	"DX_Instance2DObject.h"
#include	"DX_Audio.h"
#include	"DX_Input.h"
#include	"DX_XInput.h"
#include	"DX_Font.h"
#include	"DX_Text.h"

#endif // !__DX_LIBRARY_H_
