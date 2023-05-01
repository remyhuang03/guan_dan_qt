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

	//@brief 设置是否被选中
	void set_chosen(bool chosen) { chosen_ = chosen; }

public slots:
	//删除自己
	void delete_self();

private:
	//是否被选中
	bool chosen_;
	Card card_;
};
#endif // ! CARD_BTN_H
