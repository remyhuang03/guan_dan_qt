#ifndef GUAN_DAN_H_
#define GUAN_DAN_H_

#include <QtWidgets/QWidget>
#include "ui_guan_dan.h"
#include "player_widget.h"
#include "hand.h"

class guan_dan : public QWidget
{
	Q_OBJECT

public:
	guan_dan(QWidget* parent = nullptr);
	~guan_dan();
	friend PlayerWidget;

private slots:
	void start_game();

signals:
	void update_player_widget();
	void signal_switch_turn();

private:
	Ui::guan_danClass ui;
	// [i]: 玩家 i 的窗口
	PlayerWidget* player_widgets[4];
	//@brief 轮转到下一轮
	//@para is_next: 如为false，则按当前turn进行，否则轮转到下一个turn
	void switch_turn(bool is_next = true);

};

#endif //GUANDAN_H_