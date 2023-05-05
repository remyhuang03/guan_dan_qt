#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include "WildCardDialog.h"
#include "status.h"

WildCardDialog::WildCardDialog(const std::vector<std::vector<Card>>& combinations, QWidget* parent, int* selected_id) :
	QDialog(parent), selected_id_(selected_id)
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
	int button_id = 0;
	for (auto i : combinations)
	{
		//创建单选按钮
		QRadioButton* radio_button = new QRadioButton(this);
		sort(i.begin(), i.end());
		auto icon = get_combination_pixmap(i);
		//设置单选按钮图片
		radio_button->setIcon(icon);
		//设置单选按钮大小
		radio_button->setIconSize(QSize(icon.size().width() * 0.3, icon.size().height() * 0.3));

		//将单选按钮添加到单选按钮组和布局
		button_group_->addButton(radio_button, button_id);
		layout->addWidget(radio_button);
		button_id++;
	}

	//确认按钮
	btn_confirm_ = new QPushButton("确定", this);
	//绑定按钮确认事件
	connect(btn_confirm_, &QPushButton::clicked, this, &WildCardDialog::on_confirm);
	layout->addWidget(btn_confirm_);

	show();
}

void WildCardDialog::on_confirm()
{
	//找到被选中的按钮编号
	int selected_id = button_group_->checkedId();
	//没有按钮被选中, 返回
	if (selected_id == -1) { return; }
	//将选中id报给主窗口
	*selected_id_ = selected_id;
	//关闭选择窗口
	close();
}
