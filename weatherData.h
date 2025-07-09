#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#endif // WEATHERDATA_H

#include<QString>
//Today用于显示今天的所有天气参数
class Today{
public:
Today()
{
    date = "2022-10-20";
    city = "广州";
    ganmao = "感冒指数";
    wendu = 0;
    shidu = "0%";
    pm25 = 0;
    quality = "无数据";
    type = "多云";
    fx = "2级";
    fl = "南风";
    high = 30;
    low = 18;
    aqi = 0;

}

QString date;
QString city;
QString ganmao;
int wendu;
QString shidu;
int pm25;
QString quality;
QString type;

QString fx;
QString fl;

int high;
int low;
int aqi;




};

//用于显示六天的天气参数
class Day{
public:
    Day()
    {
        date = "2022-10-20";
        week = "星期五";
        type = "多云";
        high = 0;
        low = 0;
        fx = "2级";
        fl = "南风";
        aqi = 0;
    }


QString date;
QString week;
QString type;
int high;
int low;
QString fx;
QString fl;
int aqi; 




};
