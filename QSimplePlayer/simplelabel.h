#ifndef SIMPLELABEL_H
#define SIMPLELABEL_H
#include <QLabel>
#include <QTimer>

class SimpleLabel : public QLabel
{
	Q_OBJECT
public:
    explicit SimpleLabel(QWidget *parent=0);
    ~SimpleLabel();

	//设置文本:
	//如果ms为0则文字不滚动(默认不滚动);
	//如果调用setText()文字将不滚动(与使用QLabel相同);
    void Simple_Set_Text(const QString &context=0, const int &ms=0);

	//停止滚动
    void Simple_Stop();

private slots:
	
    void simple_time_out();

private:

    void simple_init(int ms = 0);

	QString mContext;
	QTimer *mTimer;
	
	int mMs;
	int mPos;
};

#endif
