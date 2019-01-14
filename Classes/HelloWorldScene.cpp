#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "block.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	srand(time(0));//随机事件种子

	is_move = false;

	sum = 0;//用于统分
	
	grade = Label::create();
	grade->setPosition(visibleSize.width/2, visibleSize.height/6*5);
	grade->setString(String::createWithFormat("score:%d", sum)->getCString());
	grade->setSystemFontSize(100);
	grade->setColor(Color3B::BLACK);
	this->addChild(grade, 3);

	auto bgcolor = LayerColor::create(Color4B(124, 204, 255, 255));//设置背景颜色
	this->addChild(bgcolor, 0);


	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);//监听鼠标起始位置
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnd, this);//监听触摸结束位置
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);//注册监听事件

	for(int i=0; i<4; i++)
		for (int j = 0; j < 4; j++) {
			if (random_time())//随机的创建一些初始存在的方块，不存在则为NULL
			{
				block[i][j] = Block::createBlock();
				block[i][j]->setPosition(visibleSize.width/4 * i+5, visibleSize.height/6 * j+5);
				this->addChild(block[i][j], 3);
			}
			else {
				block[i][j] = NULL;
			}
		}
	add_grade();
    
	auto sprite = Sprite::create();

	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::add_grade() {//每次操作都加上所有的方块的分
	if (!is_move) {
		return;
	}
	for(int i=0; i<4; i++)
		for (int j = 0; j < 4; j++) {
			if (block[i][j]) {
				sum += block[i][j]->get_number();
				
			}
		}
	if (sum >= 100000) {
		grade->setSystemFontSize(60);
	} 
	else if(sum >= 10000) {
		grade->setSystemFontSize(70);
	}
	else if (sum >= 1000) {
		grade->setSystemFontSize(80);
	}
	grade->setString(String::createWithFormat("score:%d", sum)->getCString());
	is_move = false;//每次移动，添加方块完了之后，就重新开始
}

void HelloWorld::add_block()
{
	if (!is_move)
		return;//如果方块没有移动，那么不生成新的方块
	auto visib = Director::getInstance()->getWinSize();
	int x, y;
	bool flag = true;
	for(int i=0; i<4; i++)
		for (int j = 0; j < 4; j++) {
			if (block[i][j] == NULL)
			{
				flag = false;
				break;
			}
		}
	if (flag) {
		return;
	}
	do {
		x = rand() % 4;
		y = rand() % 4;
	} while (block[x][y] != NULL);
	block[x][y] = Block::createBlock();
	block[x][y]->setPosition(x*(visib.width / 4) + 5, y*(visib.height / 6) + 5);
	this->addChild(block[x][y], 3);
}

void HelloWorld::restart_scene()
{
	auto scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(scene);
}

