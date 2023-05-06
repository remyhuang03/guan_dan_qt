#ifndef STATUS_H_
#define STATUS_H_
#include <map>
#include <vector>
#include "card.h"
#include "record.h"
#define SCREEN_W 960
#define SCREEN_H 540

class Hand;

/*****  状态管理函数 *****/

//@brief 全局初始化游戏状态
void init_game_data();

//@brief 轮游戏结束，更新本轮数据
//group_rank, round_rank_card, cards_round_rank, rank_list
void round_over();

//@brief 更新 cards_round_rank
void update_cards_round_rank();

//@brief 生成牌组Pixmap
//@para
//cards: 牌组
//ratio: 牌组间隔(1.0为满间隔)
QPixmap get_combination_pixmap(const std::vector<Card>&cards, double ratio = 0.5);

//@brief 开局发牌
//@return vector<Card> 打乱顺序的两幅扑克牌
std::vector<Card> shuffled_all_cards();


/***** 全局状态 *****/

// 游戏结束标志
extern bool is_game_over;
//总牌局
extern Record game_record;
// [i]: 上一轮游戏中玩家 i 的排名 (0-3,0为上游)
extern int round_rank[4];
// [i]：上一轮排名第 i 的玩家 (0-3)
extern int rank_list[4];
//  总游戏轮数
extern int round_cnt;
// [i]: Group i 的等级(按牌点数计1-13)
extern int group_rank[2];
// [i]: Group i 没有打过A的次数
extern int group_fial_pass_a[2];
// [i]: 玩家 i 对象指针
extern Hand* players[4];

/***** 当前一轮的状态 *****/

// 本轮级牌
extern int round_rank_card;
//牌point映射到相对大小（0表示最小）
extern std::map<int, int> cards_round_rank;
// 玩家上贡顺序
extern int contribute_order[4];
// 本轮上贡的人数
extern int contribute_count;

/***** 当前一圈的状态 *****/

// 当前出牌的玩家
extern int turn;
// 当前出牌的牌型
extern int circle_type;
// 当前出牌的相对大小
extern int circle_point;
// 本圈出牌的领圈人
extern int circle_leader;

#endif // STATUS_H_




