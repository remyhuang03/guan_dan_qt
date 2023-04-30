#ifndef BUTTON_H_
#define BUTTON_H_
#include "sprite.h"

//自定义Button类
//含 clicked 动画
class Button :public Sprite
{
	Q_OBJECT
public:
	//@para
	//img:图片路径
	//parent:父窗口
	//size:图片大小，100为原始大小
	Button(int x, int y, QString img, QWidget* parent, SizeMode mode, double size);

protected slots:
	//@brief 点击动画
	void click_animation();

signals:
	void click_emit();

};

#endif // BUTTON_H_
