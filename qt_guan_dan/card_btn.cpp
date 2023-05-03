#include"card_btn.h"
#include"player_widget.h"

CardButton::CardButton(int x, int y, const Card& card, PlayerWidget* parent) :
	Button(x, y,
		"img/card/" + QString::number(card.get_point()) + "_" + QString::number(card.get_suit()) + ".png",
		parent, Button::Size, 40), card_(card)
{
	//禁用常规按钮动画
	this->disable_animation();
	//设置选中图标
	set_pm("img/card/" + QString::number(card.get_point()) + "_" + QString::number(card.get_suit()) + "c.png", Mode2);
	//绑定自杀事件
	connect(parent, &PlayerWidget::delete_all_card_bottons, this, &CardButton::delete_self);
	//绑定点击事件
	connect(this, &CardButton::clicked, this, &CardButton::on_clicked);
	//绑定强制选择事件
	connect(parent, &PlayerWidget::compulsory_select, this, &CardButton::on_compulsory_selected);
	//绑定强制取消选择事件
	connect(parent, &PlayerWidget::compulsory_unselect_all_cards, this, &CardButton::on_compulsory_unselected);
	//绑定自己被选中事件
	connect(this, &CardButton::card_selected, parent, &PlayerWidget::on_card_selected);
	//绑定自己被取消选中事件
	connect(this, &CardButton::card_unselected, parent, &PlayerWidget::on_card_unselected);
}

void CardButton::delete_self()
{
	delete this;
}

void CardButton::on_clicked()
{
	//取消选中
	if (mode_ == Mode2) {
		emit card_unselected(this);
	}
	//如果未被选中，选中
	else {
		emit card_selected(this,false);
	}
	reverse_mode();
}

void CardButton::on_compulsory_unselected()
{
	//如果原先处于选中状态
	if (mode_ == Mode2) {
		emit card_unselected(this);
		reverse_mode();
	}
}

void CardButton::on_compulsory_selected(CardButton* btn)
{
	//如果不是我需要被取消选择 and 原先处于未选中状态
	if (btn == this && mode_ == Normal)
	{
		emit card_selected(this);
		reverse_mode();
	}
}
