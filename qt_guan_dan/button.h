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

	//@brief 切换按钮模式
	void reverse_mode();

	//@brief 设置按钮模式图标
	void set_pm(QString img, Mode mode);
	//@brief 设置模式
	void set_mode(Mode mode);

	//@brief 禁用按钮动画
	void disable_animation() { animation_disabled_ = true; }


public slots:
	//@brief 点击动画
    void click_animation();

	//@brief 显示牌局记录
	void show_record();

protected:
	//图标[mode+1]：不可点击状态、默认、第二模式
	QPixmap icons[3];

	//按钮模式（-1:disabled   0:normal   1:mode2）
	Mode mode_;

	//禁用按钮动画
	bool animation_disabled_;

private:
	int x_;
	int y_;

signals:
	void click_emit(int mode=0);

};

#endif // BUTTON_H_
