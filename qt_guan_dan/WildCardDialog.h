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
	WildCardDialog(std::vector<std::vector<Card>> combinations, QWidget* parent);

private:
	//单选按钮组
	QButtonGroup* button_group_;
};

#endif // WILDCARDDIALOG_H_

