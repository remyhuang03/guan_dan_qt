#ifndef SF_BUTTON_H_
#define SF_BUTTON_H_
#include "button.h"
#include "player_widget.h"

class SfButton :
	public Button
{
public:
	//@para
	//img:图片路径
	//suite:花色编号
	//parent:父窗口
SfButton(int x, int y, int suite, PlayerWidget* parent);

public slots:
	void on_clicked();
	void reset_selected_order();

signals:
	

private:
	//该花色同花顺的已选方案下标（-1为未选择）
	int selected_order_;
	//花色
	int suite_;
	//父窗口
	PlayerWidget* parent_;
};

#endif // SF_BUTTON_H_

