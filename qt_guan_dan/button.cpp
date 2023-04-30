#include "button.h"
Button::Button(int x, int y, QString img, QWidget* parent, SizeMode mode, double size)
	:Sprite(x, y, img, parent, mode, size)
{
	connect(this, &Button::clicked, this, &Button::click_animation);

}

void Button::click_animation()
{
	//播放点击动画
	QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
	int posx = pos().x();
	int posy = pos().y();
	animation->setDuration(150);
	animation->setStartValue(QRect(posx, posy, w_, h_));
	animation->setEndValue(QRect(posx + 2, posy + 2, w_, h_));
	animation->start();
	animation->setDuration(150);
	animation->setStartValue(QRect(posx + 2, posy + 2, w_, h_));
	animation->setEndValue(QRect(posx, posy, w_, h_));
	animation->start();
	//处理点击完以后的事件
	emit click_emit();
}
