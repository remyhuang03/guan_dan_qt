#include "button.h"
#include "record_widget.h"
Button::Button(int x, int y, QString img, QWidget* parent, SizeMode mode, double size)
	:Sprite(x, y, img, parent, mode, size), mode_(Normal), animation_disabled_(false),
	x_(x), y_(y)
{
	connect(this, &Button::clicked, this, &Button::on_click_animation);
	icons[0] = icons[1] = icons[2] = QPixmap(img);
}

void Button::on_click_animation()
{
	//前提：当前按钮可用
	if (mode_ == -1 || animation_disabled_)
	{
		return;
	}
	//播放点击动画
	QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
	animation->setDuration(150);
	animation->setStartValue(QRect(x_, y_, w_, h_));
	animation->setEndValue(QRect(x_ + 2, y_ + 2, w_, h_));
	animation->start();
	animation->setDuration(150);
	animation->setStartValue(QRect(x_ + 2, y_ + 2, w_, h_));
	animation->setEndValue(QRect(x_, y_, w_, h_));
	animation->start();
	//处理点击完以后的事件
	emit sig_click_emit(static_cast<int>(mode_));
}

void Button::reverse_mode()
{
	set_mode(Mode(!int(mode_)));
}

void Button::set_pm(QString img, Mode mode) {
	icons[int(mode + 1)] = QPixmap(img);
}

void Button::on_show_record()
{
	new Record_widget();
}

void Button::set_mode(Mode mode)
{
	mode_ = mode;
	setIcon(icons[int(mode + 1)]);
}
