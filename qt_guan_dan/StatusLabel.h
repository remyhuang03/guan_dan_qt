#ifndef  STATUS_LABEL_H_
#define STATUS_LABEL_H_

#include <qlabel.h>
#include "sprite.h"
class PlayerWidget;

//显示在玩家头像底部的剩余牌张/已完成游戏玩家的排名
class StatusLabel
{
public:
	StatusLabel(int x, int y, QWidget* parent);

	//标签模式
	enum Mode { Remain_cnt, Rank, Hidden };

	//@brief 设置模式（剩余牌数/完成游戏/隐藏）
	//@para mode: 模式   arg:该模式下的参数（剩余牌数/游戏排名/0）
	void set(Mode mode, int arg = 0);

private:
	Sprite* play_status_bg;
	QLabel* lb_remaning_card_cnt;
	int x_;
	int y_;
};

#endif //STATUS_LABEL_H_

