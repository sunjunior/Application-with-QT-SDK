#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pwdLineEdit->setFocus();
    ui->loginBtn->setDefault(true);
    setWindowTitle(tr("登录"));
    setFixedSize(480, 800);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

// 登录按钮
void LoginDialog::on_loginBtn_clicked()
{
    if (ui->usrLineEdit->text().isEmpty()
        ||ui->pwdLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, tr("登录提示"),
                                 tr("账号或密码为空，请重新输入后再登录！"), QMessageBox::Ok);
        ui->usrLineEdit->setFocus();
    }
    else
    {
        QSqlQuery query;
        query.exec("select usrname,password from login");
        query.next();
        if (query.value(0).toString() == ui->usrLineEdit->text()
                && query.value(1).toString() == ui->pwdLineEdit->text())
        {
            QDialog::accept();
        }
        else
        {
            QMessageBox::warning(this, tr("账号或密码错误"),
                                 tr("请重新输入之后再登录！"), QMessageBox::Ok);

            ui->pwdLineEdit->clear();
            ui->usrLineEdit->setFocus();
        }
    }
}

// 退出按钮
void LoginDialog::on_quitBtn_clicked()
{
    QDialog::reject();
}
