#include "guan_dan.h"
#include "sprite.h"
#include "button.h"
#include "player_widget.h"
#include "hand.h"

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
		//debug:少发点牌，方便调试
        #define DEBUG_CARD_CNT_ 10 //27
		auto begin = shuffled_cards.begin() + i * DEBUG_CARD_CNT_;
		auto end = shuffled_cards.begin() + (i + 1) * DEBUG_CARD_CNT_;
		players[i]->set_cards(std::vector(begin, end));

		//连接玩家窗口关闭信号槽
		connect(player_widgets[i], &PlayerWidget::player_close, this, &guan_dan::show);
		//玩家窗口下一轮信号槽
		connect(this, &guan_dan::signal_switch_turn, player_widgets[i], &PlayerWidget::on_turn_switched);
		connect(player_widgets[i], &PlayerWidget::card_played, this, &guan_dan::on_card_played);
		connect(player_widgets[i], &PlayerWidget::sig_pass, this, &guan_dan::on_passed);
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
	emit signal_switch_turn();
}


void guan_dan::on_card_played(const std::vector<Card>& cards, int player_id)
{
	//to-do：判断牌局结束

	//轮转到下一个玩家
	switch_turn();
}

void guan_dan::on_passed()
{
	//轮转到下一个玩家
	switch_turn();
}

guan_dan::~guan_dan()
{}
