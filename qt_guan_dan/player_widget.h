#ifndef PLAYER_WIDGET_H_
#define PLAYER_WIDGET_H_

#include"qwidget.h"
#include"sprite.h"
#include "qlabel.h"

class Player_widget :public QWidget
{
	Q_OBJECT
public:
	// @para id:玩家编号
	Player_widget(int id, QWidget* parent = nullptr);
protected:
	void closeEvent(QCloseEvent* event);
private:
	//玩家编号
	int id_;
	//group等级的label
	QLabel* lb_rank_self;
	QLabel* lb_rank_rival;
	//玩家头像位置坐标
	const int SPR_PLAYER_X[4] = {20,860,390,20};
	const int SPR_PLAYER_Y[4] = {370,130,15,130};
signals:
	void player_close();
};
#endif
