#ifndef  CARD_BTN_H
#define CARD_BTN_H
#include "button.h"
#include "card.h"
// UI界面中棋牌按钮
class CardBotton :public Button
{
public:
	CardBotton(int x, int y, const Card& card, QWidget* parent);

	int get_point() const { return card_.get_point(); }
	int get_suit() const { return card_.get_suit(); }
	Card get_card() const { return card_; }

public slots:
	//删除自己
	void delete_self();
	//点击处理
	void on_clicked();

private:
	Card card_;
};
#endif // ! CARD_BTN_H
