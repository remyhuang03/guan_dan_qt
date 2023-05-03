#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include "WildCardDialog.h"
#include "status.h"

WildCardDialog::WildCardDialog(std::vector<std::vector<Card>> combinations, QWidget* parent) :
	QDialog(parent)
{
	//���ڱ���
	setWindowTitle("ѡ�������");

	//������ѡ��ť��
	button_group_ = new QButtonGroup(this);
	//���õ�ѡ��ť����ѡ��
	button_group_->setExclusive(true);
	for (auto i : combinations)
	{
		//������ѡ��ť
		QRadioButton* radio_button = new QRadioButton(this);
		//���õ�ѡ��ťͼƬ
		radio_button->setIcon(get_combination_pixmap(i));
		//���õ�ѡ��ť��С
		radio_button->setFixedSize(radio_button->iconSize());
		//����ѡ��ť��ӵ���ѡ��ť��
		button_group_->addButton(radio_button);
	}

	//ȷ�ϰ�ť
	QPushButton* confirm_button = new QPushButton("ȷ��", this);
}
