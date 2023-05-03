#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include "WildCardDialog.h"
#include "status.h"

WildCardDialog::WildCardDialog(std::vector<std::vector<Card>> combinations, QWidget* parent) :
	QDialog(parent)
{
	//窗口标题
	setWindowTitle("选择逢人配");

	//创建单选按钮组
	button_group_ = new QButtonGroup(this);
	//设置单选按钮排他选择
	button_group_->setExclusive(true);
	for (auto i : combinations)
	{
		//创建单选按钮
		QRadioButton* radio_button = new QRadioButton(this);
		//设置单选按钮图片
		radio_button->setIcon(get_combination_pixmap(i));
		//设置单选按钮大小
		radio_button->setFixedSize(radio_button->iconSize());
		//将单选按钮添加到单选按钮组
		button_group_->addButton(radio_button);
	}

	//确认按钮
	QPushButton* confirm_button = new QPushButton("确定", this);
}
