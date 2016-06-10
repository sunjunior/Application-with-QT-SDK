#include <QtWidgets>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QTextCodec>
#include <QDebug>
#include <QTimer>

#include "widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),output("")
{
    typeLabel = new QLabel("信息类别:");
    queryBox = new QComboBox;
    queryBox->addItem("IP地址");
    queryBox->addItem("身份证信息");
    queryBox->addItem("手机号码归属地");
    queryBox->addItem("单词翻译");
    queryBox->addItem("今日天气");
    queryBox->addItem("空气质量指数");
    queryBox->addItem("智能搜索");
    lineEdit = new QLineEdit;

    queryBtn = new QPushButton("查询");

    textEdit = new QTextEdit;
    textEdit->setReadOnly(true);

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(typeLabel);
    hlayout1->addStretch(1);
    hlayout1->addWidget(queryBox, 8, Qt::AlignBaseline);


    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(lineEdit, 8, Qt::AlignBaseline);
    hlayout2->addSpacing(1);
    hlayout2->addWidget(queryBtn);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    vlayout->addWidget(textEdit);
    setLayout(vlayout);

    connect(queryBtn, SIGNAL(clicked(bool)), this, SLOT(onClickedFinished()));

    manager = new QNetworkAccessManager;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReplyFinished(QNetworkReply*)));
    connect(this, SIGNAL(finished(bool,QString,QString)), this, SLOT(onQueryFinished(bool,QString,QString)));

    setWindowTitle("生活一指通");
}

Widget::~Widget()
{
    delete manager;
}

//void Widget::closeEvent(QCloseEvent *event)
//{
//    switch( QMessageBox::information( this, tr("关闭提示"),
//                                      tr("确认退出生活一指通？"),tr("取消"), tr("确定"),0, 1))
//    {
//    case 0:
//        event->ignore();

//        break;
//    case 1:
//        event->accept();
//        break;
//    default:
//        break;
//    }
//}

void Widget::query(const QString &content)
{
    int index = queryBox->currentIndex();
    QString strUrl = "";
    switch (index)
    {
    case 0:
    {
        strUrl = "http://apis.baidu.com/apistore/";
        QRegExp regExp("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$");
        if (-1 == content.indexOf(regExp))
        {
            QMessageBox::critical(this, "IP合法性检测", "输入的IP地址不合法，请重新输入！", "确定");
            return;
        }
        strUrl += QString("iplookupservice/iplookup?ip=%1").arg(content);
        break;
    }
    case 1:
    {
        strUrl = "http://apis.baidu.com/apistore/";
        QRegExp regExp("^[1-9]\\d{5}[1-9]\\d{3}((0\\d)|(1[0-2]))(([0|1|2]\\d)|3[0-1])\\d{4}$");
        if (-1 == content.indexOf(regExp))
        {
            QMessageBox::critical(this, "身份证合法性检测", "输入的身份证不合法，请重新输入！", "确定");
            return;
        }
        strUrl += QString("/idservice/id?id=%1").arg(content);
        break;
    }

    case 2:
    {
        strUrl = "http://apis.baidu.com/apistore/";
        QRegExp regExp("^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$");
        if (-1 == content.indexOf(regExp))
        {
            QMessageBox::critical(this, "号码合法性检测", "输入的号码不合法，请重新输入！", "确定");
            return;
        }

        strUrl += QString("/mobilephoneservice/mobilephone?tel=%1").arg(content);
        break;
    }

    case 3:
    {
        strUrl = "http://apis.baidu.com/apistore/";
        QRegExp regExp("^[A-Za-z]+$");
        if (-1 == content.indexOf(regExp))
        {
            QMessageBox::critical(this, "单词合法性检测", "输入的单词不合法，请重新输入！", "确定");
            return;
        }

        strUrl += QString("/tranlateservice/dictionary?query=%1&from=en&to=zh").arg(content);
        break;
    }

    case 4:
    {
        strUrl = "http://apis.baidu.com/apistore/";
        strUrl += QString("/weatherservice/cityname?cityname=%1").arg(content);
        break;
    }


    case 5:
    {
        strUrl = "http://apis.baidu.com/apistore/";
        QFile file(":/resources/citylist");
        if (!file.open(QFile::ReadOnly))
        {
            return;
        }

        bool isIncludeFlag = false;

        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            if (!line.trimmed().isEmpty())
            {
                QString city =  line.trimmed();
                if (QString::compare(city, content) == 0)
                {
                    isIncludeFlag = true;
                    file.close();
                    break;
                }
            }

        }

        if (false == isIncludeFlag)
        {
            QMessageBox::information(this, "输入提示", "输入的城市暂时没有相应的信息！", "确定");
            return;
        }

        strUrl += QString("/aqiservice/aqi?city=%1").arg(content);
        break;
    }


    }
    queryBox->setEnabled(false);
    queryBtn->setEnabled(false);
    lineEdit->setEnabled(false);
    QTimer::singleShot(8000, this, SLOT(onTimerProc())); //timeout
    QUrl url(strUrl);
    QNetworkRequest req(url);
    req.setRawHeader("apikey", "c3765c678ec00b7985ff93af87198ae2");
    req.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Fedora 18; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36");
    reply = manager->get(req);

    reply->setProperty("content", content);
}


