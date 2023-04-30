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
	void disable();
	void enable();
	void reverse_mode();
	enum Mode { Disabled = -1, Normal, Mode2 };
	void set_pm(QString img, Mode mode);

protected slots:
	//@brief 点击动画
	void click_animation();

private:
	//处于可点击状态时的默认图标
	QPixmap pm_normal_;
	//处于不可点击状态时的图标
	QPixmap pm_disabled_;
	//处于第二模式时的图标
	QPixmap pm_mode2_;
	//按钮模式（-1:disabled 0:normal 1:mode2）
	int mode_;

signals:
	void click_emit();

};

#endif // BUTTON_H_
