#ifndef SIMPLE_MUSIC_BUTTON_H
#define SIMPLE_MUSIC_BUTTON_H

#include <QtGui> 
#include <QPushButton>


class Simple_Music_Button : public QPushButton
{  
	Q_OBJECT  
private:  
	QIcon normalIcon;  
	QIcon focusIcon;  
	QIcon pressedIcon;  
protected:  

	//重载“进入”和“离开”事件，进入时切换为另一个图标，离开时还原 
	void enterEvent(QEvent *event);  
	void leaveEvent(QEvent *event);  

	//重载鼠标按下和松开事件 ，切换图标
	void mousePressEvent(QMouseEvent *event);  
	void mouseReleaseEvent(QMouseEvent *event);  
public:
    Simple_Music_Button(QWidget *parent = 0);
    ~Simple_Music_Button();

	//设置图标
	//若只指定一个图标，则默认“进入”，按下，松开都为此图标
    void Simple_Set_Button_Icons(const QIcon &normal, const QIcon &focus = QIcon(), const QIcon &pressed = QIcon() );
};  


#endif
