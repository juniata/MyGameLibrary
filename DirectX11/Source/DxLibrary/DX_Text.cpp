#include "DX_Library.h"
#include "../extern/DirectXTex/DirectXTex.h"
#include <tchar.h>
#include <locale.h>

/// <summary>
/// �����o�ϐ��̏�����
/// </summary>
DX_Text::DX_Text() : m_vertexBuffer(nullptr), m_fontSampler(nullptr)
{
	DX_System*		system = DX_System::GetInstance();
	ID3D11Device*	device = system->GetDevice();
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samDesc, &m_fontSampler);

	//	���_�o�b�t�@���쐬
	DX::tagVertex2D vertices[] = {
		/* ���� */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
		/* ���� */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
		/* �E�� */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
	};

	m_vertexBuffer = DX_BufferCreater::VertexBuffer(sizeof(vertices), vertices);
}

/// <summary>
/// �����o�ϐ��̏������y�уT���v���[��o�b�t�@�̍쐬
/// </summary>
DX_Text::~DX_Text()
{
	for (auto itr = m_fontList.begin(); itr != m_fontList.end(); ++itr)
	{
		delete itr->second;
		itr->second = nullptr;
	}

	m_fontList.clear();
}

/// <summary>
/// ������\������
/// </summary>
/// <param name="x">�J�n�`��ʒu</param>
/// <param name="y">�J�n�`��ʒu</param>
/// <param name="width">�`�悷��ꕶ���̃T�C�Y(����)</param>
/// <param name="height">�`�悷��ꕶ���̃T�C�Y(����)</param>
/// <param name="pszFormat">�t�H�[�}�b�g�Bprintf���Ɠ����g����</param>
/// <param name="...">�ϒ������Bprintf���Ɠ����g����</param>
void DX_Text::Print(const float x, const float y, const float width, const float height, LPCSTR pszFormat, ...)
{
	va_list	argp;
	char pszBuf[256];

	va_start(argp, pszFormat);
	vsprintf_s(pszBuf, pszFormat, argp);
	va_end(argp);

	char message[256];
	sprintf_s(message, "%s", pszBuf);

	wchar_t msg[256];
	size_t ret = 0;
	mbstowcs_s(&ret, msg, 256, message, _TRUNCATE);

	size_t size = wcslen(msg);

	for (int i = 0; i < size; ++i)
	{
		wchar_t c = msg[i];

		DX::tagRect rect;
		rect.x = x + width * static_cast<float>(i);
		rect.y = y;
		rect.w = rect.x + width;
		rect.h = rect.y + height;

		// �L�[���Ȃ���ΐ�������
		if (!m_fontList.count(c))
		{
			m_fontList[c] = new DX_Font(&c);
		}
		// TODO:�Ȃ������x�r�q�u���擾����K�v������B�B�Ȃ��H
		m_fontList[c]->SetInfo(&c, rect);
		m_fontList[c]->Render(&m_vertexBuffer, m_fontSampler);
	}
}
