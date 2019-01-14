#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "block.h"
#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
	bool random_time();
	bool onTouchBegan(Touch *touch, Event *event);
	bool onTouchEnd(Touch *touch, Event *event);
	void move_or_not(float x, float y);
	void move_up();
	void move_down();
	void move_left();
	void move_right();
	void add_grade();
	void add_block();
	void restart_scene();
	void resaart_button();
	void can_move();

	float star_x, star_y, end_x, end_y;
	Block* block[5][5];
	int sum;
	Label *grade;
	bool is_move;
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
