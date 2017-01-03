#include "MagnetObject.h"

MagnetObject * MagnetObject::createMagnet()
{
	auto p = new MagnetObject();
	p->initWithFile("magnet_1.png");
	p->autorelease();
	return p;
}

MagnetObject::MagnetObject(void)
{
}

MagnetObject::~MagnetObject(void)
{
}