QString Widget::getInfo(const QString &content)
{
    QString strRet(content);

    if(strRet.trimmed().isEmpty() || strRet.trimmed() == "None")
    {
        strRet = "未知";
    }

    return strRet;
}

void Widget::onQueryFinished(bool bOK, QString content, QString area)
{
    Q_UNUSED(content);

    if (bOK)
    {
        textEdit->setText(area);
    }
    else
    {
        textEdit->setText("糟糕，网络连接异常，请重试！");
    }
    lineEdit->setEnabled(true);
    queryBtn->setEnabled(true);
    queryBox->setEnabled(true);
}

void Widget::readAllData()
{
    if (reply)
    {
        qDebug() << "1"<< reply->readAll() << endl;
    }
}

void Widget::onReplyFinished(QNetworkReply *reply)
{
    QString strContent = reply->property("content").toString();
    connect(reply, SIGNAL(readyRead()), this, SLOT(readAllData()));
    if (reply->error() != QNetworkReply::NoError)
    {
        emit finished(false, strContent, "");
        return;
    }

    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (status != 200) //200 is ok
    {
        emit finished(false, strContent, "");
        return;
    }

    QByteArray data = reply->readAll();
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();

    int charsetIndex = contentType.indexOf("charset=");
    if (charsetIndex > 0)
    {
        charsetIndex += 8;
        QString charset = contentType.mid(charsetIndex).trimmed().toLower();
        if (charset.startsWith("gbk") || charset.startsWith("gb2312"))
        {
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            if (codec)
            {
                data = codec->toUnicode(data).toUtf8();
            }
        }
    }
//    //if the data contains parenthesis
//    int parenthesisLeft = data.indexOf('(');
//    int parenthesisRight = data.lastIndexOf(')');
//    if(parenthesisLeft >=0 && parenthesisRight >=0)
//    {
//        parenthesisLeft++;
//        data = data.mid(parenthesisLeft, parenthesisRight - parenthesisLeft);
//    }

    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(data, &err);
    if(err.error != QJsonParseError::NoError)
    {
        emit finished(false, strContent, err.errorString());
        return;
    }

    QJsonObject obj = json.object();

    int index = queryBox->currentIndex();
    switch (index)
    {
    case 0:
    {
        QJsonObject::const_iterator it = obj.find("retData");
        if(it != obj.constEnd())
        {
            QJsonObject info = it.value().toObject();
            QString country =  info.find("country").value().toString();
            QString province = info.find("province").value().toString();
            QString city = info.find("city").value().toString();
            QString district = info.find("district").value().toString();
            QString carrier = info.find("carrier").value().toString();

            country = getInfo(country);
            province = getInfo(province);
            city = getInfo(city);
            district = getInfo(district);
            carrier = getInfo(carrier);

            QString area = "国家：" + country  + "\n"
                    + "省：" + province +  "\n"
                    + "市：" + city + "\n"
                    + "区：" + district + "\n"
                    + "运营商：" + carrier;
            emit finished(true, strContent, area);
        }
        break;
    }
    case 1:
    {
        QJsonObject::const_iterator it = obj.find("retData");
        if(it != obj.constEnd())
        {
            QJsonObject info = it.value().toObject();
            QString address =  info.find("address").value().toString();
            QString sex = info.find("sex").value().toString();
            QString birthday = info.find("birthday").value().toString();

            if (QString::compare(sex, "M", Qt::CaseInsensitive) == 0)
            {
                sex = "男性";
            }
            else
            {
                sex = "女性";
            }

            address = getInfo(address);
            sex = getInfo(sex);
            birthday = getInfo(birthday);

            QString area = "地址："+ address  + "\n"
                    + "性别：" + sex + "\n"
                    + "出生日期：" + birthday;
            emit finished(true, strContent, area);
        }

        break;
    }

    case 2:
    {
        QJsonObject::const_iterator it = obj.find("retData");
        if(it != obj.constEnd())
        {
            QJsonObject info = it.value().toObject();
            QString province =  info.find("province").value().toString();
            QString carrier =  info.find("carrier").value().toString();

            province = getInfo(province);
            carrier = getInfo(carrier);
            QString area = "归属地：" + province +  "\n"
                    + "运营商：" + carrier;
            emit finished(true, strContent, area);
        }
        break;
    }

    case 3:
    {
        QJsonObject::const_iterator it = obj.find("retData");
        if(it != obj.constEnd())
        {
            QJsonObject info = it.value().toObject();
            QJsonObject dict_result =  info.find("dict_result").value().toObject();
            QJsonArray symbols =  dict_result.find("symbols").value().toArray();
            QJsonObject content = symbols.first().toObject();
            QString amPro = content.find("ph_am").value().toString();
            QString enPro = content.find("ph_en").value().toString();

            QJsonArray parts = content.find("parts").value().toArray();
            QString wordexplain = "";
            for (QJsonArray::iterator iter = parts.begin(); iter != parts.end(); ++iter)
            {
                QJsonObject wordmean = (*iter).toObject();
                wordexplain += wordmean.find("part").value().toString();
                wordexplain += "  ";
                QJsonArray means = wordmean.find("means").value().toArray();
                for (QJsonArray::iterator subiter = means.begin(); subiter != means.end(); ++subiter)
                {
                    wordexplain += (*subiter).toString();
                    wordexplain += "; ";
                }
                wordexplain += "\n";
            }

            wordexplain = getInfo(wordexplain);
            amPro = getInfo(amPro);
            enPro = getInfo(enPro);

            QString area = "美式：[" + amPro +  "]\n"
                    + "英式：[" + enPro + "]\n"
                    + wordexplain;
            emit finished(true, strContent, area);
        }

        break;
    }

    case 4:
    {
        QJsonObject::const_iterator it = obj.find("retData");
        if(it != obj.constEnd())
        {
            QJsonObject info = it.value().toObject();
            QString city =  info.find("city").value().toString();
            QString date =  info.find("date").value().toString();
            QString time =  info.find("time").value().toString();
            QString weather =  info.find("weather").value().toString();
            QString l_tmp =  info.find("l_tmp").value().toString();
            QString h_tmp =  info.find("h_tmp").value().toString();
            QString WD =  info.find("WD").value().toString();
            QString WS =  info.find("WS").value().toString();
            city = getInfo(city);
            date = getInfo(date);
            time = getInfo(time);
            weather = getInfo(weather);
            l_tmp = getInfo(l_tmp);
            h_tmp = getInfo(h_tmp);
            WD = getInfo(WD);
            WS = getInfo(WS);
            QString area = "城市：" + city +  "\n"
                    + QString("数据采集时间：20%1 %2").arg(date).arg(time)  + "\n"
                    + "天气情况：" + weather + "\n"
                    + "最低气温：" + l_tmp + "℃\n"
                    + "最高气温：" + h_tmp + "℃\n"
                    + "风向情况：" + WD + "\n"
                    + "风力情况：" + WS;
            emit finished(true, strContent, area);
        }
        break;
    }

    case 5:
    {
        QJsonObject::const_iterator it = obj.find("retData");
        if(it != obj.constEnd())
        {
            QJsonObject info = it.value().toObject();
            QString city =  info.find("city").value().toString();
            QString time =  info.find("time").value().toString();
            qint32 aqi =  info.find("aqi").value().toInt();
            QString level =  info.find("level").value().toString();
            QString core =  info.find("core").value().toString();
            city = getInfo(city);
            time = getInfo(time);
            level = getInfo(level);
            core = getInfo(core);

            QString area = "城市：" + city +  "\n"
                    + "数据采集时间：" + time + "\n"
                    + "空气质量指数：" + QString::number(aqi) + "\n"
                    + "空气等级：" + level + "\n"
                    + "首要污染物：" + core;
            emit finished(true, strContent, area);
        }
        break;
    }

    }
}

void Widget::onTimerProc()
{
    if (!reply->isFinished())
    {
        textEdit->setText("查询超时，请稍后再试！");

        queryBtn->setEnabled(true);
        lineEdit->setEnabled(true);
        queryBox->setEnabled(true);
        reply->abort();
    }

}

void Widget::onClickedFinished()
{
    QString content = lineEdit->text();
    if (!content.trimmed().isEmpty())
    {
        content = content.trimmed();
        this->query(content);
    }
}
