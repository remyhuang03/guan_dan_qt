#ifndef PLAYER_WIDGET_H_
#define PLAYER_WIDGET_H_

#include "qwidget.h"
#include "sprite.h"
#include "qlabel.h"
#include "hand.h"
#include "card_btn.h"
#include "button.h"
class CardButton;
class PlayerWidget :public QWidget
{
	Q_OBJECT
public:
	PlayerWidget(Hand* hand);
	//@brief 牌队恢复默认排序
	void sort_card_heap();
	//@brief 根据heap重新显示牌堆
	void update_card_heap_show();

	void set_hand(Hand* hand) { hand_ = hand; }

	//@brief 根据 Hand 刷新所有页面卡牌
	void update_all();

public slots:
	void on_card_selected(CardButton* card_btn);
	void on_card_unselected(CardButton* card_btn);


protected:
	void closeEvent(QCloseEvent* event);

private:
	//group等级的label
	QLabel* lb_rank_self_;
	QLabel* lb_rank_rival_;
	//玩家头像位置坐标
	const int SPR_PLAYER_X[4] = { 20,860,390,20 };
	const int SPR_PLAYER_Y[4] = { 370,130,15,130 };
	//对应玩家指针
	Hand* hand_;
	//该玩家排堆： vector<pair<是否为整理好的牌，该堆所有的牌>>
	std::vector<std::pair<bool, std::vector<Card>>> card_heaps_;
	//四个同花顺按钮
	Button* straight_flush_btns_[4];
	//已选中的所有卡牌按钮
	std::vector<CardButton*> selected_cards_;
	//当前所有可能的同花顺组合
	std::vector<std::vector<std::vector<Card>>> straight_flush_comb_;
signals:
	void player_close();
	void delete_all_card_bottons();
	//强制取消选择所有卡牌
	void compulsory_unselect_all_cards();
};
#endif
