#ifndef GUAN_DAN_H_
#define GUAN_DAN_H_

#include <QtWidgets/QWidget>
#include "ui_guan_dan.h"
#include "player_widget.h"
#include "hand.h"
#define SCREEN_W 960
#define SCREEN_H 540

class guan_dan : public QWidget
{
	Q_OBJECT

public:
	guan_dan(QWidget* parent = nullptr);
	~guan_dan();

private slots:
	void start_game();

signals:
	void update_player_widget();

private:
	Ui::guan_danClass ui;
	// [i]: 玩家 i 的窗口
	Player_widget* player_widgets[4];
	// [i]: 玩家 i 对象指针
	Hand* players[4];
};

#endif //GUANDAN_H_