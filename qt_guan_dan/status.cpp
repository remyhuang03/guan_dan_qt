#include <algorithm>
#include <random>
#include <ctime>
#include <qpainter.h>
#include "status.h"
#include "hand.h"
#include "record.h"
#include "card.h"


//游戏结束标志
bool is_game_over = false;
int round_cnt = 0;
int group_rank[2] = { 2,2 };
int group_fial_pass_a[2] = { 0,0 };
int round_rank_card = 2;
int turn = 0;
// 当前出牌的牌型（-4,-3,-2,-1：进贡1，进贡2，还贡1，还贡2   >=0:正常牌型）
int circle_type = 0;
int circle_point = 0;
// 本圈出牌的领圈人, 可以不受牌型限制出牌
int circle_leader = 0;
int contribute_order[2];
int contribute_count;
//上一轮游戏中玩家 i 的排名(-1：该玩家正在游戏中  0 - 3：0为上游)
int round_rank[4];
int rank_list[4];
int card_played_process_count = 0;
std::map<int, int> cards_round_rank;
Card contributed_card[2];
Card retributed_card[2];

Hand* players[4];

Record game_record;

void round_over()
{
	//轮数加一
	round_cnt++;

	//获取排名情况 	
	for (int i = 0; i < 4; i++)
	{
		rank_list[round_rank[i]] = i;
	}

	//游戏增加到牌局记录中
	game_record.push_record();

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
	for (int i = 1; i <= 13; i++)
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
	//随机打乱牌序
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(ret.begin(), ret.end(), g);

	return ret;
}

void init_game_data()
{
	is_game_over = false;
	round_cnt = 0;
	group_rank[0] = group_rank[1] = 2;
	group_fial_pass_a[0] = group_fial_pass_a[1] = 0;
	round_rank_card = 2;

	//第一次随机找个出牌人
	std::srand(std::time(nullptr));
	turn = std::rand() % 4;
	card_played_process_count = 0;
	circle_type = 0;
	circle_point = 0;
	circle_leader = turn;
	//没有玩家完成比赛，玩家排名记为-1
	std::for_each(std::begin(round_rank), std::end(round_rank), [](int& a) {a = -1; });
	update_cards_round_rank();
}

QPixmap get_combination_pixmap(const std::vector<Card>& cards, double ratio)
{
	auto cards_cpy = cards;
	sort(cards_cpy.begin(), cards_cpy.end());

	//获取素材大小
	auto size = QPixmap("img/card/1_0.png").size();
	//卡牌个数
	auto cnt = cards_cpy.size();
	// 指定合成后 QPixmap 的大小
	QPixmap ret(size.width() + (cnt - 1) * size.width() * ratio, size.height());
	QPainter painter(&ret);
	// 依次叠加卡牌元素
	for (int i = 0; i < cnt; i++)
	{
		const Card& card = cards_cpy[i];
		auto card_pixmap = QPixmap(QString("img/card/%1_%2.png").arg(card.get_point()).arg(card.get_suit()));
		painter.drawPixmap(i * size.width() * ratio, 0, card_pixmap);
	}
	return ret;
}

void reset_round_rank()
{
	//rank_list复位
	for (int i = 0; i < 4; i++) { round_rank[i] = -1; }
}
