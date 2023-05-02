#include "player_widget.h"
#include "qstring.h"
#include "status.h"
#include "sprite.h"
#include "qpushbutton.h"
#include "qlabel.h"
#include "button.h"
#include "hand.h"
#include <algorithm>
#include"sf_btn.h"
#include "status.h"
#include <qmessagebox.h>

PlayerWidget::PlayerWidget(Hand* hand) :QWidget(), hand_(hand)
{
	int id = hand_->id_;
	show();
	/*****窗口基本设置****/
	//设置标题
	setWindowTitle(QString("玩家") + QString::number(id + 1));
	//背景色深灰色
	setPalette(QColor(35, 35, 35));
	//设置窗口大小
	setGeometry((id / 2) * 200, 30 + (id % 2) * 200, SCREEN_W, SCREEN_H);
	setFixedSize(SCREEN_W, SCREEN_H);

	/*****基本UI布局设置****/
	//显示级牌底边
	QString rsc_t = QString("img/label/rank_bg_group") + QString::number(id % 2) + QString(".png");
	new Sprite(15, 15, rsc_t, this, Sprite::Height, 65);

	auto set_label = [](QLabel* lb, int y) {
		lb->setStyleSheet("boder:none;");
		lb->setFont(QFont("微软雅黑", 12));
		lb->setGeometry(70, y, 30, 25);
		lb->show();
	};
	lb_rank_self_ = new QLabel("2", this);
	lb_rank_rival_ = new QLabel("2", this);
	set_label(lb_rank_self_, 15);
	set_label(lb_rank_rival_, 47);
	//显示玩家布局
	int id_t = id;
	for (int i = 0; i < 4; i++)
	{
		int id_;
		rsc_t = QString("img/label/player") + QString::number(id_t) + QString(".png");
		new Sprite(SPR_PLAYER_X[i], SPR_PLAYER_Y[i], rsc_t, this, Sprite::Height, 120);
		if (++id_t == 4) { id_t = 0; }
	}
	//显示按钮控件
	// 同花顺标签
	new Sprite(150, 512, "img/label/straight_flush.png", this, Sprite::Height, 20);
	// 同花顺按钮
	for (int i = 0; i < 4; i++)
	{
		straight_flush_btns_[i] = new SfButton(220 + i * 40, 505, i, this);
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i != j)
			{
				//绑定同花顺按钮点击与重置其它按钮
				connect(straight_flush_btns_[i], &SfButton::click_emit, straight_flush_btns_[j], &SfButton::reset_selected_order);
			}
		}
	}
	//理牌按钮
	btn_arrange_ = new Button(700, 506, "img/btn/arrange0.png", this, Button::Height, 40);
	btn_arrange_->set_pm("img/btn/arrange1.png", Button::Mode2);
	connect(btn_arrange_, &Button::click_emit, this, &PlayerWidget::on_arrange_clicked);
	//自动整理牌堆，并显示
	sort_card_heap();
	//显示记录按钮
	auto btn_t = new Button(850, 10, "img/btn/record.png", this, Button::Height, 40);
	connect(btn_t, &Button::click_emit, btn_t, &Button::show_record);
}

void PlayerWidget::closeEvent(QCloseEvent* event)
{
	emit player_close();
}

