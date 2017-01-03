#include "MyAnimation.h"
MyAnimation * MyAnimation::s_animation = nullptr;

MyAnimation * MyAnimation::getInstance()
{
	if (nullptr == s_animation)
	{
		MyAnimation::s_animation = new MyAnimation();
	}
	return MyAnimation::s_animation;
}

cocos2d::Vector<cocos2d::SpriteFrame*> MyAnimation::getAnimation(const char * format, int count)
{
	auto spritecache = cocos2d::SpriteFrameCache::getInstance();
	cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, format, i);
		cocos2d::SpriteFrame *a = spritecache->getSpriteFrameByName(str);
		animFrames.pushBack(a);
	}
	return animFrames;
}

cocos2d::Animate * MyAnimation::createAnimate(const char*format, int count, float animateFrame)
{
	cocos2d::Vector<cocos2d::SpriteFrame*>a = getAnimation(format, count);
	a.reserve(count);
	cocos2d::Animation *animation_1 = cocos2d::Animation::createWithSpriteFrames(a, animateFrame);
	cocos2d::Animate* animate = cocos2d::Animate::create(animation_1);
	return animate;
}
