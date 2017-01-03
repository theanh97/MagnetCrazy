#include "Constants.h"
Constants * Constants::s_constants = nullptr;
Constants::Constants()
{
}

Constants * Constants::getInstance()
{
	if (Constants::s_constants == nullptr)
		Constants::s_constants = new Constants(); 
	return Constants::s_constants; 
}

Constants::~Constants()
{
}

