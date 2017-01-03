#include "GamePlayScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyStringUtils.h"
USING_NS_CC;
using namespace cocostudio::timeline;
const char * APPACTIVITY = "org/cocos2dx/cpp/AppActivity";
#if(CC_TARGET_PLATFORM== CC_PLATFORM_ANDROID   )
#include"platform/android/jni/JniHelper.h"
#include<jni.h>
static short callGetBestScore() {
	JniMethodInfo methodInfo;
	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, APPACTIVITY, "getBestScore", "()S")) {
		jshort a = methodInfo.env->CallStaticShortMethod(methodInfo.classID, methodInfo.methodID);
		short bs = a;

		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		return bs;
	}
	return -1;
}

static void callSetBestScore(short bestScore) {
	JniMethodInfo methodInfo;
	jshort bs = bestScore;
	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, APPACTIVITY, "updateBestScore", "(S)V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, bs);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	return;
}

static short callGetGameGuideFlag() {
	JniMethodInfo methodInfo;
	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, APPACTIVITY, "getGameGuideFlag", "()S")) {

		jshort a = methodInfo.env->CallStaticShortMethod(methodInfo.classID, methodInfo.methodID);
		short result = a;
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		return result;
	}
	return -1;
}
#endif
//jstring stringArg = methodInfo.env->NewStringUTF(msg);
//methodInfo.env->DeleteLocalRef(stringArg);
bool GamePlayScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();

	addSpriteFrameToCache();

	addPhysicWorldBox();

	registerListener();

	addGameScene();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdmobHelper::hideAd();
	// Game Guide 
	isShowGameGuide = callGetGameGuideFlag();
	if (isShowGameGuide == 1) {
		isShowGameGuide++;
		showFirstGameGuide();
	}else 
		showTapToPlay();
#endif 
	return true;
}

Scene* GamePlayScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	auto layer = GamePlayScene::create();
	layer->setPhysicWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

void GamePlayScene::addMagnet()
{
	magnet = MagnetObject::createMagnet();
	magnet->setPositive(50);
	magnet->setNegative(50);
	magnet->setScaleX(0.25f);
	magnet->setScaleY(0.20f);
	MyBodyParser::getInstance()->parseJsonFile("magnet_body.json");
	magnetBody = MyBodyParser::getInstance()->bodyFormJson(magnet, "magnet_body", PHYSICSBODY_MATERIAL_DEFAULT);
	magnetBody->setDynamic(true);
	magnetBody->setMass(2.0f);
	magnetBody->setCollisionBitmask(Constants::getInstance()->COLLISION_MAGNET);
	magnet->setTag(Constants::getInstance()->TAG_MAGNET);
	magnetBody->setContactTestBitmask(true);
	magnet->setPhysicsBody(magnetBody);
	magnet->setPosition(visibleSize / 2);
	this->addChild(magnet, Constants::getInstance()->ORDER_MAGNET);
}

