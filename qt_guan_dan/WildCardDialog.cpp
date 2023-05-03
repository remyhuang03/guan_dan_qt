#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include "WildCardDialog.h"
#include "status.h"

WildCardDialog::WildCardDialog(std::vector<std::vector<Card>> combinations, QWidget* parent) :
	QDialog(parent)
{
	//窗口标题
	setWindowTitle("选择逢人配");
	//设置模态
	setModal(true);

	//创建单选按钮组
	button_group_ = new QButtonGroup(this);
	//创建竖直布局
	QVBoxLayout* layout = new QVBoxLayout(this);

	//设置单选按钮排他选择
	button_group_->setExclusive(true);
	for (auto i : combinations)
	{
		//创建单选按钮
		QRadioButton* radio_button = new QRadioButton(this);

		auto icon = get_combination_pixmap(i);
		//设置单选按钮图片
		radio_button->setIcon(icon);
		//设置单选按钮大小
		radio_button->setIconSize(QSize(icon.size().width() * 0.3, icon.size().height() * 0.3));

		//将单选按钮添加到单选按钮组和布局
		button_group_->addButton(radio_button);
		layout->addWidget(radio_button);
	}

	//确认按钮
	QPushButton* confirm_button = new QPushButton("确定", this);
	layout->addWidget(confirm_button);

	show();
}
