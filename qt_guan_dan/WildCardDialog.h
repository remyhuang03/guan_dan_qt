#ifndef WILDCARDDIALOG_H_
#define WILDCARDDIALOG_H_

#include <qdialog.h>
#include <vector>
#include "card.h"

//逢人配选择对话框
class WildCardDialog : public QDialog
{
public:
	//@para
	//combinations: 各种同花顺组合
	//parent: 父窗口
	WildCardDialog(const std::vector<std::vector<Card>>& combinations, QWidget* parent, int* selected_id);

public slots:
	//@brief 确认按钮槽函数
	void on_confirm();

private:
	//单选按钮组
	QButtonGroup* button_group_;
	//确认按钮
	QPushButton* btn_confirm_;
	//用户选择的逢人配(需要返回)
	int* selected_id_;
};

#endif // WILDCARDDIALOG_H_