void GamePlayScene::addBarrier(float delta)
{
	if (isPlayGame) {

		// create top + bottom pipe 
		auto topPipe = Sprite::create("barrier_1.png");
		auto bottomPipe = Sprite::create("barrier_1.png");

		topPipe->setScale(0.5f);
		bottomPipe->setScale(0.5f);

		float x = topPipe->getContentSize().width * 0.5f;
		float y = topPipe->getContentSize().height * 0.5f;

		// ---------Physic----------
		auto topPhysic = PhysicsBody::createBox(topPipe->getContentSize());
		auto bottomPhysic = PhysicsBody::createBox(topPipe->getContentSize());

		topPhysic->setCollisionBitmask(Constants::getInstance()->COLLISION_BARRIER);
		topPhysic->setContactTestBitmask(true);
		topPhysic->setDynamic(false);

		bottomPhysic->setCollisionBitmask(Constants::getInstance()->COLLISION_BARRIER);
		bottomPhysic->setContactTestBitmask(true);
		bottomPhysic->setDynamic(false);


		topPipe->setPhysicsBody(topPhysic);
		bottomPipe->setPhysicsBody(bottomPhysic);

		// ---------------------------------------

		// create random height of pipe 
		auto random = cocos2d::RandomHelper::random_real(0.0, 1.0);

		// set top pipe postion
		auto topPipePosition = visibleSize.height * 0.75 + random * y;
		topPipe->setPosition(Vec2(visibleSize.width + x, topPipePosition));


		// set bottom pipe position 
		auto bottomPipePosition = topPipePosition - magnet->getContentSize().height * 0.25f * 2.2f - y;
		bottomPipe->setPosition(Vec2(topPipe->getPositionX(), bottomPipePosition));

		// move
		auto topMove = MoveBy::create(10, Vec2(-visibleSize.width * 2, 0));
		auto bottomMove = MoveBy::create(10, Vec2(-visibleSize.width * 2, 0));

		// run action 
		auto topAnimate = MyAnimation::getInstance()->createAnimate("barrier_%d.png", 2, 1.0f / 8);
		auto bottomAnimate = MyAnimation::getInstance()->createAnimate("barrier_%d.png", 2, 1.0f / 8);

		auto callRemoveTopPipe = CallFunc::create([&]() {
			this->removeChild(topPipe);
		});
		auto callRemoveBottomPipe = CallFunc::create([&]() {
			this->removeChild(bottomPipe);
		});

		topPipe->runAction(Sequence::createWithTwoActions(Spawn::create(topMove, Repeat::create(topAnimate, 80), NULL), callRemoveTopPipe));
		bottomPipe->runAction(Sequence::createWithTwoActions(Spawn::create(bottomMove, Repeat::create(bottomAnimate, 80), NULL), callRemoveBottomPipe));

		topPipe->setTag(Constants::getInstance()->TAG_TOP_BARRIER);
		bottomPipe->setTag(Constants::getInstance()->TAG_BOTTOM_BARRIER);

		// add pipe
		this->addChild(topPipe);
		this->addChild(bottomPipe);

		// add barrier to array 
		barrierArray.pushBack(topPipe);
		barrierArray.pushBack(bottomPipe);
	}
}

void GamePlayScene::addArrow()
{
	if (isFirstCreate) {
		arrowUp = Sprite::create("set_S.png");
		arrowDown = Sprite::create("set_N.png");
		arrowUp->setPosition(Vec2(80, magnet->getPositionY() + 50));
		arrowDown->setPosition(Vec2(80, magnet->getPositionY() + 50));
		arrowUp->setVisible(false);
		arrowDown->setVisible(false);
		arrowUp->setScale(0.3f);
		arrowDown->setScale(0.3f);
		arrowUp->setTag(Constants::getInstance()->TAG_ARROW_UP);
		arrowDown->setTag(Constants::getInstance()->TAG_ARROW_DOWN);
		this->addChild(arrowUp);
		this->addChild(arrowDown);
	}
}

void GamePlayScene::gameOver()
{

	// show admob 
	AdmobHelper::showAd();

	isStartGame = false;
	isPlayGame = false;

	// stop schedule 
	unscheduleAllSelectors();

	// clear barrier
	if (barrierArray.size() >= 2) {
		this->removeChild(barrierArray.at(barrierArray.size() - 1));
		this->removeChild(barrierArray.at(barrierArray.size() - 2));
	}
	if (barrierArray.size() >= 4) {
		this->removeChild(barrierArray.at(barrierArray.size() - 3));
		this->removeChild(barrierArray.at(barrierArray.size() - 4));
	}

	// update score 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bsScore = (int)callGetBestScore();
	if (score > bsScore) {
		bsScore = score;
		callSetBestScore((short)bsScore);
	}
#endif 
	currentScore->setString("Score : " + MyStringUtils::int_to_string(score));
	bestScore->setString("Best Score : " + MyStringUtils::int_to_string(bsScore));

	// hide child 
	hideLoading(true);
	hideArrow();
	hideTabelScore(false);

	magnet->setVisible(false);

}

void GamePlayScene::exitGame()
{
	Director::getInstance()->end();
}

