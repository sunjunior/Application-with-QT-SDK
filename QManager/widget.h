#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QDomDocument>

class QStandardItemModel;


namespace Ui
{
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    enum DateTimeType{Time, Date, DateTime};
    QString getDateTime(DateTimeType type);

private slots:
    void on_sellTypeComboBox_currentIndexChanged(QString );

    void on_sellBrandComboBox_currentIndexChanged(QString );

    void on_sellNumSpinBox_valueChanged(int );

    void on_sellCancelBtn_clicked();

    void on_sellOkBtn_clicked();

    void on_typeComboBox_currentIndexChanged(QString );

    void on_updateBtn_clicked();

    void on_manageBtn_clicked();

    void on_chartBtn_clicked();

    void on_changePwdBtn_clicked();

    void on_passwordBtn_clicked();

    void on_goodsTypeComboBox_currentIndexChanged(QString );

    void on_goodsCancelBtn_clicked();

    void on_goodsBrandComboBox_currentIndexChanged(QString );

    void on_goodsNumSpinBox_valueChanged(int );

    void on_goodsOkBtn_clicked();

    void on_newTypeComboBox_currentIndexChanged(QString );

    void on_newCancelBtn_clicked();

    void on_newNumSpinBox_valueChanged(int );


    void on_newPriceSpinBox_valueChanged(int );



    void on_newBrandLineEdit_textChanged(QString );

    void on_newOkBtn_clicked();

private:
    Ui::Widget *ui;
    QDomDocument doc;

    QStandardItemModel *chartModel;

private:
    bool docRead();
    bool docWrite();
    void writeXml();
    void createNodes(QDomElement &date);
    void showDailyList();

    void createChartModelView();
    void showChart();
};

#endif // WIDGET_H