void HelloWorld::resaart_button()
{
	auto visib = Director::getInstance()->getWinSize();
	auto bgcolor1 = LayerColor::create(Color4B(0, 0, 0, 190), visib.width, visib.height);
	this->addChild(bgcolor1, 4);
	auto restart = Label::create();
	restart->setString("restart");
	restart->setColor(Color3B(255, 255, 255));
	restart->setSystemFontSize(100);
	auto quit = Label::create();
	quit->setString("quit");
	quit->setColor(Color3B(255, 255, 255));
	quit->setSystemFontSize(100);
	auto restarmune = MenuItemLabel::create(restart, CC_CALLBACK_0(HelloWorld::restart_scene, this));
	auto quitmune = MenuItemLabel::create(quit, CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	auto menu = Menu::create(restarmune, quitmune, NULL);
	menu->alignItemsVertically();
	this->addChild(menu, 5);
}

void HelloWorld::can_move()
{
	bool flag = false;
	for (int i = 0; i<4; i++)
		for (int j = 0; j < 4; j++) {
			if (block[i][j] == NULL)
			{
				flag = true;;
				break;
			}
		}
	if (flag) {
		return;
	}
	/*for (int i = 3; i >= 0; i--) {
		CCLOG("%d %d %d %d", block[i][0]->get_number(), block[i][1]->get_number(), block[i][2]->get_number(), block[i][3]->get_number());
	}*/
	for (int i = 0; i < 3; i++) {
		if (block[i][0]->get_number() == block[i][1]->get_number() || block[i][1]->get_number() == block[i][2]->get_number() || block[i][2]->get_number() == block[i][3]->get_number() || block[i][0]->get_number() == block[i + 1][0]->get_number() || block[i][1]->get_number() == block[i + 1][1]->get_number() || block[i][2]->get_number() == block[i + 1][2]->get_number() || block[i][3]->get_number() == block[i + 1][3]->get_number()) {
			return;
		}
	}
	resaart_button();
}

bool HelloWorld::random_time()
{
	int ran = rand();
	if (ran % 4 == 0) {
		return true;
	}
	return false;
}

bool HelloWorld::onTouchBegan(Touch * touch, Event * event)
{
	auto touchPoint = touch->getLocation();
	star_x = touchPoint.x;
	star_y = touchPoint.y;
	return true;
}

bool HelloWorld::onTouchEnd(Touch * touch, Event * event)
{
	auto touchPoint = touch->getLocation();
	end_x = touchPoint.x;
	end_y = touchPoint.y;
	move_or_not(end_x-star_x, end_y-star_y);
	return true;
}



void HelloWorld::move_up() {
	auto visib = Director::getInstance()->getWinSize();
	for (int j = 2; j >= 0; j--)//第一行不动
		 for (int i = 0; i<4; i++){
			if (block[i][j] == NULL) {// 自己为空则跳过
				continue;
			}
			int jj = j;
			for (; jj < 3; jj++) {
				if (block[i][jj + 1] == NULL) {
					block[i][jj]->runAction(MoveBy::create(0.2f, Vec2(0, visib.height / 6)));
					block[i][jj + 1] = block[i][jj];
					block[i][jj] = NULL;
					is_move = true;
				}
				else
					break;
			}
			if (jj == 3)//没有
			{
				continue;
			}
			//否则执行判断相等，移动，合并
			int a = block[i][jj]->get_number();
			int b = block[i][jj +1]->get_number();
			if (a == b) {
				block[i][jj +1]->set_number(a * 2);
				auto seq = Sequence::create(MoveBy::create(0.2f, Vec2(0, visib.height/6)), RemoveSelf::create(true), NULL);
				block[i][jj]->runAction(seq);
				block[i][jj] = NULL;
				is_move = true;
			}
		}
	add_block();
	can_move();
	add_grade();
}

void HelloWorld::move_down() {
	auto visib = Director::getInstance()->getWinSize();
	for (int i = 0; i<4; i++) //第一行不动
		for (int j = 1; j < 4; j++){
			if (block[i][j] == NULL) {// 自己为空则跳过
				continue;
			}
			int jj = j;
			for (; jj > 0; jj--) {
				if (block[i][jj - 1] == NULL) {
					block[i][jj]->runAction(MoveBy::create(0.2f, Vec2(0, -visib.height / 6)));
					block[i][jj - 1] = block[i][jj];
					block[i][jj] = NULL;
					is_move = true;
				}
				else
					break;
			}
			if (jj == 0) {
				continue;
			}
			//否则执行判断相等，移动，合并
			int a = block[i][jj]->get_number();
			int b = block[i][jj - 1]->get_number();
			if (a == b) {
				block[i][jj - 1]->set_number(a * 2);
				auto seq = Sequence::create(MoveBy::create(0.2f, Vec2(0, -visib.height/6)), RemoveSelf::create(true), NULL);
				block[i][jj]->runAction(seq);
				block[i][jj] = NULL;
				is_move = true;
			}
		}
	add_block();
	can_move();
	add_grade();
}

void HelloWorld::move_left() {
	auto visib = Director::getInstance()->getWinSize();
	for (int j = 0; j < 4; j++)//第一列不动
		 for(int i=1; i<4; i++){
			if (block[i][j] == NULL) {// 自己为空则跳过
				continue;
			}
			int ii = i;
			for (; ii > 0; ii--) {
				if (block[ii - 1][j] == NULL) {
					block[ii][j]->runAction(MoveBy::create(0.2f, Vec2(-visib.width / 4, 0)));
					block[ii - 1][j] = block[ii][j];
					block[ii][j] = NULL;
					is_move = true;
				}
				else
					break;
			}
			if (ii == 0) {
				continue;
			}
			//否则执行判断相等，移动，合并
			int a = block[ii][j]->get_number();
			int b = block[ii-1][j]->get_number();
			if (a == b) {
				block[ii - 1][j]->set_number(a*2);
				auto seq = Sequence::create(MoveBy::create(0.2f, Vec2(-visib.width / 4, 0)), RemoveSelf::create(true),NULL);
				block[ii][j]->runAction(seq);
				block[ii][j] = NULL;
				is_move = true;
			}
		}
	add_block();
	can_move();
	add_grade();
}

void HelloWorld::move_right() {
	auto visib = Director::getInstance()->getWinSize();
	for (int j = 0; j < 4; j++)//第一列不动
		 for (int i = 2; i >= 0; i--){
			if (block[i][j] == NULL) {// 自己为空则跳过
				continue;
			}
			int ii = i;
			for (; ii < 3; ii++) {
				if (block[ii + 1][j] == NULL) {
					block[ii][j]->runAction(MoveBy::create(0.2f, Vec2(visib.width / 4, 0)));
					block[ii + 1][j] = block[ii][j];
					block[ii][j] = NULL;
					is_move = true;
				}
				else
					break;
			}
			if (ii == 3) {
				continue;
			}
			//否则执行判断相等，移动，合并
			int a = block[ii][j]->get_number();
			int b = block[ii + 1][j]->get_number();
			if (a == b) {
				block[ii + 1][j]->set_number(a * 2);
				auto seq = Sequence::create(MoveBy::create(0.2f, Vec2(visib.width / 4, 0)), RemoveSelf::create(true), NULL);
				block[ii][j]->runAction(seq);
				block[ii][j] = NULL;
				is_move = true;
			}
		}
	add_block();
	can_move();
	add_grade();
}

void HelloWorld::move_or_not(float x, float y) {
	if (fabs(x) <= 50 && fabs(y) <= 50) {//如果x轴y轴移动量不超过50像素，则不移动
		return;
	}
	if (fabs(x) > fabs(y))//如果x轴的偏移量大于y轴，那么横向移动
	{
		if (x > 0) {
			move_right();
		}
		else {
			move_left();
		}
	} else//y轴偏移量大于等于x轴的，则垂直移动
	{
		if (y > 0) {
			move_up();
		}
		else {
			move_down();
		}
	}
}