void GamePlayScene::replayGame()
{

	// admob 
	AdmobHelper::hideAd();

	// set magent 
	magnet->setVisible(true);
	magnet->setPosition(visibleSize / 2);
	magnet->getPhysicsBody()->setVelocity(Vec2(0, 0));
	magnet->getPhysicsBody()->setAngularVelocity(0);
	magnet->setRotation(0);

	// update gravity 
	this->getScene()->getPhysicsWorld()->setGravity(Vect(0, 0));
	magnet->setPositive(50);
	magnet->setNegative(50);

	// hide + show 
	hideTabelScore(true);
	hideLoading(false);

	//addMagnet();
	score = 0;
	labelScore->setString("Score : 0");

	isStartGame = true;
	isPlayGame = true;
	backgroundScrolling->setPosition(Vec2(0, 0));
	updateGameScene();
}

void GamePlayScene::addLabelScore()
{
	std::string sc = "Score : " + MyStringUtils::int_to_string(score);
	labelScore = LabelTTF::create(sc, "Marker Felt.ttf", 30);
	labelScore->setTag(Constants::getInstance()->TAG_LABEL_SCORE);
	labelScore->setPosition(Vec2(visibleSize.width / 2, loadingBar->getPositionY() - 40));
	this->addChild(labelScore);
}

void GamePlayScene::addTableScore()
{
	// Table Score 
	tableScore = Sprite::create("table_score.png");
	tableScore->setPosition(visibleSize / 2);
	tableScore->setTag(Constants::getInstance()->TAG_TABEL_SCORE);
	tableScore->setScale(0.3f);
	this->addChild(tableScore, Constants::getInstance()->ORDER_TABLE_SCORE);


	// Best Score 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bsScore = (int)callGetBestScore();
#endif
	std::string sc = "Best Score : " + MyStringUtils::int_to_string(bsScore);
	bestScore = LabelTTF::create(sc, "arial.ttf", 30);
	bestScore->setPosition(Vec2(tableScore->getPositionX(), tableScore->getPositionY() + 60));
	bestScore->setTag(Constants::getInstance()->TAG_BEST_SCORE);
	this->addChild(bestScore, Constants::getInstance()->ORDER_TEXT_SCORE);



	// Current Score 
	std::string sc1 = "Score : " + MyStringUtils::int_to_string(score);
	currentScore = LabelTTF::create(sc1, "arial.ttf", 30);
	currentScore->setTag(Constants::getInstance()->TAG_CURRENT_SCORE);
	currentScore->setPosition(Vec2(tableScore->getPositionX(), tableScore->getPositionY()));
	this->addChild(currentScore, Constants::getInstance()->ORDER_TEXT_SCORE);

	// button replay 
	buttonReplay = ui::Button::create("replay_normal.png", "replay_click.png");
	buttonReplay->setTag(Constants::getInstance()->TAG_BUTTON_REPLAY);
	buttonReplay->setScale(1.2f);
	buttonReplay->setPosition(Vec2(tableScore->getPositionX() - 70, tableScore->getPositionY() - 70));
	buttonReplay->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tap_menu.wav", false);
			replayGame();
		}
	});
	this->addChild(buttonReplay, 12);

	// button exit 
	buttonExit = ui::Button::create("exit_normal.png", "exit_click.png");
	buttonExit->setScale(1.2f);
	buttonExit->setPosition(Vec2(tableScore->getPositionX() + 70, tableScore->getPositionY() - 70));
	buttonExit->setTag(Constants::getInstance()->TAG_BUTTON_EXIT);
	buttonExit->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tap_menu.wav",false);
			exitGame();
		}
	});
	this->addChild(buttonExit, 12);
}

void GamePlayScene::addLoading()
{
	// add loading bar background 
	loadingBarBackground = Sprite::create("loading_bar_S.png");
	loadingBarBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
	loadingBarBackground->setScale(0.4f);
	loadingBarBackground->setTag(Constants::getInstance()->TAG_LOADING_BAR_BACKGROUND);
	this->addChild(loadingBarBackground, Constants::getInstance()->ORDER_BACKGROUND_LOADING_BACKGROUND);

	// add loading bar 
	loadingBar = ui::LoadingBar::create("loading_bar_N.png");
	loadingBar->setDirection(ui::LoadingBar::Direction::RIGHT);
	loadingBar->setScale(0.4f);
	loadingBar->setTag(Constants::getInstance()->TAG_LOADING_BAR);
	loadingBar->setPosition(loadingBarBackground->getPosition());
	loadingBar->setPercent(50);
	this->addChild(loadingBar, Constants::getInstance()->ORDER_BACKGROUND_LOADING);

	// add S && N 
	s = Sprite::create("S.png");
	n = Sprite::create("N.png");
	s->setPosition(Vec2(loadingBarBackground->getPositionX() - loadingBarBackground->getContentSize().width * 0.4 / 2 - 30, loadingBarBackground->getPositionY()));
	n->setPosition(Vec2(loadingBarBackground->getPositionX() + loadingBarBackground->getContentSize().width * 0.4 / 2 + 30, loadingBarBackground->getPositionY()));
	s->setScale(0.5f);
	n->setScale(0.5f);
	s->setTag(Constants::getInstance()->TAG_S);
	n->setTag(Constants::getInstance()->TAG_N);
	this->addChild(s, Constants::getInstance()->ORDER_BACKGROUND_LOADING_SN);
	this->addChild(n, Constants::getInstance()->ORDER_BACKGROUND_LOADING_SN);
}

