#include "button.h"
#include "record_widget.h"
Button::Button(int x, int y, QString img, QWidget* parent, SizeMode mode, double size)
	:Sprite(x, y, img, parent, mode, size), mode_(Normal), animation_disabled_(false)
{
	connect(this, &Button::clicked, this, &Button::click_animation);
	icons[0] = icons[1] = icons[2] = QPixmap(img);
}

void Button::click_animation()
{
	//前提：当前按钮可用
	if (mode_ == -1 || animation_disabled_)
	{
		return;
	}
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
	emit click_emit(static_cast<int>(mode_));
}

void Button::reverse_mode()
{
	if (mode_ == Mode2)
	{
		set_mode(Normal);
	}
	else
	{
		set_mode(Mode2);
	}
}

void Button::set_pm(QString img, Mode mode) {
	icons[int(mode + 1)] = QPixmap(img);
}

void Button::show_record()
{
	new Record_widget();
}

void Button::set_mode(Mode mode)
{
	mode_ = mode;
	setIcon(icons[int(mode + 1)]);
}
