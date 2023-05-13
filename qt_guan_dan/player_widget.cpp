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
				connect(straight_flush_btns_[i], &SfButton::sig_click_emit, straight_flush_btns_[j], &SfButton::on_reset_selected_order);
			}
		}
	}

	//理牌按钮
	btn_arrange_ = new Button(700, 506, "img/btn/arrange0.png", this, Button::Height, 40);
	btn_arrange_->set_pm("img/btn/arrange1.png", Button::Mode2);
	connect(btn_arrange_, &Button::sig_click_emit, this, &PlayerWidget::on_arrange_clicked);

	//自动整理牌堆，并显示
	sort_card_heap();

	//显示记录按钮
	auto btn_t = new Button(850, 10, "img/btn/record.png", this, Button::Height, 40);
	connect(btn_t, &Button::sig_click_emit, btn_t, &Button::on_show_record);

	//不出按钮（默认隐藏）
	btn_pass_ = new Button(340, 180, "img/btn/pass0.png", this, Button::Height, 50);
	btn_pass_->set_pm("img/btn/pass1.png", Button::Disabled);
	btn_pass_->hide();
	connect(btn_pass_, &Button::sig_click_emit, this, &PlayerWidget::on_pass_clicked);

	//出牌按钮（默认隐藏）
	btn_play_ = new Button(490, 180, "img/btn/play_card0.png", this, Button::Height, 50);
	btn_play_->set_pm("img/btn/play_card1.png", Button::Disabled);
	btn_play_->hide();
	btn_play_->set_mode(Button::Disabled);
	connect(btn_play_, &Button::sig_click_emit, this, &PlayerWidget::on_play_card);

	//上贡按钮
	btn_contribute_ = new Button(430, 180, "img/btn/contribute0.png", this, Button::Height, 50);
	btn_contribute_->set_pm("img/btn/contribute1.png", Button::Disabled);
	btn_contribute_->hide();
	btn_contribute_->set_mode(Button::Disabled);
	connect(btn_contribute_, &Button::sig_click_emit, this, &PlayerWidget::on_conretribute);

	//还贡按钮
	btn_retribute_ = new Button(430, 180, "img/btn/retribute0.png", this, Button::Height, 50);
	btn_retribute_->set_pm("img/btn/retribute1.png", Button::Disabled);
	btn_retribute_->hide();
	btn_retribute_->set_mode(Button::Disabled);
	connect(btn_retribute_, &Button::sig_click_emit, this, &PlayerWidget::on_conretribute);

	//四个状态标签
	for (int i = 0; i < 4; i++)
	{
		lb_status_[(i + id_) % 4] =
			new StatusLabel(SPR_PLAYER_X[i] + 18, SPR_PLAYER_Y[i] + 70, this);
	}

	//级牌按钮（第一个round指定为第一项）
	spr_star = new Sprite(100, 20, "img/label/star.png", this, Sprite::Height, 20);
	spr_star->show();
}

void PlayerWidget::on_new_round()
{
	qDebug() << "new round";
	//删除所有已出牌显示
	for (int i = 0; i < 4; i++) { delete_played_cards_ui(i); }

	//更新等级显示
	lb_rank_self_->setText(QString::number(group_rank[hand_->get_group()]));
	lb_rank_rival_->setText(QString::number(group_rank[!hand_->get_group()]));

	//自动整理牌堆，并显示
	update_all();

	//不出按钮隐藏
	btn_pass_->hide();
	//出牌按钮隐藏
	btn_play_->hide();
	btn_play_->set_mode(Button::Disabled);

	//四个状态标签标记为上一轮的游戏排名
	for (int i = 0; i < 4; i++) { lb_status_[i]->set(StatusLabel::Rank, round_rank[i]); }

	//级牌标识
	int w_t = spr_star->size().width();
	int h_t = spr_star->size().height();
	spr_star->setGeometry(100, 20 + 30 * ((rank_list[0] % 2) == (id_ % 2)), w_t, h_t);
}

