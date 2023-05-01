#include "hand.h"
#include "status.h"
#include<algorithm>
#include<vector>
#include<string>

Hand::Hand(int id) :id_(id), fail_at_a_cnt_(0)
{}

std::pair<int, int> Hand::check(const std::vector<Card>& cards)const
{
	//统计各类型牌点的数量
	std::map<int, int> point_cnt;
	for (auto& card : cards)
	{
		point_cnt[card.get_point()]++;
	}
	//计算最大牌点值
	int max_rank = -1;
	for (auto& card : cards)
	{
		max_rank = max(max_rank, get_real_rank(card.get_point()));
	}

	//计算牌型特征, 检查牌点是否连续
	std::string card_trait;
	bool is_consecutive = true;
	std::vector<int>points;
	for (auto& point : point_cnt)
	{
		points.push_back(point.first);
		card_trait += char(point.second + '0');
	}
	sort(points.begin(), points.end());
	//存在大小王，直接否定连续性
	if (find(points.begin(), points.end(), 14) != points.end() &&
		find(points.begin(), points.end(), 15) != points.end())
	{
		is_consecutive = false;
	}
	//判断A的地位
	if (points.size() > 1 && points[0] == 1 && points[1] != 2)
	{
		//将A作为最大者重排
		points[0] = 14;
		sort(points.begin(), points.end());
	}
	//牌点不连续
	if (points.back() - points.front() != points.size() - 1)
	{
		is_consecutive = false;
	}

	//连续牌点的大小（以连续牌点中最小者减1规范到从0开始算大小）
	int consecutive_rank = max_rank - 1;

	//需返回的牌型和相对大小
	int ret_type = -1, ret_rank = -1;
	//待检查组合的牌总数
	int cards_cnt = cards.size();

	//@brief 判断炸弹类型并按倍率设置返回值
	auto bomb = [&ret_type, &ret_rank, max_rank, point_cnt](int rate) {
		if (point_cnt.size() == 1)
		{
			ret_type = 8;
			ret_rank = max_rank + 16 * rate;
			return true;
		}
		return false;
	};

	switch (cards_cnt)
	{
	case 1:
		// 单张-1
		ret_type = 1;
		ret_rank = max_rank;
		break;
	case 2:
		// 对子-2
		if (point_cnt.size() == 1)
		{
			ret_type = 2;
			ret_rank = max_rank;
		}
		break;
	case 3:
		// 三张-3
		if (point_cnt.size() == 1)
		{
			ret_type = 3;
			ret_rank = max_rank;
		}
		break;
	case 4:
		// 4个的炸弹-8(倍率0)
		if (bomb(0))
			break;
		// 天王炸-8(倍率6)
		if (point_cnt.size() == 2)
		{
			//小王和大王均存在且个数均为2
			if (point_cnt.find(13) != point_cnt.end() && point_cnt[13] == 2 &&
				point_cnt.find(14) != point_cnt.end() && point_cnt[14] == 2)
			{
				ret_type = 8;
				ret_rank = 16 * 6;
				break;
			}
		}
		break;

	case 5:
		//5个的炸弹-8(倍率1)
		if (bomb(1))
			break;
		//三带二-4
		if (point_cnt.size() == 2)
		{
			auto elem1 = point_cnt.begin();
			auto elem2 = point_cnt.rbegin();
			if (elem1->second == 3)
			{
				ret_type = 4;
				ret_rank = get_real_rank(elem1->first);
				break;
			}
			else if (elem2->second == 3)
			{
				ret_type = 4;
				ret_rank = get_real_rank(elem2->first);
				break;
			}
		}
		//同花顺-8(倍率2) || //顺子-5
		// 前提：五张不同的牌
		if (point_cnt.size() == 5)
		{
			//判断5张牌是否为同花
			bool is_same_suit = true;
			for (int i = 1; i < cards_cnt; i++)
			{
				if (cards[i].get_suit() != cards[i - 1].get_suit())
				{
					is_same_suit = false;
					break;
				}
			}
			if (is_consecutive)
				if (is_same_suit)
				{
					ret_type = 8;
					ret_rank = consecutive_rank + 16 * 2;
					break;
				}
				else
				{
					ret_type = 5;
					ret_rank = consecutive_rank;
					break;
				}
		}
		break;
	case 6:
		//6个的炸弹-8（倍率3）
		if (bomb(3))
			break;
		//三连对-6
		if (is_consecutive && card_trait == "222")
		{
			ret_type = 6;
			ret_rank = max_rank;
		}
		//二连三-7
		if (is_consecutive && card_trait == "33")
		{
			ret_type = 6;
			ret_rank = max_rank;
		}
		break;
	case 7:
		//7个的炸弹-8（倍率4）
		bomb(4);
		break;
	case 8:
		//8个的炸弹-8（倍率5）
		bomb(5);
		break;
	case 9:
		//9个的炸弹-9（倍率6）
		bomb(6);
		break;
	case 10:
		//10个的炸弹-10（倍率7）
		bomb(7);
		break;

	default:
		// 不符合出牌要求
		break;
	}
	return std::pair<int, int>(ret_type, ret_rank);
}


