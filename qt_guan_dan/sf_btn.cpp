#include "sf_btn.h"

SfButton::SfButton(int x, int y, int suite, QWidget* parent) :
	Button(x, y, "img/btn/straight_flush" + QString::number(suite) + "0" + ".png", parent, Button::Height, 40),
	suite_(suite), selected_order_(-1)
{
	//设置禁用图标
	set_pm("img/btn/straight_flush" + QString::number(suite) + "1" + ".png", Disabled);
	//绑定点击事件
	connect(this, &SfButton::click_emit, this, &SfButton::on_clicked, Qt::DirectConnection);
}

void SfButton::on_clicked()
{
	/*to-do*/
}
