#ifndef WILDCARDDIALOG_H_
#define WILDCARDDIALOG_H_

#include <qdialog.h>
#include <vector>
#include "card.h"

//������ѡ��Ի���
class WildCardDialog : public QDialog
{
public:
	//@para
	//combinations: ����ͬ��˳���
	//parent: ������
	WildCardDialog(std::vector<std::vector<Card>> combinations, QWidget* parent);

private:
	//��ѡ��ť��
	QButtonGroup* button_group_;
};

#endif // WILDCARDDIALOG_H_

