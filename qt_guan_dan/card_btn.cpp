#include"card_btn.h"
#include"player_widget.h"

CardBotton::CardBotton(int x, int y, const Card& card, QWidget* parent) :
	Button(x, y,
		"img/card/" + QString::number(card.get_point()) + "_" + QString::number(card.get_suit()) + ".png",
		parent, Button::Size, 40), card_(card)
{
	//禁用常规按钮动画
	this->disable_animation();
	//设置选中图标
	set_pm("img/card/" + QString::number(card.get_point()) + "_" + QString::number(card.get_suit()) + "c.png", Mode2);

}

void CardBotton::delete_self()
{
	delete this;
}

void CardBotton::on_clicked()
{
	//如果被选中，取消选中
	if (mode_ = Mode2) {

	}
	//如果未被选中，选中
	else {
	}
	reverse_mode();
}


