#include "simplelabel.h"

SimpleLabel::SimpleLabel(QWidget *parent)
:QLabel(parent)
{
    simple_init();
}

SimpleLabel::~SimpleLabel()
{
    Simple_Stop();
}

void SimpleLabel::Simple_Stop()
{
	if (mTimer){
		if (mTimer->isActive()){
			mTimer->stop();
		}
		delete mTimer;
		mTimer = NULL;
	}
}

void SimpleLabel::simple_init(int ms)
{
	mTimer = NULL;
	mMs = ms;
	mContext = "";
	mPos = 0;
}

void SimpleLabel::Simple_Set_Text(const QString &context, const int &ms)
{

	mContext = context;
	mMs = ms;

	if (mMs != 0)
	{
		if (mTimer == NULL)
		{
			mTimer = new QTimer(this);

			if (mTimer)
			{
                connect(mTimer, SIGNAL(timeout()), this, SLOT(ice_time_out()));
				mTimer->start(mMs);
			}
		}
	}
}

void SimpleLabel::simple_time_out()
{
	
	QString str="";

	if (mPos > mContext.count())
	{
		mPos = 0;
	}

	str = mContext.mid(mPos);
			
	setText(str);
	mPos++;
}
