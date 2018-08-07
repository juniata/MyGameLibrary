#ifndef _BULLETMANAGER_H_
#define _BULLETMANAGER_H_
class CircularBarrage;

//****************************************************************************************************
//
//	CircularBarrageManager
//
//****************************************************************************************************
class CircularBarrageManager
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		������
	//
	//------------------------------------------------------------------------------
	CircularBarrageManager();

	//------------------------------------------------------------------------------
	//
	//  @brief		���
	//
	//------------------------------------------------------------------------------
	~CircularBarrageManager();

	//------------------------------------------------------------------------------
	//
	//  @brief		�X�V
	//
	//------------------------------------------------------------------------------
	void Update();

	//------------------------------------------------------------------------------
	//
	//  @brief		�`��
	//
	//------------------------------------------------------------------------------
	void Render();

private:
	// ��������~�`�e���̐�
	static const int CIRCULARBARRAGE_MAX = 15;

	// x�t���[�������ɉ~�`�e���𐶐�����
	static const int CREATE_CIRCULAR_BARRAGE_FRAME = 5;

	DX_2DObject* m_pCircle;

	// �T�[�N���I�u�W�F�N�g�ꗗ
	CircularBarrage* m_pCircularBarrageList[CircularBarrageManager::CIRCULARBARRAGE_MAX];

	// 2D�C���X�^���X�`��I�u�W�F�N�g
	DX_Instance2DObject* m_pInstance2d;

	// �e�L�����C���^�[�o��
	int m_interval;

	// �L��������e�̃C���f�b�N�X
	int m_enabledBulletIndex;

};

#endif