void GamePlayScene::updateScore(float dt)
{
	// score 
	startCountScore += dt;
	if (startCountScore >= 3.0f)
		if (isPlayGame) {
			score += 1;
			std::string sc = "Score : " + MyStringUtils::int_to_string(score);
			labelScore->setString(sc);
		}
}

void GamePlayScene::updateGameScene()
{
	setRandomElectricStateAndLevel();

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GamePlayScene::updateBackground), this, 1.0f / 60, false);
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GamePlayScene::updateMagnet), this, 0.1f, false);
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GamePlayScene::updateElectricState), this, 2.0f, false);
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GamePlayScene::addBarrier), this, 3.0f, false);
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GamePlayScene::updateScore), this, 1.0f, false);

	isFirstCreate = false;
}

void GamePlayScene::hideLoading(bool a)
{
	if (a == false) {
		s->setVisible(true);
		n->setVisible(true);
		loadingBar->setVisible(true);
		loadingBarBackground->setVisible(true);
		labelScore->setVisible(true);
	}
	else {
		labelScore->setVisible(false);
		s->setVisible(false);
		n->setVisible(false);
		loadingBar->setVisible(false);
		loadingBarBackground->setVisible(false);
	}
}

void GamePlayScene::hideTabelScore(bool a)
{
	if (a == true) {
		tableScore->setVisible(false);
		currentScore->setVisible(false);
		bestScore->setVisible(false);
		buttonExit->setVisible(false);
		buttonReplay->setVisible(false);
	}
	else {
		tableScore->setVisible(true);
		currentScore->setVisible(true);
		bestScore->setVisible(true);
		buttonExit->setVisible(true);
		buttonReplay->setVisible(true);
	}
}

void GamePlayScene::hideArrow()
{
	arrowDown->setVisible(false);
	arrowUp->setVisible(false);
}

void GamePlayScene::updateMagnet(float dt)
{
	if (isPlayGame) {
		// magnet 
		int positive = magnet->getPositive();
		int negative = magnet->getNegative();
		if (positive > 0 && positive < 100) {
			if (state == POSITIVE) {
				int a = magnet->getPositive() + level;
				if (a > 100)
					a = 100;
				magnet->setPositive(a);
			}
			else {
				int a = magnet->getPositive() - level;
				if (a < 0)
					a = 0;
				magnet->setPositive(a);
			}
		}
		else {
			if (positive <= 0 && state == POSITIVE)
				magnet->setPositive(level);
			if (positive >= 100 && state == NEGATIVE)
				magnet->setPositive(100 - level);
		}
		magnet->setNegative(100 - magnet->getPositive());
		// gravity 
		if (positive > 50) {
			this->getScene()->getPhysicsWorld()->setGravity(Vect(0, -(positive - 50) * 25));
			arrowDown->setVisible(true);
			arrowUp->setVisible(false);
		}
		if (positive < 50) {
			this->getScene()->getPhysicsWorld()->setGravity(Vect(0, +(negative - 50) * 25));
			arrowDown->setVisible(false);
			arrowUp->setVisible(true);
		}
		// update loading bar 
		loadingBar->setPercent(magnet->getPositive());
	}
}

