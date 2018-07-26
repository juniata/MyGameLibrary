#ifndef __CIRCULARBARRAGE_H_
#define __CIRCULARBARRAGE_H_
class Bullet;

//****************************************************************************************************
//
//	CircularBarrage Class
//  �~�`�ɐ�������^�C�v�̒e���N���X
//
//****************************************************************************************************
class CircularBarrage
{
private:
	// ��������e�̐�
	const int BULLET_MAX = 12;
	// �e���X�g
	std::vector<Bullet*> bulletList;
	// �L���t���O
	bool isEnabled;
public:
	CircularBarrage();
	~CircularBarrage();

	void Update(float angle, float distance);
	void Render();

	//------------------------------------------------------------------------------
	//
	//  @brief		�L���t���O�Z�b�g
	//  @param[in]  isEnabled �Z�b�g����L���t���O(bool)
	//  @detail     �����̒l�ɂ����Update��Render�̓��쐧��
	//
	//------------------------------------------------------------------------------
	void SetEnabled(bool isEnabled);

	//------------------------------------------------------------------------------
	//
	//  @brief		�L���t���O�Z�b�g
	//
	//------------------------------------------------------------------------------
	void Response();
};

#endif