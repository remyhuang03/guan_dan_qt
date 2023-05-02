#ifndef BUTTON_H_
#define BUTTON_H_
#include "sprite.h"

//自定义Button类
//含 clicked 动画
class Button :public Sprite
{
	Q_OBJECT
public:
	//按钮模式
	enum Mode { Disabled = -1, Normal, Mode2 };

	//@para
	//img:图片路径
	//parent:父窗口
	//size:图片大小，100为原始大小
	Button(int x, int y, QString img, QWidget* parent, SizeMode mode, double size);

	//@brief 将按钮设置为不可点击状态
	void disable();
	//@brief 将按钮设置为可点击状态
	void enable();
	//@brief 切换按钮模式
	void reverse_mode();

	//@brief 设置按钮模式图标
	void set_pm(QString img, Mode mode);

	//@brief 禁用按钮动画
	void disable_animation() { animation_disabled_ = true; }


public slots:
	//@brief 点击动画
    void click_animation();

	//@brief 显示牌局记录
	void show_record();

protected:
	//处于可点击状态时的默认图标
	QPixmap pm_normal_;
	//处于不可点击状态时的图标
	QPixmap pm_disabled_;
	//处于第二模式时的图标
	QPixmap pm_mode2_;

	//按钮模式（-1:disabled   0:normal   1:mode2）
	Mode mode_;

	//禁用按钮动画
	bool animation_disabled_;

signals:
	void click_emit(int mode=0);

};

#endif // BUTTON_H_
