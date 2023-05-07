#include "sf_btn.h"

SfButton::SfButton(int x, int y, int suite, PlayerWidget* parent) :
	Button(x, y, "img/btn/straight_flush" + QString::number(suite) + "0" + ".png", parent, Button::Height, 40),
	suite_(suite), selected_order_(-1), parent_(parent)
{
	//设置禁用图标
	set_pm("img/btn/straight_flush" + QString::number(suite) + "1" + ".png", Disabled);
	//绑定点击事件
	connect(this, &SfButton::sig_click_emit, this, &SfButton::on_clicked);
}

void SfButton::on_clicked()
{
	parent_->emit_unselect_all_cards();
	auto comb = parent_->get_sf_combination(suite_);
	if (++selected_order_ > comb.size() - 1)
	{
		selected_order_ = 0;
	}
	parent_->emit_select(comb[selected_order_]);
}

void SfButton::on_reset_selected_order()
{
	selected_order_ = -1;
}
