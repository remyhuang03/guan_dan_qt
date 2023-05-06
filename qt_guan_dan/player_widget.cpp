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
#include "status.h"

PlayerWidget::PlayerWidget(Hand* hand) : QWidget(), hand_(hand)
{
	/***** 窗口基本设置 ****/
	id_ = hand_->id_;
	show();
	//设置标题
	setWindowTitle(QString("玩家") + QString::number(id_ + 1));
	//背景色深灰色
	setPalette(QColor(35, 35, 35));
	//设置窗口大小
	//setGeometry((id_ / 2) * 200, 30 + (id_ % 2) * 200, SCREEN_W, SCREEN_H);
	setGeometry(100, 100, SCREEN_W, SCREEN_H);
	setFixedSize(SCREEN_W, SCREEN_H);

	/***** 成员初始化 *****/
	for (auto& i : spr_played_cards_)
	{
		i = nullptr;
	}

	/***** 基本UI布局设置 ****/
	//显示级牌底边
	QString rsc_t = QString("img/label/rank_bg_group") + QString::number(id_ % 2) + QString(".png");
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
	int id_t = id_;
	for (int i = 0; i < 4; i++)
	{
		rsc_t = QString("img/label/player") + QString::number(id_t) + QString(".png");
		new Sprite(SPR_PLAYER_X[i], SPR_PLAYER_Y[i], rsc_t, this, Sprite::Height, 120);
		if (++id_t == 4) { id_t = 0; }
	}
	//显示按钮控件
	// 同花顺标签
	new Sprite(150, 512, QString("img/label/straight_flush.png"), this, Sprite::Height, 20);
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

	//不出按钮（默认隐藏）
	btn_pass_ = new Button(340, 180, "img/btn/pass0.png", this, Button::Height, 50);
	btn_pass_->set_pm("img/btn/pass1.png", Button::Disabled);
	btn_pass_->hide();
	connect(btn_pass_, &Button::click_emit, this, &PlayerWidget::on_pass_clicked);
	//出牌按钮（默认隐藏）
	btn_play_ = new Button(490, 180, "img/btn/play_card0.png", this, Button::Height, 50);
	btn_play_->set_pm("img/btn/play_card1.png", Button::Disabled);
	btn_play_->hide();
	btn_play_->set_mode(Button::Disabled);

	//四个状态标签
	for (int i = 0; i < 4; i++) { lb_status_[(i + id_) % 4] = new StatusLabel(SPR_PLAYER_X[i] + 18, SPR_PLAYER_Y[i] + 70, this); }

	//连接按钮事件
	connect(btn_play_, &Button::click_emit, this, &PlayerWidget::on_play_card);
}

void PlayerWidget::on_new_round() {/*to - do*/ }

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
	for (const auto& i : cards)
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
	int offset_x, offset_y;

	if (x_count == 1) { offset_x = 100; }
	else { offset_x = min(100, ((480 - 115) * 2 - 100) / (x_count - 1)); }
	offset_y = 35;

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
	for (const Card& i : hand_->get_cards())
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
		for (const Card& card : cards)
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
			for (const Card& card_to_be_selected : cards)
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
	if (turn != id_) { return; }
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
	for (const Card& card_to_be_selected : cards)
	{
		for (auto i : card_heaps_)
		{
			//每一个UI按钮
			for (auto& j : i.second)
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
		for (const Card& i : cards)
		{
			card_heaps_.back().second.push_back(i);
		}
		//删除已选卡牌
		selected_cards_.clear();
		update_card_heap_show();
	}
}

