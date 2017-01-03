// to enable CCLOG()
#define COCOS2D_DEBUG 1

#include "cocos2d.h"
#include "FileOperation.h"
#include <stdio.h>
#include "MyStringUtils.h"
using namespace std;

void FileOperation::saveFile(int score)
{
	string path = getFilePath();
	FILE *fp = fopen(path.c_str(), "w");

	if (! fp)
	{
		CCLOG("can not create file %s", path.c_str());
		return;
	}
	string tmp = MyStringUtils::int_to_string(score);
	//string tmp = std::to_string(score);
	char tab2[1024];
	strncpy(tab2, tmp.c_str(), sizeof(tab2));
	tab2[sizeof(tab2) - 1] = 0;
	fputs(tab2, fp);
	fclose(fp);
}

void FileOperation::readFile(int &score)
{
	string path = getFilePath();
	FILE *fp = fopen(path.c_str(), "r");
	char buf[50] = {0};

	if (! fp)
	{
		CCLOG("can not open file %s", path.c_str());
		return;
	}

	fgets(buf, 50, fp);
	score = atoi(buf);
	fclose(fp);
}

string FileOperation::getFilePath()
{
	string path("");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// In android, every programe has a director under /data/data.
	// The path is /data/data/ + start activity package name.
	// You can save application specific data here.
	path.append("/data/data/org.cocos2dx.application/tmpfile");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// You can save file in anywhere if you have the permision.
	path.append("D:/tmpfile");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)
	path = cocos2d::CCApplication::sharedApplication().getAppDataPath();

#ifdef _TRANZDA_VM_
	// If runs on WoPhone simulator, you should insert "D:/Work7" at the
	// begin. We will fix the bug in no far future.
	path = "D:/Work7" + path;
	path.append("tmpfile");
#endif

#endif

	return path;
}