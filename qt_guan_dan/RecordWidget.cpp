#include "RecordWidget.h"
#include "record.h"
#include "status.h"
#include <qheaderview.h>
#include <qtablewidget.h>

Record_widget::Record_widget()
{
	/*****窗口基本设置****/
	show();
	//设置标题
	setWindowTitle("牌局记录");
	//背景色深灰色
	setPalette(QColor(35, 35, 35));
	//设置窗口大小
	setFixedSize(255, 300);

	/*****基本UI布局设置****/
	auto record = game_record.get_record();
	auto table = new QTableWidget(record.size() + 1, 4, this);
	//表格字体大小
	table->setFont(QFont("微软雅黑", 10));
	//设置每一行的表头
	table->setHorizontalHeaderLabels(QStringList() << "头游" << "二游" << "三游" << "下游");
	//所有单元格都不允许编辑
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置表格内容
	for (int i = 0; i < record.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			auto item = new QTableWidgetItem("玩家" + QString::number(record[i][j] + 1));
			table->setItem(i, j, item);
		}
	}

	//解决 win11 dark theme 下的显示颜色问题
	table->horizontalHeader()->setStyleSheet("QHeaderView::section{font:black;}");
	table->verticalHeader()->setStyleSheet("QHeaderView::section{font:black;}");

	//大小自适应
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	
	table->show();
}