#include "sprite.h"
#include <qwidget.h>

Sprite::Sprite(int x, int y, QPixmap img, QWidget* parent, SizeMode mode, double size)
	:QPushButton(parent)
{
	show();
	//扁平化按钮
	setStyleSheet("QPushButton{border:none;}");
	setFlat(true);

	//设置按钮图片
	auto pixmap = QPixmap(img);
	setIcon(pixmap);
	//调整按钮图片大小
	double ratio = 1.0 * pixmap.size().width() / pixmap.size().height();
	switch (mode)
	{
	case Sprite::Size:
		w_ = pixmap.size().width() * size / 100.0;
		h_ = w_ / ratio;
		break;
	case Sprite::Width:
		w_ = size;
		h_ = w_ / ratio;
		break;
	case Sprite::Height:
		h_ = size;
		w_ = ratio * h_;
		break;
	default:
		break;
	}
	setIconSize(QSize(w_, h_));
	setGeometry(x, y, w_, h_);
};

//Sprite::set_pm(QString img)
//{
//	setIcon(QPixmap(img));
//}