void GamePlayScene::setRandomElectricStateAndLevel()
{
	if (isPlayGame == true) {
		// LEVEL
		float rd = Utils::getInstance()->randomValueBetween(1, 3);
		if (rd < 1.5)
			level = LOW;
		else if (rd < 2.5)
			level = MEDIUM;
		else
			level = HIGH;

		// STATE 
		if (Utils::getInstance()->randomValueBetween(0, 1) >= 0.5f) {
			state = POSITIVE;
		}
		else
			state = NEGATIVE;
	}
}

void GamePlayScene::updateElectricState(float dt)
{
	if (isPlayGame)
		setRandomElectricStateAndLevel();
}

void GamePlayScene::addBackground()
{
	backgroundScrolling = InfiniteParallaxNode::create();
	// add sky to background
	int skyQuantity = 3;
	float  y = 0;
	for (int i = 0; i < skyQuantity; i++)
	{
		auto sky = Sprite::create("background.png");
		sky->setAnchorPoint(Point::ZERO);
		/*sky->setScale(0.8f);*/
		sky->setTag(1000 + i);
		backgroundScrolling->addChild(sky,
			// Set random z-index from [-10,-6]
			Constants::getInstance()->ORDER_BACKGROUND_SKY,
			// Set ration (skys moves slow)
			Point(0.01f, 0.0),
			// Set position with random component
			Point(y, 0));
		y += sky->getContentSize().width /** 0.8f*/;
	}
	// add land 
	int landQuantity = 5;
	float  x = 0;
	for (int i = 0; i < landQuantity; i++)
	{
		auto land = Sprite::create("land_2.png");
		land->setAnchorPoint(Point::ZERO);
		//land->setScale(0.8f);
		land->setTag(1000 + i);
		auto landBody = PhysicsBody::createBox(land->getContentSize() * 0.8, PHYSICSBODY_MATERIAL_DEFAULT);
		landBody->setDynamic(false);
		landBody->setCollisionBitmask(Constants::getInstance()->COLLISION_LAND);
		landBody->setContactTestBitmask(true);
		land->setPhysicsBody(landBody);
		backgroundScrolling->addChild(land,
			// Set random z-index from [-10,-6]
			Constants::getInstance()->ORDER_BACKGROUND_LAND,
			// Set ration (skys moves slow)
			Point(0.2f, 0.0),
			// Set position with random component
			Point(x, 0));
		x += land->getContentSize().width /** 0.8f*/ - 300;
	}

	this->addChild(backgroundScrolling, Constants::getInstance()->ORDER_BACKGROUND);
}

void GamePlayScene::updateBackground(float dt)
{
	Point scrollDecrement = Point(5, 0);
	backgroundScrolling->setPosition(backgroundScrolling->getPosition() - scrollDecrement);
	backgroundScrolling->updatePosition();
}

void GamePlayScene::addGameScene()
{
	if (isFirstCreate == true) {
		addBackground();
		addMagnet();
		addArrow();
		addLoading();
		addLabelScore();
		addTableScore();

		hideLoading(true);
		hideTabelScore(true);
	}
}

