#include "player_widget.h"
#include "qstring.h"
#include "status.h"
#include "sprite.h"
#include "qpushbutton.h"
#include "qlabel.h"
#include "button.h"

Player_widget::Player_widget(int id, QWidget* parent) :QWidget(parent), id_(id)
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
	QString rsc_t = QString("img/label/rank_bg_group") + QString::number(id_ % 2) + QString(".png");
	new Sprite(15, 15, rsc_t, this, Sprite::Height, 65);

	auto set_label = [](QLabel* lb, int y) {
		lb->setStyleSheet("boder:none;");
		lb->setFont(QFont("微软雅黑", 12));
		lb->setGeometry(70, y, 30, 25);
		lb->show();
	};
	lb_rank_self = new QLabel("2", this);
	lb_rank_rival = new QLabel("2", this);
	set_label(lb_rank_self, 15);
	set_label(lb_rank_rival, 47);
	//显示玩家布局
	int id_t = id_;
	for (int i = 0; i < 4; i++)
	{
		int id_;
		rsc_t = QString("img/label/player") + QString::number(id_t) + QString(".png");
		new Sprite(SPR_PLAYER_X[i], SPR_PLAYER_Y[i], rsc_t, this, Sprite::Height, 120);
		if (++id_t == 4) { id_t = 0; }
	}
	//显示按钮控件
	new Button(850,10,"img/btn/record.png",this,Button::Height,40);

}

void Player_widget::closeEvent(QCloseEvent* event)
{
	emit player_close();
}
