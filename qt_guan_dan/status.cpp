#include"status.h"
#include"hand.h"
#include<algorithm>
#include<random>

bool is_game_over = false;
int round_cnt = 0;
int group_rank[2] = { 2,2 };
int group_fial_pass_a[2] = { 0,0 };
int round_rank_card = 2;
int turn = 0;
int circle_type = 0;
int circle_point = 0;
int circle_leader = 0;
int contribute_order[4];
int contribute_count;
int round_rank[4];
int rank_list[4];
std::map<int, int> cards_round_rank;

Hand* players[4];

void update_round_rank()
{

	//获取排名情况 	
	for (int i = 0; i < 4; i++)
	{
		rank_list[round_rank[i]] = i;
	}

	//上游的id
	int first_id = rank_list[0];

	int winner_group = first_id / 2;

	//上游对家的排名
	int partner_rank;
	switch (first_id)
	{
	case 0:
		partner_rank = round_rank[2];
		break;
	case 1:
		partner_rank = round_rank[3];
		break;
	case 2:
		partner_rank = round_rank[0];
		break;
	case 3:
		partner_rank = round_rank[1];
		break;
	default:
		break;
	}

	//可升级的级数
	int rank_incre = 4 - partner_rank;

	//打过A，游戏结束
	if (group_rank[winner_group] == 1)
	{
		is_game_over = true;
	}

	//对手方打A失败
	if (group_rank[!winner_group] == 1)
	{
		group_fial_pass_a[!winner_group]++;
		//三次未过，回退至2
		if (group_fial_pass_a[!winner_group] == 3)
		{
			group_rank[!winner_group] = 2;
			group_fial_pass_a[!winner_group] = 0;
		}
	}
	int new_rank_t = group_rank[winner_group] + rank_incre;
	new_rank_t = new_rank_t > 13 ? 13 : new_rank_t;
	group_rank[winner_group] = new_rank_t;

	//更新级牌
	int round_rank_card = new_rank_t;

	//更新牌点大小映射
	update_cards_round_rank();

	return;
}

void update_cards_round_rank()
{
	int rank_t = 0;
	//2-K
	for (int i = 2; i <= 13; i++)
	{
		if (i != round_rank_card)
			cards_round_rank[i] = rank_t++;
	}
	//A(是不是级牌排名一样)
	cards_round_rank[1] = rank_t++;
	//级牌（前面A已经排过）
	if (round_rank_card != 1)
		cards_round_rank[round_rank_card] = rank_t++;
	//小王
	cards_round_rank[14] = rank_t++;
	//大王
	cards_round_rank[15] = rank_t++;
}

std::vector<Card> shuffled_all_cards()
{
	std::vector<Card> ret;
	//普通牌
	for (int i = 2; i <= 13; i++)
		for (int j = 0; j <= 3; j++)
		{
			ret.push_back(Card(i, j));
			ret.push_back(Card(i, j));
		}
	//大小王
	ret.push_back(Card(14, -1));
	ret.push_back(Card(14, -1));
	ret.push_back(Card(15, -1));
	ret.push_back(Card(15, -1));
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(ret.begin(), ret.end(), g);

	return ret;
}
