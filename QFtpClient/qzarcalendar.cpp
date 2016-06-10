#include <QtGui>
#include "qzarcalendar.h"

QZarCalendar::QZarCalendar(QWidget *parent) :
    QWidget(parent)
{
    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(1970, 1, 1));
    calendar->setMaximumDate(QDate(2100, 1, 1));
    calendar->setGridVisible(true);

    previewLayout = new QGridLayout;
    previewLayout->addWidget(calendar, 0, 0, Qt::AlignCenter);


    currentDateLabel = new QLabel(tr("版本日期"));
    currentDateLabel->setBuddy(currentDateEdit);

    currentDateEdit = new QDateEdit;
    currentDateEdit->setDisplayFormat("yyyy-MM-dd");
    currentDateEdit->setDate(calendar->selectedDate());
    currentDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());


    connect(currentDateEdit, SIGNAL(dateChanged(const QDate &)),
            calendar, SLOT(setSelectedDate(const QDate &)));
    connect(calendar, SIGNAL(selectionChanged()),
            this, SLOT(selectedDateChanged()));

    QGridLayout *dateBoxLayout = new QGridLayout;
    dateBoxLayout->addWidget(currentDateLabel, 1, 0);
    dateBoxLayout->addWidget(currentDateEdit, 1, 1);
    dateBoxLayout->setRowStretch(3, 1);


    QGridLayout *layout = new QGridLayout;
    layout->addLayout(previewLayout, 0, 0);
    layout->addLayout(dateBoxLayout, 1, 0);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    previewLayout->setRowMinimumHeight(0, calendar->sizeHint().height());
    previewLayout->setColumnMinimumWidth(0, calendar->sizeHint().width());

    setWindowTitle(tr("日期选择"));
    setWindowFlags(windowFlags() & ~(Qt::WindowMinMaxButtonsHint | Qt::FramelessWindowHint));
}


void QZarCalendar::selectedDateChanged()
{
    currentDateEdit->setDate(calendar->selectedDate());
}