std::vector<std::vector<Card>> Hand::play_cards(const std::vector<Card>& cards)
{
	std::vector<std::vector<Card>>ret;
	std::vector<Card> cards_tmp = cards;
	//检查是否有逢人配
	int free_card_cnt = 0;
	for (auto i = cards.begin(); i != cards.end(); i++)
	{
		//为级牌并且为红桃
		if (i->get_point() == group_rank[get_group()] && i->get_suit() == 1)
		{
			//删除级牌
			cards_tmp.erase(i);
			free_card_cnt++;
		}
	}
	if (free_card_cnt == 0)
	{
		//牌型不是逢人配且合法	
		if (check(cards).first != -1)
		{
			ret.push_back(cards);
		}
	}
	else if (free_card_cnt == 1)
	{
		//枚举所有可能的牌型
		for (int i = 1; i <= 13; i++)
			for (int j = 0; j <= 3; j++)
			{
				cards_tmp.push_back(Card(i, j));
				if (check(cards_tmp).first)
				{
					ret.push_back(cards_tmp);
				}
				cards_tmp.pop_back();
			}
	}
	else
	{
		//枚举所有可能的牌型
		for (int i = 1; i <= 13; i++)
			for (int j = 0; j < 3; j++)
				for (int p = 1; p <= 13; p++)
					for (int q = 0; q < 3; q++)
					{
						cards_tmp.push_back(Card(i, j));
						cards_tmp.push_back(Card(p, q));
						if (check(cards_tmp).first)
						{
							ret.push_back(cards_tmp);
						}
						cards_tmp.pop_back();
						cards_tmp.pop_back();
					}
	}
	return ret;
}

std::vector<Card> Hand::get_cards() const
{
	return cards_;
}

// 删除手牌
void Hand::pop_card(const Card& card)
{
	for (auto it = cards_.begin(); it != cards_.end(); it++)
	{
		// 找到了这张牌
		if (*it == card)
		{
			// 删除它
			cards_.erase(it);
			return;
		}
	}
}

// 判断该牌是否满足上贡的要求
bool Hand::could_contribute(const Card& chosen_card)
{
	auto MAX_card = [&](const Card& card1, const Card& card2)->Card
	{
		if (get_real_rank(card1.get_point()) > get_real_rank(card2.get_point()))
			return card1;
		return card2;
	};
	// 设定出红桃级牌 和 能取的最小牌
	Card rank_card{ round_rank_card,2 }, max_card(-1, -1);
	for (const auto& card : cards_)
	{
		// 红桃级牌可以选择不取
		if (card == rank_card) continue;
		max_card = MAX_card(max_card, card);
	}
	// 如果选择的牌的点数比能取的最小点数大
	if (get_real_rank(chosen_card.get_point()) >= get_real_rank(max_card.get_point()))
	{
		pop_card(chosen_card);
		return true;
	}
	return false;
}

// 贡牌
void contribute(std::vector <Hand>& player_list, const std::vector <Card> chosen_cards)
{
	// 需要上贡的人数
	contribute_count = 1;
	// 如果头游和二游是同一组的，那么双下游上贡
	if ((rank_list[0] & 1) == (rank_list[1] & 1))
	{
		contribute_count = 2;
	}
	// 如果是双下游的情况
	if (contribute_count == 2)
	{
		// 检查是否可以进行抗贡
		auto temp_cards = player_list[rank_list[2]].get_cards();
		int JOKER_count = std::count(temp_cards.begin(), temp_cards.end(), Card(15, -1));
		temp_cards = player_list[rank_list[3]].get_cards();
		JOKER_count += std::count(temp_cards.begin(), temp_cards.end(), Card(15, -1));
		// 抗贡
		if (JOKER_count == 2)
		{
			turn = rank_list[0];
			contribute_count = 0;
			return;
		}
		else //认命
		{
			// 准上贡的卡片
			std::vector <Card> contribute_cards;
			for (int i = 2; i < 4; i++)
			{
				// 如果满足上贡条件，那么加入可上贡卡片列表
				if (player_list[rank_list[i]].could_contribute(chosen_cards[i - 2]))
					contribute_cards.push_back(chosen_cards[i - 2]);
			}
			// 确定卡片上贡顺序
			int value_first = player_list[0].get_real_rank(contribute_cards[0].get_point());
			int value_second = player_list[0].get_real_rank(contribute_cards[1].get_point());
			if (value_first < value_second)
			{
				std::swap(contribute_cards[0], contribute_cards[1]);
				contribute_order[rank_list[0]] = rank_list[3];
				contribute_order[rank_list[1]] = rank_list[2];
			}
			else
			{
				contribute_order[rank_list[0]] = rank_list[2];
				contribute_order[rank_list[1]] = rank_list[3];
			}
			// 将卡片放入
			for (int i = 0; i < 2; i++)
			{
				player_list[rank_list[i]].push_card(contribute_cards[i]);
			}
		}
	}
	else
	{
		// 检查是否可以进行抗贡
		auto temp_cards = player_list[rank_list[3]].get_cards();
		int JOKER_count = std::count(temp_cards.begin(), temp_cards.end(), Card(15, -1));
		// 抗贡
		if (JOKER_count == 2)
		{
			turn = rank_list[0];
			contribute_count = 0;
			return;
		}
		else //认命
		{
			// 如果可以上供
			if (player_list[rank_list[3]].could_contribute(chosen_cards[0]))
			{
				player_list[rank_list[0]].push_card(chosen_cards[0]);
				contribute_count = 1;
				contribute_order[rank_list[0]] = rank_list[3];
			}
		}
	}
}

// 还贡
void return_card(std::vector <Hand>& player_list, std::vector <Card>& chosen_cards)
{
	for (int i = 0; i < contribute_count; i++)
	{
		player_list[contribute_order[rank_list[i]]].push_card(chosen_cards[i]);
		player_list[rank_list[i]].pop_card(chosen_cards[i]);
	}
}

void Hand::set_cards(const std::vector<Card>& cards)
{
	//设置当前手牌
	cards_ = cards;
	//更新界面上的手牌
	widget_->update_all();
}

