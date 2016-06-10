#include "simpleaboutform.h"


Simple_About_Form::Simple_About_Form(QWidget *parent /* = 0 */)
{
    Q_UNUSED(parent);

    exitButton = new Simple_Music_Button(this);
	exitButton->setObjectName(QStringLiteral("exitButton"));
	exitButton->setGeometry(QRect(255, 12, 30, 30));
	QIcon icon_exit, icon_exit_focus;
    icon_exit.addFile(QStringLiteral(":/SimplePlayer/Resources/关闭按钮.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon_exit_focus.addFile(QStringLiteral(":/SimplePlayer/Resources/关闭按钮2.png"), QSize(), QIcon::Normal, QIcon::Off);
    exitButton->Simple_Set_Button_Icons(icon_exit, icon_exit_focus);

	exitButton->setIconSize(QSize(20,20));
	exitButton->setFlat(true);
	exitButton->setFocusPolicy(Qt::NoFocus);
	exitButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
	
	//去除标题栏
	this->setWindowFlags(Qt::FramelessWindowHint);
	
	//设置窗体背景色
	/*QColor background(217, 227, 236);

	QPalette win_palette(this->palette());
	win_palette.setColor(QPalette::Background, background);
	this->setPalette(win_palette);*/

	//设置窗体背景透明，配合paintEvent设置不规则背景图
	setAttribute(Qt::WA_TranslucentBackground);

	//半透明
	this->setWindowOpacity(0.9);

	//固定大小
	this->setFixedSize(300, 400);

	//设置本窗口为模式窗口（即弹出后无法点击主界面）
	this->setModal(true);


}

Simple_About_Form::~Simple_About_Form()
{

}

void Simple_About_Form::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(0, 0 , QPixmap(":/SimplePlayer/Resources/about.png"));
}



void Simple_About_Form::mousePressEvent(QMouseEvent *event)
{
    this->dPos =  event->globalPos() - this->pos();
}

void Simple_About_Form::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()- this->dPos);
}

