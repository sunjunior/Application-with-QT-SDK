#ifndef SIMPLE_ABOUT_H
#define SIMPLE_ABOUT_H

#include <QDialog>
#include "simplemusicbutton.h"

class Simple_About_Form : public QDialog
{
	Q_OBJECT
public:
    Simple_About_Form(QWidget *parent = 0);
    ~Simple_About_Form();

private:

	//绘制背景图片
	void paintEvent(QPaintEvent *event); 
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    Simple_Music_Button *exitButton;
    QPoint dPos;
};


#endif
