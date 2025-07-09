#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QVBoxLayout>
#include <QMouseEvent>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    form = new Form(this);
//    form = new Form();

//    connect(form, &Form::backClicked, [=] {
//        form->hide();

//        this->show();
//    });
    // 给 widget03 设置布局
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(ui->fluLabel, 1);
    layout->addWidget(ui->widgetMidNotice, 5);
    ui->widgetMid->setLayout(layout);

    // 设置无边框
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(width(), height());

    // 构建右键菜单
    mExitMenu = new QMenu(this);
    mExitAct = new QAction(this);
    mExitAct->setText("退出");
    mExitAct->setIcon(QIcon(":/res/close.ico"));
    mExitMenu->addAction(mExitAct);


    mExitMenu->setStyleSheet(R"(
        QMenu {
            background-color: #000000;     /* 菜单背景：黑色 */
            color: white;                  /* 默认文字颜色：白色 */
        }
        QMenu::item:selected {
            background-color: #444444;     /* 鼠标悬停时背景 */
            color: white;                  /* 悬停时文字颜色 */
        }
    )");



    connect(mExitAct, &QAction::triggered, this, [=] {
        qApp->exit(0);
    });

    //将控件添加到控件数组
    mWeekList<<ui->labelWeek0<<ui->labelWeek1<<ui->labelWeek2<<ui->labelWeek3<<ui->labelWeek4<<ui->labelWeek5;



    //网络请求
    mNetAccessManager = new QNetworkAccessManager(this);
    connect(mNetAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::onReplied);

    //直接在构造函数中，请求天气数据
    //"101010100"是北京的城市编码
    getWeatherInfo("101010100");

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 右键菜单事件
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    mExitMenu->exec(QCursor::pos());
    event->accept();
}

// 👇👇👇 添加以下鼠标拖动相关函数 👇👇👇

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
    }
}


void MainWindow::getWeatherInfo(QString cityCode)
{
    mNetAccessManager->get(QNetworkRequest(QUrl("http://t.weather.itboy.net/api/weather/city/" + cityCode)));
}

void MainWindow::parseJson(QByteArray &byteArray)
{
     QJsonParseError err;
     QJsonDocument doc = QJsonDocument::fromJson(byteArray,&err);
     if(err.error!=QJsonParseError::NoError)
     {
        qDebug()<<"parseJson error:"<<err.errorString();
        return;
     }
     QJsonObject rootObj = doc.object();
     qDebug()<<rootObj.value("message").toString();


     //1.解析日期和城市
     mToday.date = rootObj.value("date").toString();
     mToday.city = rootObj.value("cityInfo").toObject().value("city").toString();

     //2.解析yesterday
     QJsonObject objData = rootObj.value("data").toObject();

     QJsonObject objYesterday = objData.value("yesterday").toObject();
     mDay[0].week = objYesterday.value("week").toString();
     mDay[0].date = objYesterday.value("ymd").toString();
     mDay[0].type = objYesterday.value("type").toString();

     QString s;
     s = objYesterday.value("high").toString().split(" ").at(1);
     s = s.left(s.length()-1);
     mDay[0].high = s.toInt();

     s = objYesterday.value("low").toString().split(" ").at(1);
     s = s.left(s.length()-1);
     mDay[0].low = s.toInt();


     //风向风力
     mDay[0].fx = objYesterday.value("fx").toString();
     mDay[0].fl = objYesterday.value("fl").toString();

     //污染指数
     mDay[0].aqi = objYesterday.value("aqi").toDouble();

     //3.解析forcast中5天的数据
     QJsonArray forecastArray = objData.value("forecast").toArray();


     for(int i=0;i<5;i++)
     {
        QJsonObject objForecast = forecastArray[i].toObject();
        //0已经设为昨天了，从i+1开始
        mDay[i+1].week = objForecast.value("week").toString();
        mDay[i+1].date = objForecast.value("ymd").toString();
        mDay[i+1].type = objForecast.value("type").toString();

        QString s;
        s = objForecast.value("high").toString().split(" ").at(1);
        s = s.left(s.length()-1);
        mDay[i+1].high = s.toInt();

        s = objForecast.value("low").toString().split(" ").at(1);
        s = s.left(s.length()-1);
        mDay[i+1].low = s.toInt();

        //风向风力
        mDay[i+1].fx = objForecast.value("fx").toString();
        mDay[i+1].fl = objForecast.value("fl").toString();

        //污染指数
        mDay[i+1].aqi = objForecast.value("aqi").toDouble();



     }
     //4.解析今天的数据
        mToday.ganmao = objData.value("ganmao").toString();
        mToday.wendu = objData.value("wendu").toInt();
        mToday.shidu = objData.value("shidu").toString();
        mToday.pm25 = objData.value("pm25").toDouble();
        mToday.quality = objData.value("quality").toString();

    //5.forecast中第一个数组元素，也是今天的数据
        mToday.type = mDay[1].type;
        mToday.high = mDay[1].high;
        mToday.low = mDay[1].low;
        mToday.fx = mDay[1].fx;
        mToday.fl = mDay[1].fl;
        mToday.aqi = mDay[1].aqi;


     //6.更新UI
}

void MainWindow::onReplied(QNetworkReply *reply)
{
    qDebug()<<"onReplied success";
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    // qDebug()<<"operation:"<<reply->operation();
    // qDebug()<<"status_code:"<<status_code;
    if(reply->error() == QNetworkReply::NoError || status_code == 200)
    {
        qDebug()<<"readAll success";
        QByteArray byteArray = reply->readAll();
        // qDebug()<<byteArray.data();
        parseJson(byteArray);
    }
    else
    {
        QMessageBox::warning(this, "错误", "获取天气数据失败",QMessageBox::Ok);
    }
    reply->deleteLater();

}

void MainWindow::on_pushButton_clicked()
{

    this->hide();
       form->show();
}
