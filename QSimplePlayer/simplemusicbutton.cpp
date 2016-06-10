#include "simplemusicbutton.h"

Simple_Music_Button::Simple_Music_Button(QWidget *parent)
	: QPushButton(parent)  
{  
	//setDown(false);  

	//设置图标无焦点
	setFocusPolicy(Qt::NoFocus);  

	//QWidgetResizeHandler *movewin = new QWidgetResizeHandler(this);
	//movewin->setMovingEnabled(true);
}  

Simple_Music_Button::~Simple_Music_Button()
{  

}  

void Simple_Music_Button::enterEvent(QEvent *event)
{  
    Q_UNUSED(event);
    if( isEnabled() && !focusIcon.isNull() )
		setIcon(focusIcon);  
}  

void Simple_Music_Button::leaveEvent(QEvent *event)
{  
    Q_UNUSED(event);
    if( isEnabled() )
		setIcon(normalIcon);  
}  


void Simple_Music_Button::mousePressEvent(QMouseEvent *event)
{  
    Q_UNUSED(event);
    if( isEnabled() && !pressedIcon.isNull() )
		setIcon(pressedIcon);  
	QPushButton::mousePressEvent(event);  
}  

void Simple_Music_Button::mouseReleaseEvent(QMouseEvent *event)
{  
	if( isEnabled() && focusIcon.isNull() )  
		setIcon(focusIcon);  
	QPushButton::mouseReleaseEvent(event);  
}  

void Simple_Music_Button::Simple_Set_Button_Icons(const QIcon &normal, const QIcon &focus, const QIcon &pressed)
{  
	normalIcon = normal;  
	focusIcon = focus;  
	pressedIcon = pressed;  
	setIcon(normalIcon);  
} 
