#include "block.h"
#include <cmath>

Color3B Block::color[] = {
	Color3B(255, 255, 255),Color3B(255, 247, 236),Color3B(255, 239, 217),
	Color3B(255, 231, 198),Color3B(255, 223, 179),Color3B(255, 215, 160),
	Color3B(255, 207, 141),Color3B(255, 199, 122),Color3B(255, 191, 103),
	Color3B(255, 183, 84),Color3B(255, 175, 65),Color3B(255, 167, 46),
	Color3B(255, 159, 27)
};

Block::Block() {

}
Block::~Block(){

}


Block* Block::createBlock()
{
	Block *block = new Block();
	if (block && block->init()) {
		block->autorelease();
		block->initBlock();
		return block;
	}
	CC_SAFE_DELETE(block);
	return NULL;
}

bool Block::init()
{
	if (!Sprite::init()) {
		return false;
	}
	return true;
}

void Block::onExit() {
	unscheduleUpdate();
	Sprite::onExit();
}

void Block::font_size() {
	if (number >= 0) {
		label->setSystemFontSize(80);
	}

	if (number >= 16) {
		label->setSystemFontSize(60);
	}

	if (number >= 128) {
		label->setSystemFontSize(55);
	}

	if (number >= 1024) {
		label->setSystemFontSize(50);
	}
}

void Block::initBlock()
{
	auto size = Director::getInstance()->getWinSize();
	struct timeval tv;
	long time = gettimeofday(&tv, NULL);
	long long num = tv.tv_sec * 1000 + tv.tv_usec / 1000;//获取毫秒级的时间，来判断奇偶，达成随机
	if (num & 1) {
		number = 2;
	}
	else {
		number = 4;
	}
	int mi = log(number);
	bgcolor = LayerColor::create(Color4B(255, 255, 255, 255), size.width / 4 - 10, size.height / 6 - 10);
	label = Label::create();
	char x[10];
	itoa(number, x, 10);
	label->setString(x);
	label->setColor(Color3B(0, 0, 0));
	font_size();
	bgcolor->addChild(label);
	bgcolor->setAnchorPoint(Vec2::ZERO);
	bgcolor->setPosition(Vec2::ZERO);
	auto bgsize = bgcolor->getContentSize();
	label->setPosition(bgsize.width/2, bgsize.height/2);//设置在背景中间
	this->addChild(bgcolor);
}

int Block::get_number()
{
	return number;
}

void Block::set_number(int a)
{
	number = a;
	label->setString(String::createWithFormat("%d", a)->getCString());
	font_size();
	int mi = log(number);
	bgcolor->setColor(color[mi-1]);
}
