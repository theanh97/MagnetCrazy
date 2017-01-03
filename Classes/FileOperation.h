#ifndef __HELLOWORLD_FILE_OPERATION__
#define __HELLOWORLD_FILE_OPERATION__

#include <string>
#include "cocos2d.h"

class FileOperation 
{
public:
	static void saveFile(int score);
	static void readFile(int &score);
	static std::string getFilePath();
};

#endif
