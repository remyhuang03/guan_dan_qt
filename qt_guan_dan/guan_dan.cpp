#include "guan_dan.h"
#include <qpalette.h>
#include <qpushbutton.h>
#include <qicon.h>
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
	//设置首页图片
	/*auto lb_background = new QLabel(this);
	auto pm_background = QPixmap("img/bg/home.png");
	pm_background = pm_background.scaled(SCREEN_W, SCREEN_H, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	lb_background->setScaledContents(true);
	lb_background->setPixmap(pm_background);*/
	Sprite* btn_background = new Sprite(0, 0, "img/bg/home.png", this,Sprite::Width,SCREEN_W);
	//setStyleSheet("width:200px;height:auto;background-image:url(img/bg/home.png);");
	Sprite* btn_start_game = new Sprite(360, 380, "img/btn/start_game.png", this, Sprite::Height, 80);
	/*QPushButton* btn_start_game = new QPushButton(this);
	btn_start_game->setStyleSheet("QPushButton{border:none;}");
	auto bitmap_start_game = QPixmap("img/btn/start_game.png");
	btn_start_game->setIcon(bitmap_start_game);
	btn_start_game->setIconSize(bitmap_start_game.size());
	btn_start_game->setFlat(true);*/
}



guan_dan::~guan_dan()
{}
