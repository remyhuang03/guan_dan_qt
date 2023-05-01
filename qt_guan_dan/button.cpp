#include "button.h"
#include "record_widget.h"
Button::Button(int x, int y, QString img, QWidget* parent, SizeMode mode, double size)
	:Sprite(x, y, img, parent, mode, size), mode_(Normal), animation_disabled_(false)
{
	connect(this, &Button::clicked, this, &Button::click_animation);
	pm_normal_ = pm_disabled_ = pm_mode2_ = QPixmap(img);
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
	emit click_emit();
}
void Button::disable() {
	mode_ = Disabled;
	setIcon(pm_disabled_);
}
void Button::enable()
{
	mode_ = Normal;
	setIcon(pm_normal_);
}
void Button::reverse_mode()
{
	if (mode_ == Mode2)
	{
		mode_ = Normal;
		setIcon(pm_normal_);
	}
	else
	{
		mode_ = Mode2;
		setIcon(pm_mode2_);
	}
}

void Button::set_pm(QString img, Mode mode) {
	switch (mode)
	{
	case Button::Disabled:
		pm_disabled_ = QPixmap(img);
		break;
	case Button::Normal:
		pm_normal_ = QPixmap(img);
		break;
	case Button::Mode2:
		pm_mode2_ = QPixmap(img);
		break;
	default:
		break;
	}
}

void Button::show_record()
{
	new Record_widget();
}
