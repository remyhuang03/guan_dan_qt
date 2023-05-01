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
	Sprite* btn_background = new Sprite(0, 0, "img/bg/home.png", this, Sprite::Width, SCREEN_W);
	Button* btn_start_game = new Button(360, 380, "img/btn/start_game.png", this, Button::Height, 80);
	connect(btn_start_game, &Button::click_emit, this, &guan_dan::start_game);
}

void guan_dan::start_game()
{
	//隐藏自身窗口
	hide();
	//获取打乱好的所有卡牌
	auto shuffled_cards = shuffled_all_cards();
	for (int i = 0; i < 4; i++)
	{
		//创建玩家
		players[i] = new Hand(i);
		//创建玩家窗口
		player_widgets[i] = new Player_widget(players[i]);
		//窗口UI绑定到Hand对象
		players[i]->set_widget(player_widgets[i]);
		//Hand绑定到窗口UI
		player_widgets[i]->set_hand(players[i]);

		//分发卡牌
		auto begin = shuffled_cards.begin() + i * 27;
		auto end = shuffled_cards.begin() + (i + 1) * 27;
		players[i]->set_cards(std::vector(begin, end));
		connect(player_widgets[i], &Player_widget::player_close, this, &guan_dan::show);
	}

	//子窗口其一关闭，所有窗口关闭的信号槽连接
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i != j)
				connect(player_widgets[i], &Player_widget::player_close, player_widgets[j], &Player_widget::close);

}

guan_dan::~guan_dan()
{}
