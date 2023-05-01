#ifndef  CARD_BTN_H
#define CARD_BTN_H
#include "button.h"
#include "card.h"
// UI���������ư�ť
class CardBotton :public Button
{
public:
	CardBotton(int x, int y, const Card& card, QWidget* parent);

	int get_point() const { return card_.get_point(); }
	int get_suit() const { return card_.get_suit(); }
	Card get_card() const { return card_; }

	//@brief �����Ƿ�ѡ��
	void set_chosen(bool chosen) { chosen_ = chosen; }

public slots:
	//ɾ���Լ�
	void delete_self();

private:
	//�Ƿ�ѡ��
	bool chosen_;
	Card card_;
};
#endif // ! CARD_BTN_H
