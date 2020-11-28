#ifndef __DX_LIBRARY_H_
#define __DX_LIBRARY_H_

/*
	DirectX��p�����Q�[���G���W���ł��B
	Windwos10�ɑΉ��B
	DreictX11�ō쐬���B
	�̂���DirectX12�ɑΉ������܂��B
	DirectX API�����b�v���Ďg���₷�����ăN���X�Q������܂��B

	�g���₷���d���Ő݌v���܂��B


	�R�[�e�B���O�K��
	�E�����o�ϐ���m_������B
	���R�F�N���X���Ŏg�p����ہA�����o�ϐ��ɉ������邩���c�����₷������I
	
	�E�o�p�͉��s���Ă��B
	��)
	if ()
	{
	}
	���R�F�N���X���`����Ƃ����J�b�R�͉��s���Ă邩�瓝�����Ă���B

	�E�������ł͔ے�n���g���Ƃ��́Afalse���g�����I
	��)
	if (hogehoge == false)
	���R�F�I���g���Ƃ��܂Ɍ����Ƃ��Ă��܂��ڂ�ʂ��Ă邩��


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
