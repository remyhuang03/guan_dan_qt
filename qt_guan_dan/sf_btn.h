#ifndef SF_BUTTON_H_
#define SF_BUTTON_H_
#include "button.h"

class SfButton :
	public Button
{
public:
	//@para
	//img:ͼƬ·��
	//suite:��ɫ���
	//parent:������
	SfButton(int x, int y, int suite, QWidget* parent);

public slots:
	void on_clicked();

private:
	//�û�ɫͬ��˳����ѡ�����±꣨-1Ϊδѡ��
	int selected_order_;
	//��ɫ
	int suite_;
};

#endif // SF_BUTTON_H_

