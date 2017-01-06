#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__
#pragma once
#include "MyStringUtils.h"
#include <stdio.h>
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "MyAnimation.h"
#include "Utils.h"
#include "MagnetObject.h"
#include "Constants.h"
#include "MyBodyParser.h"
#include "InfiniteParallaxNode.h"
#include <fstream>
#include "FileOperation.h"
#include "AdmobHelper.h"
#include "SimpleAudioEngine.h"
enum ElectricState {
	POSITIVE = 0,
	NEGATIVE = 1,
	BALANCE = 2
};

enum ElectricLevel {
	NONE = 0,
	LOW,
	MEDIUM,
	HIGH
};

class GamePlayScene : public cocos2d::Layer
{
private:
	cocos2d::Size visibleSize;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	
	// LISTENER 
	void registerListener();
	bool onCollisionLister(cocos2d::PhysicsContact & contact);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

	// PHYSIC 
	cocos2d::PhysicsWorld *m_world;
	void setPhysicWorld(cocos2d::PhysicsWorld *world) { m_world = world; }
	void addPhysicWorldBox();
	void addSpriteFrameToCache();

	// OBJECT 
	MagnetObject *magnet;
	cocos2d::Vector<cocos2d::Sprite*> barrierArray;
	cocos2d::PhysicsBody *magnetBody;
	void addMagnet();
	void addBarrier(float delta);

	// GUI 
	cocos2d::Sprite * arrowUp;
	cocos2d::Sprite * arrowDown;
	void addArrow();
	LabelTTF *labelScore ;
	void addLabelScore();
	void addTableScore(); 
	void addLoading();
	void updateScore(float dt);
	ui::LoadingBar * loadingBar; 
	cocos2d::Sprite * loadingBarBackground;
	cocos2d::Sprite * s;
	cocos2d::Sprite * n;
	cocos2d::Sprite * tableScore;
	LabelTTF * currentScore;
	LabelTTF * bestScore ;
	cocos2d::ui::Button * buttonExit;
	cocos2d::ui::Button * buttonReplay;
	void updateGameScene();
	void hideLoading(bool a);
	void hideTabelScore(bool a);
	void hideArrow();
	int score = 0;
	int bsScore = 0;

	// GAME STATE + LEVEL + POINT
	LabelTTF * positiveLabel;
	LabelTTF * negativeLabel;
	float startCountScore = 0; // >=3  -> score ++
	bool isPlayGame = false;
	bool isStartGame = true; 
	int isFirstPlayGame = 0;
	bool isFirstEndGame = true;
	bool isFirstCreate = true;
	void updateMagnet(float dt);
	ElectricState state = BALANCE;
	ElectricLevel level = NONE;
	void setRandomElectricStateAndLevel();
	void updateElectricState(float dt);
	void gameOver();
	void exitGame();
	void replayGame();

	// BACKGROUND
	void addBackground(); 
	InfiniteParallaxNode* backgroundScrolling;
	void updateBackground(float dt); 
	void addGameScene(); 

	// GAME GUIDE
	short isShowGameGuide; 
	void showFirstGameGuide();
	void showSecondGameGuide();
	void showTapToPlay();
	void hideTapToPlay();

	CREATE_FUNC(GamePlayScene);
};

#endif // __GAMEPLAY_SCENE_H__
