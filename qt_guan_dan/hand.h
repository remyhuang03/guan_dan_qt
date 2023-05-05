#ifndef HAND_H_
#define HAND_H_

#include<vector>
#include<map>
#include"card.h"
#include"status.h"
//#include"player_widget.h"

class PlayerWidget;
// 单个用户类
class Hand
{
public:
	friend class PlayerWidget;

	Hand(int id);

	void set_cards(const std::vector<Card>& cards);

	//@brief 检查牌组合合法性，返回牌型和相对大小（不检查逢人配）
	//@para cards: 待检查的牌组合
	//@ret pair<int, int>: 牌类型（-1 为 invalid), 组合的相对大小
	std::pair<int, int> certain_comb_info(const std::vector<Card>& cards)const;

	//@brief 返回当前游戏状态下已选牌的所有有效牌型（包含判别不合法和逢人配）
	//@para cards: 用户所选择的牌
	//@ret vector<vector<Card>>: 所有有效的具体牌型
	std::vector<std::vector<Card>> all_valid_comb(const std::vector<Card>& cards);

	//@brief 执行贡牌和还贡
	//@para player_list: 玩家列表 chosen_cards 待上贡/还贡献的牌
	//@ret vector<vector<Card>>: 所有有效的具体牌型
	friend void contribute(std::vector <Hand>& player_list, const std::vector <Card>& chosen_cards);
	friend void return_card(std::vector <Hand>& player_list, const std::vector <Card>& chosen_cards);

	//@brief 判断是否满足上贡条件
	//@para card: 用户所选择的牌
	bool could_contribute(const Card& chosen_card);

	//@brief  返回玩家手牌
	std::vector<Card> get_cards() const;

	//@brief 为该用户增加一张牌
	void push_card(const Card& card) { cards_.push_back(card); }

	//@brief 为该用户删除一张牌
	void pop_card(const Card& card);

	//@brief 为该用户删除多张牌
	void pop_card(const std::vector<Card>& cards);

	int get_group()const { return id_ % 2; }


	//@brief 获取牌点的真实大小
	//@para point: 牌点编号
	//@ret int: 相对大小
	int get_real_rank(int point)const { return cards_round_rank.at(point); }

	//@brief 绑定窗口实现
	void set_widget(PlayerWidget* widget) { widget_ = widget; }

	//@brief 返回所有同花顺的可行牌型
	//@ret:std::vector<std::vector<std::vector<Card>>>，[i][j]第i种花色无重复的第j种同花顺组合
	std::vector<std::vector<std::vector<Card>>> all_straight_flush_combinations()const;

private:
	//玩家编号：0-3(02为一方，13为另一方)
	int id_;
	//所持有卡牌
	std::vector<Card> cards_;
	//该用户的窗口实现
	PlayerWidget* widget_;
};

#endif //HAND_H_

