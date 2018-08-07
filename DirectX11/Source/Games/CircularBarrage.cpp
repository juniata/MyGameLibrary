#include	"DxLibrary\DX_Library.h"
#include	"Bullet.h"
#include	"CircularBarrage.h"

//-----------------------------------------------------------------------------------------
//
//  ������
//
//-----------------------------------------------------------------------------------------
CircularBarrage::CircularBarrage() :
	m_isEnabled(false)
{
	for (int i = 0; i < BULLET_MAX; ++i)
	{
		m_pBulletList[i] = new Bullet();
	}
}

//-----------------------------------------------------------------------------------------
//
//  ���
//
//-----------------------------------------------------------------------------------------
CircularBarrage::~CircularBarrage()
{
	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		DELETE_OBJ(m_pBulletList[i]);
	}
}

//-----------------------------------------------------------------------------------------
//
//  �X�V
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::Update(float angle, float distance, DirectX::XMFLOAT3* pPosList)
{
	if (m_isEnabled)
	{
		for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
		{
			// �O��̊p�x�Ƌ����ɂЂ�������Z���Ă����A�~�`�̒e���O���ɓ����悤�ɂ���B
			// �e����ʊO�ɍs���ƍX�V�ƕ`�悪�s���Ȃ��Ȃ�B
			// �X�V��̍��W��2D�C���X�^���X�I�u�W�F�N�g�̍��W�ɐݒ肷��B
			m_pBulletList[i]->SetAngle(m_pBulletList[i]->GetAngle() + angle);
			m_pBulletList[i]->SetDistance(m_pBulletList[i]->GetDistance() + distance);
			m_pBulletList[i]->Update();
			pPosList[i] = m_pBulletList[i]->GetPos();
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//  ���X�|�[��
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::Response()
{
	m_isEnabled = true;
	for (int i = 0; i < CircularBarrage::BULLET_MAX; i++)
	{
		m_pBulletList[i]->SetEnabled(true);
		m_pBulletList[i]->SetAngle(i * 30.0f);
		m_pBulletList[i]->SetDistance(0.0f);
	}
}


//-----------------------------------------------------------------------------------------
//
//  �e�̐����擾����
//
//-----------------------------------------------------------------------------------------
int CircularBarrage::GetBulletNum()
{
	return BULLET_MAX;
}

//-----------------------------------------------------------------------------------------
//
//  �L���t���O�Z�b�g
//
//-----------------------------------------------------------------------------------------
void CircularBarrage::SetEnabled(bool isEnabled)
{
	m_isEnabled = isEnabled;
}