void PlayerWidget::closeEvent(QCloseEvent* event)
{
	emit sig_player_close();
}

void PlayerWidget::sort_card_heap(bool is_internal, bool is_partial)
{
	//无卡牌的情况
	if (card_heaps_.empty()) { return; }

	//获取卡牌并重排
	std::vector<std::pair<bool, std::vector<Card>>> remaining_heaps;
	std::vector<Card>cards;
	for (auto heap : card_heaps_)
	{
		//是理过的牌
		if (is_partial && heap.first)
		{
			remaining_heaps.push_back(heap);
		}
		else
		{
			cards.insert(cards.end(), heap.second.begin(), heap.second.end());
		}
	}
	sort(cards.begin(), cards.end(), [](Card& c1, Card& c2) {return c2 > c1; });

	//由大到小分点数将卡牌加入到牌堆中
	card_heaps_.clear();
	card_heaps_.push_back(std::make_pair<bool, std::vector<Card>>(false, {}));
	int last_point;
	if (!cards.empty())
	{
		last_point = (cards.front()).get_point();
	}

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
	//加入剩余卡牌
	card_heaps_.insert(card_heaps_.end(), remaining_heaps.begin(), remaining_heaps.end());

	//更新牌堆显示
	if (!is_internal)
	{
		update_card_heap_show();
	}
}

void PlayerWidget::update_card_heap_show()
{
	emit sig_delete_all_card_bottons();
	btn_arrange_->set_mode(Button::Mode2);
	btn_play_->set_mode(Button::Disabled);

	//清空所有已选卡牌
	selected_cards_.clear();

	//相邻两张牌的偏移量
	//中心位置x:480,极限位置x:115
	int x_count = card_heaps_.size();
	int offset_x, offset_y;

	if (x_count == 1) { offset_x = 100; }
	else { offset_x = std::min(100, ((480 - 115) * 2 - 100) / (x_count - 1)); }
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
	emit sig_compulsory_unselect_all_cards();
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
				sig_compulsory_select(card_to_be_selected.get_card_btn());
			}
		}
	}
}

