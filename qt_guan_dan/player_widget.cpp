#include"player_widget.h"
#include "qstring.h"
#include "qpalette.h"
#include "qcolor.h"

Player_widget::Player_widget(int id)
{
	//设置标题
	setWindowTitle(QString("玩家") + QString::number(id));
	//背景色深灰色
	setPalette(QColor(35, 35, 35));
	//显示级牌底边
	
	//显示玩家布局

	//显示按钮控件

}
