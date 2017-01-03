#include "Utils.h"
Utils *Utils::s_utils = nullptr; 
Utils * Utils::getInstance()
{
	if (Utils::s_utils == nullptr) {
		Utils::s_utils = new Utils; 
	}
	return Utils::s_utils;
}

float Utils::randomValueBetween(float a , float b)
{
	return cocos2d::RandomHelper::random_real(0.0, 1.0) * (b - a ) + a;
}
