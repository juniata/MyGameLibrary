#ifndef __CIRCULARBARRAGE_H_
#define __CIRCULARBARRAGE_H_
class Bullet;

//****************************************************************************************************
//
//	CircularBarrage Class
//  円形に生成するタイプの弾幕クラス
//
//****************************************************************************************************
class CircularBarrage
{
private:
	// 生成する弾の数
	static const int BULLET_MAX = 12;
	// 弾リスト
	Bullet* bulletList[CircularBarrage::BULLET_MAX];

	// 有効フラグ
	bool isEnabled;
public:
	CircularBarrage();
	~CircularBarrage();

	void Update(float angle, float distance, DirectX::XMFLOAT3* pPosList);

	//------------------------------------------------------------------------------
	//
	//  @brief		有効フラグセット
	//  @param[in]  isEnabled セットする有効フラグ(bool)
	//  @detail     引数の値によってUpdateとRenderの動作制御
	//
	//------------------------------------------------------------------------------
	void SetEnabled(bool isEnabled);

	//------------------------------------------------------------------------------
	//
	//  @brief		有効フラグセット
	//
	//------------------------------------------------------------------------------
	void Response();
};

#endif