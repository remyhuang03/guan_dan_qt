#include "sprite.h"
#include <qwidget.h>

Sprite::Sprite(int x, int y, QString img, QWidget* parent, SizeMode mode, double size)
{
	//扁平化按钮
	setStyleSheet("QPushButton{border:none;}");
	setFlat(true);

	//设置按钮图片
	auto pixmap = QPixmap(img);
	setIcon(pixmap);
	//调整按钮图片大小
	double ratio = 1.0 * pixmap.size().width() / pixmap.size().height();
	double w, h;
	switch (mode)
	{
	case Sprite::Size:
		w = pixmap.size().width() * size / 100.0;
		h = w / ratio;
		break;
	case Sprite::Width:
		w = size;
		h = w / ratio;
		break;
	case Sprite::Height:
		h = size;
		w = ratio * h;
		break;
	default:
		break;
	}
	setIconSize(QSize(w, h));
	setGeometry(x, y, w, h);

	//设置父窗口
	setParent(parent);

	//链接按钮点击动画
	connect(this, &QPushButton::clicked, this, &Sprite::zoom);
};

