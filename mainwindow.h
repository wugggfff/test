#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QContextMenuEvent>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonDocument>
#include"weatherData.h"
#include<QLabel>
#include<QList>
#include"form.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenu* mExitMenu; //右键退出的菜单
    QAction* mExitAct; //退出的行为 -菜单项
    Form* form = nullptr;
protected:
    bool m_dragging = false;          // 是否正在拖拽
    QPoint m_dragPosition;           // 鼠标按下时相对窗口的位置

    //鼠标左键点住拖拽界面功能添加
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    //获取天气预报数据
    void getWeatherInfo(QString cityCode);


    void parseJson(QByteArray &byteArray);

    void upDateUI();

private slots:
    void onReplied(QNetworkReply* reply);
    void on_pushButton_clicked();

private:
    //HTTP请求
    QNetworkAccessManager* mNetAccessManager;
    Today mToday;
    Day mDay[6];

    //星期和日期
    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    //天气和天气图标
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;

    //天气污染指数
    QList<QLabel*> mAqiList;

    //风力和风向
    QList<QLabel*> mFxList;
    QList<QLabel*> mFlList;

   

protected:
    void contextMenuEvent(QContextMenuEvent *event);
};
#endif // MAINWINDOW_H
