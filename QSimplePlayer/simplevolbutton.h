#ifndef SIMPLEVOLBUTTON_H
#define SIMPLEVOLBUTTON_H

#include "simplemusicbutton.h"
#include <QtWidgets>

class Simple_Vol_Button : public QWidget
{
	Q_OBJECT
    Q_PROPERTY(int Simple_Get_Volume READ Simple_Get_Volume WRITE Simple_Set_Volume NOTIFY Simple_Volume_Changed)

public:
    Simple_Vol_Button(QWidget *parent = 0);

	//返回音量值（0~100）
    int Simple_Get_Volume() const;

	//设置音量条是否可见
    void Simple_Set_Slider_Visiable(bool);

public slots:

	//点击音量条使音量增加一部分	
    void Simple_Increase_Volume();

	//点击音量条使音量减少一部分
    void Simple_Descrease_Volume();

	//设置音量
    void Simple_Set_Volume(int volume);

	//按钮单击事件，弹出菜单
    void Simple_Button_Clicked();

signals:

	//音量改变信号（当音量条被拖动时发射）
    void Simple_Volume_Changed(int volume);

private:
    Simple_Music_Button *button;
	QSlider *slider;
	QMenu *menu;
	QWidgetAction *action;
};

#endif 
