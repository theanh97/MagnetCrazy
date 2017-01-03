#ifndef __MAGNET_OBJECT_H__
#define __MAGNET_OBJECT_H__

#include "cocos2d.h"
#include <functional>
class MagnetObject;
// khai báo kiểu con trỏ hàm, sẽ sử dụng để đối tượng con (Card) 
// gọi lại đối tượng cha (HelloWorld)
//typedef std::function<void(PointObject* sender)> onPointCollisionListener;

class MagnetObject : public cocos2d::Sprite
{
private:
	int positive;
	int negative;
	//onPointCollisionListener onCollisionListener;	// con trỏ hàm để gọi lại đối tượng cha
public:
	// hàm static để tạo mới một đối tượng Card - làm theo kiểu Cocos2dx
	static MagnetObject* createMagnet();
	MagnetObject(void);
	virtual ~MagnetObject(void);

	inline int getPositive() { return positive; }
	inline int getNegative() { return negative; }
	inline void setPositive(int pos) { positive = pos; }
	inline void setNegative(int neg) { negative = neg; }
};

#endif // __MAGNET_OBJECT_H__