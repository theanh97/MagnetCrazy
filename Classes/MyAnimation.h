#ifndef __MY_ANIMATION_H__
#define __MY_ANIMATION_H__
#include"cocos2d.h"
class MyAnimation {
public:
	static MyAnimation * getInstance();
	cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char * format, int count);
	cocos2d::Animate * createAnimate(const char*format, int count, float animateFrame);
private:
	static MyAnimation *s_animation;
};
#endif !__MY_ANIMATION_H__