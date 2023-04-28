#ifndef SPRITE_H_
#define SPRITE_H_
#include <qpushbutton.h>
#include <qimage.h>

//自定义UI类
class Sprite :public QPushButton
{
public:
	enum SizeMode { Size, Width, Height };
	//@para
	//img:图片路径
	//parent:父窗口
	//size:图片大小，100为原始大小
	Sprite(int x, int y, QString img, QWidget* parent, SizeMode mode = Size, double size = 100);
};
#endif // BUTTON_H_

