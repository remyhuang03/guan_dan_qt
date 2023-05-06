#ifndef PLAYER_WIDGET_H_
#define PLAYER_WIDGET_H_

#include "qwidget.h"
#include "sprite.h"
#include "qlabel.h"
#include "hand.h"
#include "card_btn.h"
#include "button.h"
#include "StatusLabel.h"

class CardButton;
class SfButton;
class guan_dan;

class PlayerWidget :public QWidget
{
	Q_OBJECT
public:
	PlayerWidget(Hand* hand);

	//@brief 牌队恢复默认排序
	//@para is_partial:是否只对未手工整理的牌堆部分进行排序
	void sort_card_heap(bool is_partial = false);

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
	void on_card_selected(CardButton* card_btn, bool is_compulsory = true);
	void on_card_unselected(CardButton* card_btn);
	void on_arrange_clicked(int mode);
	void on_turn_switched();
	void on_play_card();
	void on_card_played(const std::vector<Card>& cards, int player_id);
	void on_passed(int player_id);
	void on_pass_clicked();

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
	//已出牌展示
	Sprite* spr_played_cards_[4];
	//玩家游戏状态标签
	StatusLabel* lb_status_[4];
	

	/***** 常量坐标数据 ******/
	//玩家头像位置坐标
	const int SPR_PLAYER_X[4] = { 20,860,390,20 };
	const int SPR_PLAYER_Y[4] = { 370,110,15,110 };
	//玩家已出牌显示位置
	const int PLAYED_CARD_X[4] = { 15,780,480,120 };
	const int PLAYED_CARD_Y[4] = { 300,140,30,140 };


	/***** 游戏数据 ******/
	//对应玩家id
	int id_;
	//对应玩家指针
	Hand* hand_;
	//该玩家排堆： vector<pair<是否为整理好的牌，该堆所有的牌>>
	std::vector<std::pair<bool, std::vector<Card>>> card_heaps_;

	//当前所有可能的同花顺组合
	std::vector<std::vector<std::vector<Card>>> straight_flush_comb_;

	//所选牌对应的所有可能的出牌组合
	std::vector<std::vector<Card>> all_combs_;


	/***** 功能函数 ******/
	//@brief 更新出牌按钮状态
	void update_play_btn();

	//@brief 卡牌按钮转换为卡牌
	std::vector<Card> btns_to_cards(const std::vector<CardButton*>& card_btns);

	//@brief 删除所有已出牌显示UI
	//@para player_id: 对应玩家id
	void delete_played_cards_ui(int player_id);

	//@brief 更新已出牌显示UI
	//@para  cards: 需要渲染的卡牌
	//       player_id: 对应玩家id
	void update_played_cards_ui(const std::vector<Card>& cards, int player_id);
	//@brief 更新已出牌显示UI（显示为“不出”）
	//       player_id: 对应玩家id
	void update_played_cards_ui(int player_id);

signals:
	//@brief 强制取消选择所有卡牌
	void compulsory_unselect_all_cards();
	//@brief 当前玩家窗口关闭
	void player_close();
	//@brief 需要删除所有卡牌显示
	void delete_all_card_bottons();
	//@brief 强制选中卡牌
	void compulsory_select(CardButton* btn);
	//@brief 打出卡牌
	//@para 
	// cards: 打出的卡牌
	// player_id: 打出卡牌的玩家id
	void card_played(std::vector<Card> cards, int player_id);
	//@brief 跳过该玩家出牌
	void sig_pass(int player_id);

};
#endif
