#ifndef GUAN_DAN_H_
#define GUAN_DAN_H_

#include <QtWidgets/QWidget>
#include "ui_guan_dan.h"
#include "hand.h"

class guan_dan : public QWidget
{
	Q_OBJECT

public:
	guan_dan(QWidget* parent = nullptr);

public slots:
	void on_start_game();
	void on_card_played(const std::vector<Card>& cards, int player_id);
	void on_passed();

signals:
	void sig_update_player_widget();
	void sig_game_over();
	void sig_new_round();
	void sig_switch_turn();
	
private:
	Ui::guan_danClass ui;
	// [i]: 玩家 i 的窗口
	PlayerWidget* player_widgets[4];

	//@brief 轮转到下一circle
	//@para is_next: 如为false，则按当前turn进行，否则轮转到下一个turn
	void switch_turn(bool is_next = true);

	//@brief 进入下一个round，处理进贡、还贡
	void new_round();
};

#endif //GUANDAN_H_