void PlayerWidget::update_play_btn()
{
	//没轮到自己，不用判断
	if (turn != id_) { return; }

	//判断所选牌能否上贡
	if (circle_type == -4 || circle_type == -3)
	{
		bool could_contribute = true;
		if (selected_cards_.size() != 1)
		{
			could_contribute = false;
		}
		else
		{
			Card selected_card = selected_cards_[0]->get_card();
			//上贡条件：除级牌外最大的牌
			for (const Card& card : hand_->cards_)
			{
				//发现了比当前牌牌点更大的牌且不是级牌
				if (card.get_point() > selected_card.get_point() &&
					card.get_point() != round_rank_card)
				{
					could_contribute = false;
					break;
				}
			}
		}
		btn_contribute_->set_mode(could_contribute ? Button::Normal : Button::Disabled);
	}
	//判断所选牌能否还贡
	else if (circle_type == -2 || circle_type == -1)
	{
		//还贡条件：不能大于10
		if (selected_cards_.size() == 1 &&
			selected_cards_[0]->get_card().get_point() >= 3
			&& selected_cards_[0]->get_card().get_point() <= 10)
		{
			btn_retribute_->set_mode(Button::Normal);
		}
		else
		{
			btn_retribute_->set_mode(Button::Disabled);
		}
	}
	else
	{
		auto selected_info = hand_->certain_comb_info(btns_to_cards(selected_cards_));
		//所选牌可出牌的组合
		all_combs_ = hand_->all_valid_comb(btns_to_cards(selected_cards_));
		int combs_cnt = all_combs_.size();

		btn_play_->set_mode(combs_cnt == 0 ? Button::Disabled : Button::Normal);
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
	qDebug() << "unselected" << card_btn;
	qDebug() << selected_cards_[0];
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
					emit sig_compulsory_select(j.get_card_btn());
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
	qDebug() <<"switch:" << turn << id_;
	//上贡
	if ((circle_type == -4 || circle_type == -3) &&
		turn == id_)
	{
		//显示上贡按钮
		btn_contribute_->show();
		btn_contribute_->raise();
		//顶置窗口
		raise();
	}
	//还贡
	else if ((circle_type == -2 || circle_type == -1) &&
		turn == id_)
	{
		//显示还贡按钮
		btn_retribute_->show();
		btn_retribute_->raise();
		//顶置窗口
		raise();
	}
	//正常出牌
	else
	{
		//如果当前是新开的一个round
		if (circle_type == 0)
		{
			//清空上一轮的玩家排名提示
			for (int i = 0; i < 4; i++) { lb_status_[i]->set(StatusLabel::Hidden); }
			//清空已出牌
			for (int i = 0; i < 4; i++) { delete_played_cards_ui(i); }
		}
		//轮到领圈人出牌，桌面已出牌清空显示
		if (turn == circle_leader)
		{
			for (int i = 0; i < 4; i++) { delete_played_cards_ui(i); }
		}
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
			btn_contribute_->hide();
			btn_retribute_->hide();
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

	//debug: 发送卡牌已打出信号
	emit sig_card_played(all_combs_[*selected_wild_card_id], id_);
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
	delete_played_cards_ui(player_id);
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
	delete_played_cards_ui(player_id);
	spr_played_cards_[player_id] = new Sprite(x, y, "img/label/pass.png", this, Sprite::Size, 30);
	//置于图层底层，防止遮挡主牌显示
	spr_played_cards_[player_id]->lower();
	spr_played_cards_[player_id]->show();
}

void PlayerWidget::on_card_played(const std::vector<Card>& cards, int player_id)
{
	//更新桌面牌面显示
	int turn_next = turn;
	do
	{
		turn_next = (turn_next + 1) % 4;
	} while (round_rank[turn_next] != -1);

	if (turn_next != circle_leader) { update_played_cards_ui(cards, player_id); }

	int remain_cnt = players[player_id]->get_cards().size();
	//更新玩家状态情况（剩余牌数<=10）
	if (remain_cnt <= 10) { lb_status_[player_id]->set(StatusLabel::Remain_cnt, remain_cnt); }
	//已经无牌，更新该玩家状态
	if (remain_cnt == 0) { lb_status_[player_id]->set(StatusLabel::Rank, round_rank[player_id]); }
	card_played_process_count++;
	if (card_played_process_count == 4)
	{
		emit sig_global_card_played_process(cards, player_id);
	}
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

void PlayerWidget::on_conretribute()
{
	emit sig_conretributed(id_, selected_cards_[0]->get_card());
}

Hand* PlayerWidget::get_hand()const
{
	return hand_;
}

void PlayerWidget::on_card_transfered(int sender, int receiver, const Card& card)
{
	qDebug() << circle_type << "card transfered" << sender << receiver;
	//显示 sender 给出的牌
	update_played_cards_ui(std::vector<Card>({ card }), sender);
	if (id_ == receiver)
	{
		//增加卡牌
		hand_->cards_.push_back(card);
		card_heaps_.push_back(std::make_pair(false, std::vector<Card>{card}));
		//重新排序显示
		sort_card_heap(false, true);
	}
	else if (id_ = sender)
	{
		//删除卡牌
		hand_->pop_card(card);
		//debug:有风险
		for (auto& heap : card_heaps_)
		{
			auto target_t = std::find(heap.second.begin(), heap.second.end(), card);
			if (target_t != heap.second.end())
			{
				heap.second.erase(target_t);
				heap.first = false;
				break;
			}
		}
		//重新排序显示
		sort_card_heap(false, true);
	}

	//处理计数加1
	card_played_process_count++;
	if (card_played_process_count == 4)
	{
		//处理计数完成，发送完成信号
		emit sig_conretribution_processed();
	}
}
