#include "AppDelegate.h"
#include "GamePlayScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

static cocos2d::Size designSize = cocos2d::Size(576, 1024);
static cocos2d::Size resolutionSize = cocos2d::Size(576, 1024);
const Vec2 RATIO_TABLE[] = {
	Vec2(4, 3),
	Vec2(15, 9),
	Vec2(16, 9),
	Vec2(3, 2)
};

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) {
        glview = GLViewImpl::createWithRect("HelloCpp", Rect(0, 0, designSize.width,designSize.height));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(resolutionSize.width,resolutionSize.height, ResolutionPolicy::FIXED_WIDTH);

    //// turn on display FPS
    //director->setDisplayStats(true);

    //// set FPS. the default value is 1.0/60 if you don't call this
    //director->setAnimationInterval(1.0 / 60);

	// add source 
	std::vector<std::string> res; 
	res.push_back("res/object");
	res.push_back("res/fonts");
	res.push_back("res/buttons");
	res.push_back("res/background");
	res.push_back("res/audio");

    FileUtils::getInstance()->setSearchPaths(res);
    // create a scene. it's an autorelease object
    auto scene = GamePlayScene::createScene();
	
	// preload audio 
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("tap_screen.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("tap_menu.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("tap_menu.wav");

    // run
    director->runWithScene(scene);
	
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     //SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
