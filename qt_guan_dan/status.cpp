#include <algorithm>
#include <random>
#include <ctime>
#include <qpainter.h>
#include "status.h"
#include "hand.h"
#include "record.h"
#include "card.h"

//全局数据初始定义

int round_cnt = 0;
int round_level_card = 2;
int turn = 0;
int circle_type = 0;
int circle_point = 0;
int circle_leader = 0;
int contribute_count;
int card_played_process_count = 0;
int leading_flag = -1;
int round_rank[4];
int rank_list[4];
int contribute_order[2];
bool leading_visited[4];
int group_level[2] = { 2,2 };
int group_fial_pass_a[2] = { 0,0 };
bool is_game_over = false;
Card contributed_card[2];
Card retributed_card[2];
Hand* players[4];
Record game_record;
std::map<int, int> cards_round_level;

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
	//赢家组
	int winner_group = first_id % 2;

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
	int level_incre = 4 - partner_rank;

	//打过A，游戏结束
	if (group_level[winner_group] == 1)
	{
		is_game_over = true;
	}

	//对手方打A失败
	if (group_level[!winner_group] == 1)
	{
		group_fial_pass_a[!winner_group]++;
		//三次未过，回退至2
		if (group_fial_pass_a[!winner_group] == 3)
		{
			group_level[!winner_group] = 2;
			group_fial_pass_a[!winner_group] = 0;
		}
	}
	int new_level_t = group_level[winner_group] + level_incre;
	if (group_level[winner_group] == 1)
	{
		new_level_t = 1;
	}
	new_level_t = new_level_t > 13 ? 1 : new_level_t;
	group_level[winner_group] = new_level_t;

	//更新级牌
	round_level_card = new_level_t;

	//更新牌点大小映射
	update_cards_round_level();
}

void update_cards_round_level()
{
	int level_t = 0;
	//2-K
	for (int i = 2; i <= 13; i++)
	{
		if (i != round_level_card)
			cards_round_level[i] = level_t++;
	}
	//A(是不是级牌排名一样)
	cards_round_level[1] = level_t++;
	//级牌（前面A已经排过）
	if (round_level_card != 1)
		cards_round_level[round_level_card] = level_t++;
	//小王
	cards_round_level[14] = level_t++;
	//大王
	cards_round_level[15] = level_t++;

	qDebug() << cards_round_level[2];
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
	group_level[0] = group_level[1] = 2;//debug
	group_fial_pass_a[0] = group_fial_pass_a[1] = 0;
	round_level_card = 2;

	//第一次随机找个出牌人
	std::srand(std::time(nullptr));
	turn = std::rand() % 4;
	card_played_process_count = 0;
	circle_type = 0;
	circle_point = 0;
	circle_leader = turn;

	//没有玩家完成比赛，玩家排名记为-1
	std::for_each(std::begin(round_rank), std::end(round_rank), [](int& a) {a = -1; });
	update_cards_round_level();
	reset_leading();
}

QPixmap get_combination_pixmap(const std::vector<Card>& cards, double ratio)
{
	bool is_wild_card = false;
	auto cards_cpy = cards;
	sort(cards_cpy.begin(), cards_cpy.end());

	//获取素材大小
	auto size = QPixmap("img/card/1_0.png").size();

	//卡牌个数
	auto cnt = cards_cpy.size();
	//指定合成后 QPixmap 的大小
	int pixmap_width = size.width() + (cnt - 1) * size.width() * ratio;
	QPixmap ret(pixmap_width, size.height());
	QPainter painter(&ret);

	//依次叠加卡牌元素
	for (int i = 0; i < cnt; i++)
	{
		const Card& card = cards_cpy[i];
		//该卡牌素材叠加
		auto card_pixmap = QPixmap(
			QString("img/card/%1_%2.png").arg(card.get_point()).arg(card.get_suit()));
		painter.drawPixmap(i * size.width() * ratio, 0, card_pixmap);
		//发现逢人配
		if (card.is_wild_card())
		{
			is_wild_card = true;
		}
	}
	if (is_wild_card)
	{
		//逢人配加星显示
		painter.drawPixmap(pixmap_width - 60, 0, QPixmap("img/label/star.png"));
	}

	return ret;
}

void reset_round_rank()
{
	//rank_list复位
	for (int i = 0; i < 4; i++) { round_rank[i] = -1; }
}

void reset_leading()
{
	leading_flag = -1;
	memset(leading_visited, 0, sizeof(leading_visited));
}
