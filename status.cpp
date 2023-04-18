#include"status.h"

bool is_game_over = false;

void update_round_rank()
{
	//找到上游的id
	int first_id;
	//获取排名情况 	
	for(int i = 0; i < 4; i++)
	{
		rank_list[round_rank[i]] = i;
	}
	for (int i = 0; i < 4; i++)
	{
		if (round_rank[i] == 0)
		{
			first_id = i;
			break;
		}
	}
	//上游所在组
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
	if (group_rank[~winner_group] == 1)
	{
		group_fial_pass_a[~winner_group]++;
		//三次未过，回退至2
		if (group_fial_pass_a[~winner_group] == 3)
		{
			group_rank[~winner_group] = 2;
			group_fial_pass_a[~winner_group] = 0;
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

void init_game_status()
{
	//初始化全局状态
	is_game_over = false;
	round_cnt = 0;
	group_rank[0] = 2;
	group_rank[1] = 2;
	group_fial_pass_a[0] = 0;
	group_fial_pass_a[1] = 0;
	//初始化当前一轮的状态
	round_rank_card = 2;
	update_cards_round_rank();
	//初始化当前一圈的状态
	turn = 0;
	circle_type = 0;
	circle_point = 0;
	circle_leader = 0;
	return;
}
