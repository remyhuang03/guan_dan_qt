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
	Sprite* btn_start_game = new Button(360, 380, "img/btn/start_game.png", this, Button::Height, 80);
	connect(btn_start_game, &Button::click_emit, this, &guan_dan::start_game);
}

void guan_dan::start_game()
{
	//隐藏自身窗口
	hide();

	for (int i = 0; i < 4; i++)
	{
		//创建玩家窗口
		player_widgets[i] = new Player_widget(i);
		//创建玩家
		players[i] = new Hand(i);
		//获取打乱好的所有卡牌并分发
		auto shuffled_cards = shuffled_all_cards();
		for (int i = 0; i < 4; i++)
		{
			//设置每位玩家的牌（每人27张）
			auto begin = shuffled_cards.begin() + i * 27;
			auto end = shuffled_cards.end() + (i + 1) * 27;
			players[i]->set_cards(std::vector(begin, end));
		}
	}
}

guan_dan::~guan_dan()
{}
