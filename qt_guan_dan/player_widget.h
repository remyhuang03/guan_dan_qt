#ifndef PLAYER_WIDGET_H_
#define PLAYER_WIDGET_H_

#include"qwidget.h"
#include"sprite.h"

class Player_widget :public QWidget
{
	Q_OBJECT
public:
	// @para id:玩家编号
	Player_widget(int id, QWidget* parent = nullptr);
private:
	//玩家编号
	int id_;
};
#endif
