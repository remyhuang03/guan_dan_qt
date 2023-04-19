#include "guan_dan.h"
#include <qpalette.h>

guan_dan::guan_dan(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle("掼蛋");
	//16：9 分辨率显示
	setFixedSize(960, 540);
	setPalette(QColor(35,35,35));
}

guan_dan::~guan_dan()
{}
