#include <algorithm>
#include <qmessagebox.h>
#include "player_widget.h"
#include "qstring.h"
#include "status.h"
#include "sprite.h"
#include "qpushbutton.h"
#include "qlabel.h"
#include "button.h"
#include "hand.h"
#include "sf_btn.h"
#include "status.h"
#include "WildCardDialog.h"


PlayerWidget::PlayerWidget(Hand* hand) :QWidget(), hand_(hand)
{
	/***** 窗口基本设置 ****/
	int id = hand_->id_;
	show();
	//设置标题
	setWindowTitle(QString("玩家") + QString::number(id + 1));
	//背景色深灰色
	setPalette(QColor(35, 35, 35));
	//设置窗口大小
	setGeometry((id / 2) * 200, 30 + (id % 2) * 200, SCREEN_W, SCREEN_H);
	setFixedSize(SCREEN_W, SCREEN_H);

	/***** 基本UI布局设置 ****/
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

	//出牌、不出按钮（默认隐藏）
	btn_pass_ = new Button(340, 200, "img/btn/pass.png", this, Button::Height, 50);
	btn_pass_->hide();
	btn_play_ = new Button(490, 200, "img/btn/play_card0.png", this, Button::Height, 50);
	btn_play_->set_pm("img/btn/play_card1.png", Button::Disabled);
	btn_play_->hide();
	btn_play_->set_mode(Button::Disabled);

	//连接按钮事件
	connect(btn_play_, &Button::click_emit, this, &PlayerWidget::on_play_card);
}

void PlayerWidget::closeEvent(QCloseEvent* event)
{
	emit player_close();
}

