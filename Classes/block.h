#ifndef __BLOCK_SCENE_H__
#define __BLOCK_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class Block : public Sprite
{
public:
	Block();
	~Block();

	static Block* createBlock();
	virtual bool init();
	void onExit();
	void initBlock();
	int get_number();
	void set_number(int a);
	void font_size();//根据字的多少这是字的大小
	int number;//显示在方块上的数字
	LayerColor *bgcolor;
	Label *label;
	static Color3B color[15];
	CREATE_FUNC(Block);
};


#endif // __HELLOWORLD_SCENE_H__
