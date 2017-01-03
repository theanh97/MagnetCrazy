#ifndef __UTILS_H__
#define __UTILS_H__
#include"cocos2d.h"
class Utils {
public:
	static Utils* getInstance();
	float randomValueBetween(float a, float b); 
private:
	static Utils *s_utils;
};
#endif !__UTILS_H__
