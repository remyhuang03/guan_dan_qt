#include"card_btn.h"
#include"player_widget.h"

CardBotton::CardBotton(int x, int y, const Card& card, QWidget* parent) :
	Button(x, y,
		"img/card/" + QString::number(card.get_point()) + "_" + QString::number(card.get_suit()) + ".png",
		parent, Button::Size, 40), chosen_(false), card_(card)
{
	//禁用常规按钮动画
	this->disable_animation();
}

void CardBotton::delete_self()
{
	delete this;
}


