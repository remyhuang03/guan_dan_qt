#include "guan_dan.h"
#include <qpalette.h>
#include <qpushbutton.h>
#include <qicon.h>
#include "sprite.h"
#include "button.h"
guan_dan::guan_dan(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle("掼蛋");
	//16：9 分辨率显示
	setFixedSize(SCREEN_W, SCREEN_H);
	//背景色灰色
	setPalette(QColor(35, 35, 35));
	Sprite* btn_background = new Sprite(0, 0, "img/bg/home.png", this, Sprite::Width, SCREEN_W);
	Sprite* btn_start_game = new Button(360, 380, "img/btn/start_game.png", this, Button::Height, 80);
	//connect(btn_start_game, &Button::click_emit, this, &start_game);
}



guan_dan::~guan_dan()
{}