void GamePlayScene::showFirstGameGuide()
{
	auto Down = Sprite::create("set_N.png");
	Down->setScale(0.3f);
	Down->setPosition(Vec2(100, visibleSize.height / 2));
	Down->setTag(Constants::getInstance()->TAG_GUIDE_ARROW_DOWN);
	this->addChild(Down, Constants::getInstance()->ORDER_TEXT_SCORE);

	auto Next = Sprite::create("arrow_next.png");
	Next->setScale(0.1f);
	Next->setTag(Constants::getInstance()->TAG_GUIDE_ARROW_NEXT);
	Next->setPosition(Vec2(visibleSize.width/3 + 20, visibleSize.height / 2));
	this->addChild(Next, Constants::getInstance()->ORDER_TEXT_SCORE);

	auto imageView = ui::ImageView::create("magnet_1.png");
	imageView->setScaleX(0.25f);
	imageView->setScaleY(0.20f);

	imageView->setTag(Constants::getInstance()->TAG_GUIDE_MAGNET);
	imageView->setPosition(Vec2(visibleSize.width/2 + 80, visibleSize.height/2 + 80));
	this->addChild(imageView,Constants::getInstance()->ORDER_MAGNET);

	auto tap = ui::ImageView::create("tap.png");
	tap->setScale(0.5f);
	tap->setTag(Constants::getInstance()->TAG_GUIDE_TAP_MOUSE);
	tap->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2 - 180));
	this->addChild(tap, Constants::getInstance()->ORDER_MAGNET);

	auto tapLeft = ui::ImageView::create("tap_left.png");
	tapLeft->setScale(1.2f);
	tapLeft->setTag(Constants::getInstance()->TAG_GUIDE_TAP_LEFT);
	tapLeft->setPosition(Vec2(tap->getPosition().x - 60, tap->getPositionY() + tap->getContentSize().height/2 * 0.5));
	this->addChild(tapLeft, Constants::getInstance()->ORDER_MAGNET);

	auto tapRight = ui::ImageView::create("tap_right.png");
	tapRight->setScale(1.2f);
	tapRight->setTag(Constants::getInstance()->TAG_GUIDE_TAP_RIGHT);
	tapRight->setPosition(Vec2(tap->getPosition().x + 60, tap->getPositionY() + tap->getContentSize().height / 2 * 0.5));
	this->addChild(tapRight, Constants::getInstance()->ORDER_MAGNET);

	auto tap2Gach = ui::ImageView::create("2_vach.png");
	tap2Gach->setScale(0.2f);
	tap2Gach->setTag(Constants::getInstance()->TAG_GUIDE_2_VACH);
	tap2Gach->setRotation(-75);
	tap2Gach->setPosition(Vec2(imageView->getPosition().x - 30, imageView->getPositionY() - 45));
	this->addChild(tap2Gach, Constants::getInstance()->ORDER_MAGNET);
}

void GamePlayScene::showSecondGameGuide()
{
	this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_ARROW_DOWN);

	auto Up = Sprite::create("set_S.png");
	Up->setScale(0.3f);
	Up->setPosition(Vec2(100, visibleSize.height / 2));
	Up->setTag(Constants::getInstance()->TAG_GUIDE_ARROW_UP);
	this->addChild(Up, Constants::getInstance()->ORDER_TEXT_SCORE);

	this->getChildByTag(Constants::getInstance()->TAG_GUIDE_MAGNET)->setPosition(Vec2(visibleSize.width / 2 + 80, visibleSize.height / 2 - 80));
	auto imageView = this->getChildByTag(Constants::getInstance()->TAG_GUIDE_MAGNET);
	auto tap2Vach = this->getChildByTag(Constants::getInstance()->TAG_GUIDE_2_VACH);
	tap2Vach->setRotation(15);
	tap2Vach->setPosition(Vec2(imageView->getPosition().x - 30, imageView->getPositionY() +45));
}

void GamePlayScene::showTapToPlay()
{
	auto tapToPlay = ui::ImageView::create("tap_to_play.png");
	tapToPlay->setScale(0.7f);
	tapToPlay->setTag(Constants::getInstance()->TAG_GUIDE_TAP_TO_PLAY);
	tapToPlay->setPosition(Vec2(visibleSize.width/2 + 10, visibleSize.height / 2 + 100));
	this->addChild(tapToPlay, Constants::getInstance()->ORDER_MAGNET);

	auto tap = ui::ImageView::create("tap.png");
	tap->setScale(0.5f);
	tap->setTag(Constants::getInstance()->TAG_GUIDE_TAP_MOUSE);
	tap->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 140));
	this->addChild(tap, Constants::getInstance()->ORDER_MAGNET);

	auto tapLeft= ui::ImageView::create("tap_left.png"); 
	tapLeft->setScale(1.2f);
	tapLeft->setPosition(Vec2(tap->getPosition().x - 60, tap->getPositionY() + tap->getContentSize().height / 2 * 0.5));
	tapLeft->setTag(Constants::getInstance()->TAG_GUIDE_TAP_LEFT);
	this->addChild(tapLeft, Constants::getInstance()->ORDER_MAGNET); 

	auto tapRight = ui::ImageView::create("tap_right.png");
	tapRight->setScale(1.2f);
	tapRight->setTag(Constants::getInstance()->TAG_GUIDE_TAP_RIGHT);
	tapRight->setPosition(Vec2(tap->getPosition().x + 60, tap->getPositionY() + tap->getContentSize().height / 2 * 0.5));
	this->addChild(tapRight, Constants::getInstance()->ORDER_MAGNET);
}