void PlayerWidget::sort_card_heap()
{
	//无卡牌的情况
	if (card_heaps_.empty()) { return; }

	//获取全部卡牌并重排
	std::vector<Card>cards;
	for (auto i : card_heaps_)
	{
		cards.insert(cards.end(), i.second.begin(), i.second.end());
	}
	sort(cards.begin(), cards.end(), [](Card& c1, Card& c2) {return c2 > c1; });

	//由大到小分点数将卡牌加入到牌堆中
	card_heaps_.clear();
	card_heaps_.push_back(std::make_pair<bool, std::vector<Card>>(false, {}));
	int last_point = (cards.front()).get_point();
	for (auto i : cards)
	{
		//获取卡牌点数
		int point = i.get_point();
		//同种点数牌，加在统一堆
		if (point == last_point) {
			card_heaps_.back().second.push_back(i);
		}
		//不同点数牌，新建一堆
		else {
			card_heaps_.push_back(std::make_pair<bool, std::vector<Card>>(false, {}));
			card_heaps_.back().second.push_back(i);
			last_point = point;
		}
	}
	//更新牌堆显示
	update_card_heap_show();
}
void PlayerWidget::update_card_heap_show()
{
	emit delete_all_card_bottons();

	//清空所有已选卡牌
	selected_cards_.clear();

	//相邻两张牌的偏移量
	//中心位置x:480,极限位置x:115
	int x_count = card_heaps_.size();
	int offset_x = min(100, ((480 - 115) * 2 - 100) / (x_count - 1));
	int offset_y = 35;
	card_btn_heaps_.clear();

	//起始x位置，由左到右渲染
	int x = 480 - (offset_x * (x_count - 1) + 100) / 2.0;
	for (auto i : card_heaps_)
	{
		card_btn_heaps_.push_back({});
		//初始y坐标，由上到下渲染
		int y = 360 - (i.second.size() - 1) * offset_y;
		for (auto& j : i.second)
		{
			//创建卡牌按钮
			auto card_btn = new CardButton(x, y, j, this);
			//将卡牌加到数组中
			card_btn_heaps_.back().push_back(card_btn);
			y += offset_y;
		}
		x += offset_x;
	}

	//检查同花顺情况
	straight_flush_comb_ = hand_->all_straight_flush_combinations();
	//枚举花色
	for (int i = 0; i < 4; i++)
	{
		//该花色不存在同花顺
		if (straight_flush_comb_[i].empty()) { straight_flush_btns_[i]->disable(); }
		else { straight_flush_btns_[i]->enable(); }
	}
}

void PlayerWidget::emit_unselect_all_cards()
{
	emit compulsory_unselect_all_cards();
}

void PlayerWidget::update_all()
{
	//清空原有heap
	card_heaps_.clear();
	card_heaps_.push_back(std::make_pair<int, std::vector<Card>>(false, {}));
	//根据Hand中的数据重建UI
	for (auto i : hand_->get_cards())
	{
		card_heaps_.back().second.push_back(i);
	}
	sort_card_heap();
}


void PlayerWidget::on_card_selected(CardButton* card_btn)
{
	//加入已选牌的牌堆
	selected_cards_.push_back(card_btn);
	//将已选card_btns转换为cards
	std::vector<Card>cards;
	for (auto i : selected_cards_)
	{
		cards.push_back(i->get_card());
	}
	/*to-do*/
	/*switch ()
	{
	default:
		break;
	}*/
	//检查所选卡牌情况
	auto selected_info = hand_->check(cards);
	////非合法牌型
	//if (selected_info.first == -1)
	//{

	//}
}
void PlayerWidget::on_card_unselected(CardButton* card_btn)
{
	//从已选牌堆中删除
	selected_cards_.erase(std::find(selected_cards_.begin(), selected_cards_.end(),card_btn));
}

void PlayerWidget::emit_select(std::vector<Card> cards)
{
	//枚举每一张需要被选中的牌
	for (auto i : cards)
	{
		for (auto j : card_btn_heaps_)
		{
			//每一个UI按钮
			for (auto k : j)
			{
				if (k->get_card() == i)
				{
					emit compulsory_select(k);
					goto Next_card;
				}
			}
		}
	Next_card:;
	}
}

void PlayerWidget::on_arrange_clicked(int mode)
{
	//转换为已选Card类型
	std::vector<Card>cards;
	for (auto i : selected_cards_)
	{
		cards.push_back(i->get_card());
	}
	//理牌模式
	if (mode == 0)
	{
		//检查所选牌型合法性
		auto selected_info = hand_->check(cards);
		//非合法牌型
		if (selected_info.first == -1)
		{
			//弹出错误提示
			QMessageBox::warning(this, "错误", "所选牌型不合法！");
		}
		//合法牌型
		else
		{
			/*to-do 这边写得不对*/
			//遍历原有牌堆，删除待排序的卡牌
			for (int i = 0; i < card_heaps_.size(); i++)
			{
				auto& j = card_heaps_[i].second;
				for (Card k : cards)
				{
					//to-do 这里写得不对
					remove(j.begin(), j.end(), k);
				}
				//如果刷空了牌堆，删除该牌堆
				if (j.empty())
				{
					card_heaps_.erase(card_heaps_.begin() + i);
					i--;
				}
			}
			//将所选牌加入牌堆
			card_heaps_.push_back(std::make_pair<int, std::vector<Card>>(true, {}));
			for (auto i : cards)
			{
				card_heaps_.back().second.push_back(i);
			}
			//card_heaps_.back().second.insert(card_heaps_.back().second.end(), cards.size(), cards);
			//更新牌堆显示
			update_card_heap_show();
		}
	}
	//恢复默认牌序
	else
	{
		sort_card_heap();
	}
}
