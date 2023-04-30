#include"player_widget.h"
#include "qstring.h"
#include "qpalette.h"
#include "qcolor.h"
#include "status.h"
#include "sprite.h"

Player_widget::Player_widget(int id, QWidget* parent) :QWidget(parent),id_(id)
{
	show();
	/*****窗口基本设置****/
	//设置标题
	setWindowTitle(QString("玩家") + QString::number(id_ + 1));
	//背景色深灰色
	setPalette(QColor(35, 35, 35));
	//设置窗口大小
	setGeometry((id_ / 2) * 200, 30 + (id_ % 2) * 200, SCREEN_W, SCREEN_H);
	setFixedSize(SCREEN_W, SCREEN_H);
	/*****基本UI布局设置****/
	//显示级牌底边
	QString rsc_t = QString("img/label/rank_bg_group") + QString::number(id_ % 2)+QString(".png");
	new Sprite(20, 20, rsc_t, this, Sprite::Height, 20);

	//显示玩家布局

	//显示按钮控件

}