void GamePlayScene::hideTapToPlay()
{
	this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_TAP_LEFT);
	this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_TAP_RIGHT);
	this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_TAP_MOUSE);
	this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_TAP_TO_PLAY);
}

void GamePlayScene::addPhysicWorldBox()
{
	auto box = PhysicsBody::createEdgeBox(Size(visibleSize.width * 2, visibleSize.height), PHYSICSBODY_MATERIAL_DEFAULT, 0.2f);
	box->setCollisionBitmask(Constants::getInstance()->COLLISION_BOX);  // collision flag
	box->setContactTestBitmask(true);
	auto box_node = Node::create();   // create box_node
	box_node->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	box_node->setPhysicsBody(box);
	this->addChild(box_node, Constants::getInstance()->ORDER_BOX);
}

void GamePlayScene::addSpriteFrameToCache()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("barrier.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("magnet.plist");
}

void GamePlayScene::registerListener()
{
	// collision
	auto collisionListener = EventListenerPhysicsContact::create();
	collisionListener->onContactBegin = CC_CALLBACK_1(GamePlayScene::onCollisionLister, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(collisionListener, this);

	// touch
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GamePlayScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GamePlayScene::onCollisionLister(cocos2d::PhysicsContact & contact)
{
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	if ((bodyA->getCollisionBitmask() == Constants::getInstance()->COLLISION_BARRIER &&
		bodyB->getCollisionBitmask() == Constants::getInstance()->COLLISION_MAGNET) ||
		(bodyB->getCollisionBitmask() == Constants::getInstance()->COLLISION_BARRIER &&
			bodyA->getCollisionBitmask() == Constants::getInstance()->COLLISION_MAGNET) ||
			(bodyA->getCollisionBitmask() == Constants::getInstance()->COLLISION_LAND &&
				bodyB->getCollisionBitmask() == Constants::getInstance()->COLLISION_MAGNET) ||
				(bodyB->getCollisionBitmask() == Constants::getInstance()->COLLISION_LAND &&
					bodyA->getCollisionBitmask() == Constants::getInstance()->COLLISION_MAGNET) ||
					(bodyA->getCollisionBitmask() == Constants::getInstance()->COLLISION_BOX &&
						bodyB->getCollisionBitmask() == Constants::getInstance()->COLLISION_MAGNET) ||
						(bodyB->getCollisionBitmask() == Constants::getInstance()->COLLISION_BOX &&
							bodyA->getCollisionBitmask() == Constants::getInstance()->COLLISION_MAGNET)) {
		gameOver();
	}
	return true;
}

bool GamePlayScene::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (isShowGameGuide == 2) {
		isShowGameGuide++;
		showSecondGameGuide();
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tap_menu.wav",false);
		return true; 
	}
	else if (isShowGameGuide == 3) {
		isShowGameGuide++;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tap_menu.wav",false);
		this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_MAGNET, true);
		this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_2_VACH, true);
		this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_ARROW_DOWN, true);
		this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_ARROW_UP, true);
		this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_ARROW_NEXT, true);
		this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_TAP_MOUSE, true);
		this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_TAP_LEFT, true);
		this->removeChildByTag(Constants::getInstance()->TAG_GUIDE_TAP_RIGHT, true);
		showTapToPlay();
		return true;
	}
	// Game Play -> Tap
	else {
		isFirstPlayGame++;
		if (isStartGame) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tap_screen.mp3",false);
			if (isPlayGame == false) {
				hideTapToPlay();
				isPlayGame = true;
				score = 0;
				hideLoading(false);

				if (isFirstCreate)
					updateGameScene();
			}
			else {
				auto animation = MyAnimation::getInstance()->createAnimate("magnet_%d.png", 2, 1.0f / 16); 
				magnet->runAction(animation); 
				if (magnet->getPositive() > 50) {
					magnet->getPhysicsBody()->setVelocity(Vect(0, 0));
					magnet->getPhysicsBody()->applyImpulse(Vect(0, 700));
				}
				else if (magnet->getPositive() < 50)
				{
					magnet->getPhysicsBody()->setVelocity(Vect(0, 0));
					magnet->getPhysicsBody()->applyImpulse(Vect(0, -700));
				}
			}
		}
	}
	return true;
}