void PlayerWidget::sort_card_heap(bool is_partial)
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
	if (!is_partial)
	{
		update_card_heap_show();
	}
}
void PlayerWidget::update_card_heap_show()
{
	emit delete_all_card_bottons();
	btn_arrange_->set_mode(Button::Mode2);

	//清空所有已选卡牌
	selected_cards_.clear();

	//相邻两张牌的偏移量
	//中心位置x:480,极限位置x:115
	int x_count = card_heaps_.size();
	int offset_x = min(100, ((480 - 115) * 2 - 100) / (x_count - 1));
	int offset_y = 35;

	//起始x位置，由左到右渲染
	int x = 480 - (offset_x * (x_count - 1) + 100) / 2.0;
	for (auto& i : card_heaps_)
	{
		//初始y坐标，由上到下渲染
		int y = 360 - (i.second.size() - 1) * offset_y;
		for (auto& j : i.second)
		{
			//创建卡牌按钮
			auto card_btn = new CardButton(x, y, j, this);
			j.set_card_btn(card_btn);
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
		if (straight_flush_comb_[i].empty()) { straight_flush_btns_[i]->set_mode(Button::Disabled); }
		else { straight_flush_btns_[i]->set_mode(Button::Normal); }
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


void PlayerWidget::on_card_selected(CardButton* card_btn, bool is_compulsory)
{
	//已选择牌，可以进行整理
	btn_arrange_->set_mode(Button::Normal);
	//加入已选牌的牌堆
	selected_cards_.push_back(card_btn);
	update_play_btn();
	if (is_compulsory) { return; }
	//用户主动选中牌，检查当前所选牌是否处于组合牌堆中
	for (auto i : card_heaps_)
	{
		auto cards = i.second;
		for (Card card : cards)
		{
			//不是牌堆中当前所选牌
			if (card.get_card_btn() != card_btn) { continue; }

			//当前所选牌不处于组合牌
			if (i.first == false) { return; }

			//选中该组合牌堆中其它所有牌未选中
			int selected_cnt = 0;
			for_each(cards.begin(), cards.end(),
				[&selected_cnt](Card& c) {if (c.get_card_btn()->get_mode() == Button::Mode2) { selected_cnt++; } });
			if (selected_cnt != 0) { return; }

			//强制选中该牌堆中的所有牌
			for (Card card_to_be_selected : cards)
			{
				if (card_to_be_selected.get_card_btn() == card_btn) { continue; }
				compulsory_select(card_to_be_selected.get_card_btn());
			}
		}
	}
}

void PlayerWidget::update_play_btn()
{
	//没轮到自己，不用判断
	if (turn != hand_->id_) { return; }
	auto selected_info = hand_->certain_comb_info(btns_to_cards(selected_cards_));

	//所选牌可出牌的组合
	all_combs_ = hand_->all_valid_comb(btns_to_cards(selected_cards_));
	int combs_cnt = all_combs_.size();
	//不能出牌
	if (combs_cnt == 0)
	{
		btn_play_->set_mode(Button::Disabled);
	}
	else
	{
		btn_play_->set_mode(Button::Normal);
	}
}

std::vector<Card> PlayerWidget::btns_to_cards(const std::vector<CardButton*>& card_btns)
{
	std::vector<Card>cards;
	for (auto i : card_btns)
	{
		cards.push_back(i->get_card());
	}
	return cards;
}

void PlayerWidget::on_card_unselected(CardButton* card_btn)
{
	//从已选牌堆中删除
	selected_cards_.erase(std::find(selected_cards_.begin(), selected_cards_.end(), card_btn));
	//如果没有已选择的牌，将整理按钮改为恢复按钮(模式2)
	if (selected_cards_.empty())
	{
		btn_arrange_->set_mode(Button::Mode2);
	}
	update_play_btn();
}

void PlayerWidget::emit_select(std::vector<Card> cards)
{
	//枚举每一张需要被选中的牌
	for (Card card_to_be_selected : cards)
	{
		for (auto i : card_heaps_)
		{
			//每一个UI按钮
			for (auto j : i.second)
			{
				if (j == card_to_be_selected)
				{
					emit compulsory_select(j.get_card_btn());
					goto Next_card;
				}
			}
		}
	Next_card:;
	}
}

void PlayerWidget::on_arrange_clicked(int mode)
{
	//恢复默认排序
	if (mode == 1)
	{
		sort_card_heap();
		return;
	}

	//mode == 0, 整理选中牌
	//转换为已选Card类型
	std::vector<Card>cards;
	for (auto i : selected_cards_)
	{
		cards.push_back(i->get_card());
	}
	//卡牌排序
	sort(cards.begin(), cards.end());
	//检查所选牌型合法性
	auto selected_info = hand_->certain_comb_info(cards);
	//非合法牌型
	if (selected_info.first == -1)
	{
		//弹出错误提示
		QMessageBox::warning(this, "错误", "所选牌型不合法！");
	}
	//单张牌无需理牌
	else if (selected_info.first == 1)
	{
		QMessageBox::warning(this, "错误", "单张牌无需理牌哦！");
	}
	//合法牌型
	else
	{
		//遍历原有牌堆，删除待排序的卡牌
		for (auto card_to_be_removed : selected_cards_)
		{
			for (int i = 0; i < card_heaps_.size(); i++)
			{
				auto& heap = card_heaps_[i].second;
				for (int j = 0; j < heap.size(); j++)
				{
					//当前位置找到卡牌，删除
					if (heap[j].get_card_btn() == card_to_be_removed)
					{
						heap.erase(heap.begin() + j);
					}
				}
				//如果刷空了牌堆，删除该牌堆
				if (heap.empty())
				{
					card_heaps_.erase(card_heaps_.begin() + i);
					i--;
				}
			}
		}
		sort_card_heap(true);
		//将所选牌加入牌堆
		card_heaps_.push_back(std::make_pair<int, std::vector<Card>>(true, {}));
		for (auto i : cards)
		{
			card_heaps_.back().second.push_back(i);
		}
		update_card_heap_show();
	}
}

void PlayerWidget::on_turn_switched()
{
	//正常打牌模式
	if (circle_type >= 0)
	{
		//自己的回合
		if (turn == hand_->id_)
		{
			//显示出牌、不出按钮
			btn_play_->show();
			btn_pass_->show();
			//按钮移至最前（防止被cards覆盖）
			btn_play_->raise();
			btn_pass_->raise();
		}
		//不是自己的回合
		else
		{
			btn_play_->hide();
			btn_pass_->hide();
		}
	}
}

void PlayerWidget::on_play_card()
{
	//只有唯一一种牌型，直接出牌
	if (all_combs_.size() == 1)
	{
		//to-do
	}
	//逢人配
	else
	{
		auto dialog = new WildCardDialog(all_combs_,this);
		
		//得到返回的选择
		int index = dialog->exec();
	}
}
