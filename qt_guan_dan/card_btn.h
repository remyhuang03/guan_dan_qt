#ifndef  CARD_BTN_H
#define CARD_BTN_H
#include "card.h"
#include "button.h"
#include "player_widget.h"

// UI界面中棋牌按钮
class CardButton :public Button
{
	Q_OBJECT
public:
	CardButton(int x, int y, const Card& card, PlayerWidget* parent);

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

signals:
	void card_selected(CardButton* card_btn);
	void card_unselected(CardButton* card_btn);
};
#endif // ! CARD_BTN_H
