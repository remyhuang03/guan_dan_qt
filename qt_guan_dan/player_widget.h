#ifndef PLAYER_WIDGET_H_
#define PLAYER_WIDGET_H_

#include "qwidget.h"
#include "sprite.h"
#include "qlabel.h"
#include "hand.h"
#include "card_btn.h"
#include "button.h"

class CardButton;
class SfButton;
class PlayerWidget :public QWidget
{
	Q_OBJECT
public:
	PlayerWidget(Hand* hand);

	//@brief 牌队恢复默认排序
	//@para is_partial:是否只对未手工整理的牌堆部分进行排序
	void sort_card_heap(bool is_partial=false);

	//@brief 根据heap重新显示牌堆
	void update_card_heap_show();

	void set_hand(Hand* hand) { hand_ = hand; }

	//@brief 根据 Hand 刷新所有页面卡牌
	void update_all();

	//@brief 获取指定花色的全部同花顺组合
	std::vector<std::vector<Card>> get_sf_combination(int suite) { return straight_flush_comb_[suite]; }

	void emit_unselect_all_cards();
	void emit_select(std::vector<Card> cards);

public slots:
	void on_card_selected(CardButton* card_btn);
	void on_card_unselected(CardButton* card_btn);
	void on_arrange_clicked(int mode);
	void on_turn_switched();


protected:
	void closeEvent(QCloseEvent* event);

private:
	/***** UI控件 *****/
	//group等级的label
	QLabel* lb_rank_self_;
	QLabel* lb_rank_rival_;
	//整理卡牌按钮
	Button* btn_arrange_;
	//四个同花顺按钮
	SfButton* straight_flush_btns_[4];
	//已选中的所有卡牌按钮
	std::vector<CardButton*> selected_cards_;
	//出牌按钮
	Button* btn_play_;
	//不出牌按钮
	Button* btn_pass_;

	/***** ******/
	//玩家头像位置坐标
	const int SPR_PLAYER_X[4] = { 20,860,390,20 };
	const int SPR_PLAYER_Y[4] = { 370,130,15,130 };
	//对应玩家指针
	Hand* hand_;
	//该玩家排堆： vector<pair<是否为整理好的牌，该堆所有的牌>>
	std::vector<std::pair<bool, std::vector<Card>>> card_heaps_;

	//当前所有可能的同花顺组合
	std::vector<std::vector<std::vector<Card>>> straight_flush_comb_;

	//更新出牌按钮状态
	void update_play_btn();

	//卡牌按钮转换为卡牌
	std::vector<Card> btns_to_cards(const std::vector<CardButton*>& card_btns);

	//所选牌对应的所有可能的出牌组合
	std::vector<std::vector<Card>> all_combs_;

signals:
	//强制取消选择所有卡牌
	void compulsory_unselect_all_cards();
	void player_close();
	void delete_all_card_bottons();
	void compulsory_select(CardButton* btn);

};
#endif
