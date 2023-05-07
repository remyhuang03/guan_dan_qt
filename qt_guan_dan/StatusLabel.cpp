#include "StatusLabel.h"

StatusLabel::StatusLabel(int x, int y, QWidget* parent) :x_(x), y_(y)
{
	//背景图标（默认置空）
	play_status_bg = new Sprite(x, y, QPixmap("img/label/blank.png"), parent, Sprite::Height, 60);
	lb_remaning_card_cnt = new QLabel(parent);
	lb_remaning_card_cnt->setStyleSheet("font-size: 14px; color: white; font-weight: bold;");
	//置顶覆盖头像位置
	play_status_bg->raise();
	lb_remaning_card_cnt->raise();
	//默认隐藏
	set(Hidden);
}

void StatusLabel::set(Mode mode, int arg)
{
	switch (mode)
	{
	case Remain_cnt:
		lb_remaning_card_cnt->setText(QString::number(arg));
		if (arg == 10) { lb_remaning_card_cnt->setGeometry(x_ + 23, y_ + 8, 40, 40); }
		else { lb_remaning_card_cnt->setGeometry(x_ + 27, y_ + 8, 40, 40); }
		play_status_bg->set_pm(QString("img/label/remaining_card.png").arg(arg));
		play_status_bg->show();
		lb_remaning_card_cnt->show();
		break;
	case Rank:
		play_status_bg->set_pm(QString("img/label/round_rank%1.png").arg(arg));
		qDebug() << "arg:" << arg;
		play_status_bg->show();
		lb_remaning_card_cnt->hide();
		break;
	case Hidden:
		play_status_bg->hide();
		lb_remaning_card_cnt->hide();
		break;
	default:
		break;
	}
}
