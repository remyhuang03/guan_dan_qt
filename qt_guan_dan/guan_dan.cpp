#include "guan_dan.h"
#include "sprite.h"
#include "button.h"
#include "player_widget.h"
#include "hand.h"

//debug:少发点牌，方便调试
#define DEBUG_CARD_CNT_ 27 //正常值：27

guan_dan::guan_dan(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle("掼蛋");
	//16：9 分辨率显示
	setFixedSize(SCREEN_W, SCREEN_H);
	//背景色灰色
	setPalette(QColor(35, 35, 35));
	Sprite* btn_background = new Sprite(0, 0, QString("img/bg/home.png"), this, Sprite::Width, SCREEN_W);
	Button* btn_start_game = new Button(360, 380, "img/btn/start_game.png", this, Button::Height, 80);
	connect(btn_start_game, &Button::click_emit, this, &guan_dan::start_game);
}

void guan_dan::start_game()
{
	//初始化游戏数据
	init_game_data();
	//隐藏自身窗口
	hide();
	//获取打乱好的所有卡牌
	auto shuffled_cards = shuffled_all_cards();
	for (int i = 0; i < 4; i++)
	{
		//创建玩家
		players[i] = new Hand(i);
		//创建玩家窗口
		player_widgets[i] = new PlayerWidget(players[i]);
		//窗口UI绑定到Hand对象
		players[i]->set_widget(player_widgets[i]);
		//Hand绑定到窗口UI
		player_widgets[i]->set_hand(players[i]);

		//分发卡牌
		auto begin = shuffled_cards.begin() + i * DEBUG_CARD_CNT_;
		auto end = shuffled_cards.begin() + (i + 1) * DEBUG_CARD_CNT_;
		players[i]->set_cards(std::vector(begin, end));

		//连接玩家窗口关闭信号槽
		connect(player_widgets[i], &PlayerWidget::player_close, this, &guan_dan::show);
		//玩家窗口下一轮信号槽
		connect(this, &guan_dan::sig_switch_turn, player_widgets[i], &PlayerWidget::on_turn_switched);
		connect(this, &guan_dan::sig_new_round, player_widgets[i], &PlayerWidget::on_new_round);
		connect(player_widgets[i], &PlayerWidget::sig_pass, this, &guan_dan::on_passed);
		connect(player_widgets[i], &PlayerWidget::sig_global_card_played_process, this, &guan_dan::on_card_played);
	}

	//子窗口其一关闭，所有窗口关闭的信号槽连接
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (i != j) { connect(player_widgets[i], &PlayerWidget::player_close, player_widgets[j], &PlayerWidget::close); }
			connect(player_widgets[i], &PlayerWidget::card_played, player_widgets[j], &PlayerWidget::on_card_played);
			connect(player_widgets[i], &PlayerWidget::sig_pass, player_widgets[j], &PlayerWidget::on_passed);
		}
	//游戏开始，轮转下一位
	switch_turn(false);
}

void guan_dan::switch_turn(bool is_next)
{
	if (is_next)
	{
		do
		{
			turn = (turn + 1) % 4;
		} while (round_rank[turn] != -1);
	}
	emit sig_switch_turn();
}


void guan_dan::on_card_played(const std::vector<Card>& cards, int player_id)
{

	card_played_process_count = 0;
	//该组为胜利，轮转到下一个玩家
	if (round_rank[player_id] == -1 || round_rank[(player_id + 2) % 4] == -1)
	{
		qDebug() << "switch_turn";
		switch_turn(true);
		return;
	}

	//该方两个玩家的牌均已出完，牌局结束
	int& rival_rank_1 = round_rank[(player_id + 1) % 4];
	int& rival_rank_2 = round_rank[(player_id + 3) % 4];
	//对手组都未出完牌
	if (rival_rank_1 == -1 && rival_rank_2 == -1)
	{
		//随意设置两人最终排名
		rival_rank_1 = 2;
		rival_rank_2 = 3;
	}
	//对手1已结束
	else if (rival_rank_1 != -1) { rival_rank_2 = 3; }
	//对手2已结束
	else { rival_rank_1 = 3; }

	//更新 round 数据
	round_over();

	//检查整个掼蛋游戏结束表示
	if (is_game_over)
	{
		emit sig_game_over();
		return;
	}

	//进入下一个 round
	//获取打乱好的所有卡牌
	auto shuffled_cards = shuffled_all_cards();
	for (int i = 0; i < 4; i++)
	{
		//分发卡牌
		auto begin = shuffled_cards.begin() + i * DEBUG_CARD_CNT_;
		auto end = shuffled_cards.begin() + (i + 1) * DEBUG_CARD_CNT_;
		players[i]->set_cards(std::vector(begin, end));
	}

	//贡牌判断
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
		auto temp_cards = players[rank_list[2]]->get_cards();
		int JOKER_count = std::count(temp_cards.begin(), temp_cards.end(), Card(15, -1));
		temp_cards = players[rank_list[3]]->get_cards();
		JOKER_count += std::count(temp_cards.begin(), temp_cards.end(), Card(15, -1));
		// 抗贡
		if (JOKER_count == 2)
		{
			turn = rank_list[0];
			contribute_count = 0;
		}
		else //认命
		{
			contribute_order[0] = rank_list[3];
			contribute_order[1] = rank_list[2];
		}
	}
	else
	{
		// 检查是否可以进行抗贡
		auto temp_cards = players[rank_list[3]]->get_cards();
		int JOKER_count = std::count(temp_cards.begin(), temp_cards.end(), Card(15, -1));
		// 抗贡
		if (JOKER_count == 2)
		{
			turn = rank_list[0];
			contribute_count = 0;
		}
		else //认命
		{
			contribute_count = 1;
			contribute_order[0] = rank_list[3];
		}
	}
	emit sig_new_round();
	/*circle_type = -4;
	while (contribute_count--)
	{

	}*/
	//to-do new_round();
}

void guan_dan::on_passed()
{
	//轮转到下一个玩家
	switch_turn(true);
}


