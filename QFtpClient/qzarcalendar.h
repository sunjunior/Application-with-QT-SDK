#ifndef QZARCALENDAR_H
#define QZARCALENDAR_H

#include <QWidget>

class QCalendarWidget;
class QCheckBox;
class QComboBox;
class QDate;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QLabel;

class QZarCalendar : public QWidget
{
    Q_OBJECT
public:
    explicit QZarCalendar(QWidget *parent = 0);
    //QString getCurrentDate(void);

private slots:

    void selectedDateChanged();
private:

    QGridLayout *previewLayout;
    QCalendarWidget *calendar;
    QLabel *currentDateLabel;
    QDateEdit *currentDateEdit;




};

#endif // QZARCALENDAR_H