void PlayerWidget::on_turn_switched()
{
	qDebug() << circle_leader << " " << turn;
	//轮到领圈人出牌，桌面已出牌清空显示
	if (turn == circle_leader)
	{
		for (int i = 0; i < 4; i++)
		{
			delete_played_cards_ui(i);
		}
	}
	//正常打牌模式
	if (circle_type >= 0)
	{
		//自己的回合
		if (turn == id_)
		{
			//删除自己已出牌的显示
			delete_played_cards_ui(id_);
			//顶置当前活动窗口
			raise();
			//更新出牌按钮模式
			update_play_btn();

			//如果自己是领圈人，则必须出牌
			if (id_ == circle_leader) { btn_pass_->set_mode(Button::Disabled); }
			else { btn_pass_->set_mode(Button::Normal); }

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
	int* selected_wild_card_id = new int;
	//只有唯一一种牌型，直接出牌
	if (all_combs_.size() == 1)
	{
		*selected_wild_card_id = 0;
	}
	//逢人配
	else
	{
		auto dialog = new WildCardDialog(all_combs_, this, selected_wild_card_id);
		//得到返回的选择
		int index = dialog->exec();
	}
	//全局状态操作
	circle_leader = id_;
	auto pair_info = hand_->certain_comb_info(btns_to_cards(selected_cards_));
	circle_type = pair_info.first;
	circle_point = pair_info.second;

	//从hand中删除已出的牌
	hand_->pop_card(btns_to_cards(selected_cards_));


	// 将牌堆中已出的牌全部删除
	for (auto i : selected_cards_)
	{
		for (int j = 0; j < card_heaps_.size(); j++)
		{
			auto& heap = card_heaps_[j].second;
			for (int k = 0; k < heap.size(); k++)
			{
				if (heap[k].get_card_btn() == i)
				{
					heap.erase(heap.begin() + k);
					card_heaps_[j].first = false;
				}
			}
			if (heap.empty())
			{
				card_heaps_.erase(card_heaps_.begin() + j);
				j--;
			}
		}
	}

	sort_card_heap(false);
	//如果牌已经出完
	if (hand_->cards_.empty())
	{
		//更新排名状态
		auto max_rank = std::max_element(std::begin(round_rank), std::end(round_rank));
		round_rank[id_] = *max_rank + 1;
		//实现接风功能
		circle_leader = (id_ + 2) % 4;
	}

	//发送卡牌已打出信号
	emit card_played(all_combs_[*selected_wild_card_id], id_);
}

void PlayerWidget::delete_played_cards_ui(int player_id)
{
	if (spr_played_cards_[player_id] != nullptr)
	{
		delete spr_played_cards_[player_id];
		spr_played_cards_[player_id] = nullptr;
	}
}

void PlayerWidget::update_played_cards_ui(const std::vector<Card>& cards, int player_id)
{
	auto icon = get_combination_pixmap(cards, 0.17);
	int x = PLAYED_CARD_X[(player_id - id_ + 4) % 4];
	int y = PLAYED_CARD_Y[(player_id - id_ + 4) % 4];
	//设置卡牌图标
	spr_played_cards_[player_id] = new Sprite(x, y, icon, this, Sprite::Size, 20);
	//置于图层底层，防止遮挡主牌显示
	spr_played_cards_[player_id]->lower();
	spr_played_cards_[player_id]->show();
}

void PlayerWidget::update_played_cards_ui(int player_id)
{
	int x = SPR_PLAYER_X[(player_id - id_ + 4) % 4] + 13;
	int y = SPR_PLAYER_Y[(player_id - id_ + 4) % 4] + 105;
	spr_played_cards_[player_id] = new Sprite(x, y, QPixmap("img/label/pass.png"), this, Sprite::Size, 30);
	//置于图层底层，防止遮挡主牌显示
	spr_played_cards_[player_id]->lower();
	spr_played_cards_[player_id]->show();
}

void PlayerWidget::on_card_played(const std::vector<Card>& cards, int player_id)
{
	//to-do
	//更新桌面牌面显示
	if (turn != circle_leader) { update_played_cards_ui(cards, player_id); }

	int remain_cnt = players[player_id]->get_cards().size();
	//更新玩家状态情况（剩余牌数<=10）
	if (remain_cnt <= 10) { lb_status_[player_id]->set(StatusLabel::Remain_cnt, remain_cnt); }
	//已经无牌，更新该玩家状态
	if (remain_cnt == 0) { lb_status_[player_id]->set(StatusLabel::Rank, round_rank[player_id]); }
}

void PlayerWidget::on_passed(int player_id)
{
	//更新桌面牌面显示"不出"
	if (turn != circle_leader) { update_played_cards_ui(player_id); }
}

void PlayerWidget::on_pass_clicked()
{
	emit sig_pass(id_);
}
