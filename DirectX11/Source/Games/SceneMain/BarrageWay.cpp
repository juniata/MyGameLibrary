#include	"DxLibrary\DX_Library.h"
#include	"Bullet.h"
#include	"BarrageWay.h"

using namespace DirectX;

//------------------------------------------------------------------------------
//
//  �e���𐶐�
//
//------------------------------------------------------------------------------
BarrageWay::BarrageWay(const char* pFilepath, const UINT bulletNum, const DirectX::XMFLOAT2& size) :
	m_pInstance2D(new DX_Instance2DObject(pFilepath, bulletNum, size)),
	m_pBullets(new tagInstanceBullet[bulletNum]),
	m_bulletNum(bulletNum),
	m_size(size),
	m_shotInterval(0),
	m_way(1),
	m_bUseRefrect(false)
{
	XMFLOAT3* pList = m_pInstance2D->GetPosList();
	for (size_t i = 0; i < m_bulletNum; ++i)
	{
		m_pBullets[i].pos = reinterpret_cast<XMFLOAT2*>(&pList[i]);
		m_pInstance2D->Disable(i);
	}
	m_pInstance2D->Enable();
}

//------------------------------------------------------------------------------
//
//  �e�������
//
//------------------------------------------------------------------------------
BarrageWay::~BarrageWay()
{
	DELETE_OBJ(m_pInstance2D);
	DELETE_OBJ_ARRAY(m_pBullets);
}

//------------------------------------------------------------------------------
//
//  �e���̍��W�ƈړ��ʂ�ݒ�
//
//------------------------------------------------------------------------------
void BarrageWay::Set(const DirectX::XMFLOAT2& playerPos, const float moveSpeed, const int shotInterval)
{

	if (m_shotInterval < shotInterval) {
		++m_shotInterval;
		return;
	}
	m_shotInterval = 0;

	float angle[360];
	const float addRad1 = (180.0f / CAST_F(m_way + 1)) * (XM_PI / 180.0f);

	for (int i = 0; i < 360; ++i)
	{
		angle[i] = addRad1 * CAST_F(i + 1);
	}
	for (int count = 0; count < m_way; ++count)
	{
		for (UINT i = 0; i < m_bulletNum; ++i)
		{
			if (m_pInstance2D->IsEnable(i)) {
				continue;
			}
			m_pBullets[i].move.x = (-1.0f * cosf(angle[count])) * moveSpeed;
			m_pBullets[i].move.y = -sinf(angle[count]) * moveSpeed;
			m_pBullets[i].pos->x = playerPos.x;
			m_pBullets[i].pos->y = playerPos.y;
			m_pInstance2D->Enable(i);
			break;
		}
	}
}

//------------------------------------------------------------------------------
//
//  �e���̍��W���X�V
//
//------------------------------------------------------------------------------
void BarrageWay::Update()
{
	DX_System* pSystem = DX_System::GetInstance();

	const unsigned int height = pSystem->GetWindowHeight();
	const unsigned int width = pSystem->GetWindowWidth();

	for (UINT i = 0; i < m_bulletNum; ++i)
	{
		if (m_pInstance2D->IsDisable(i)) {
			continue;
		}
		m_pBullets[i].pos->y += m_pBullets[i].move.y;
		m_pBullets[i].pos->x += m_pBullets[i].move.x;

		if (m_pBullets[i].pos->y < -m_size.x || m_pBullets[i].pos->y > height ||
			m_pBullets[i].pos->x < -m_size.y || m_pBullets[i].pos->x > width
			) {
			m_pInstance2D->Disable(i);
		}
	}
}

//------------------------------------------------------------------------------
//
//  �`��
//
//------------------------------------------------------------------------------
bool BarrageWay::Render()
{
	return m_pInstance2D->Render();
}

//------------------------------------------------------------------------------
//
//  �e����������̐����擾����
//
//------------------------------------------------------------------------------
int BarrageWay::GetWayCount() const
{
	return m_way;
}

//------------------------------------------------------------------------------
//
//  �e����������̐���ݒ肷��
//
//------------------------------------------------------------------------------
void BarrageWay::SetWayCount(const int count)
{
	m_way = count;
}

//------------------------------------------------------------------------------
//
//	���t���N�g�@�\�𗘗p����
//
//------------------------------------------------------------------------------
void BarrageWay::UseRefrect()
{
	m_bUseRefrect = true;
}