#ifndef BASE_H
#define BASE_H
#include "QString"
#include <QByteArray>
#include <QVariant>
class Base
{
public:
    Base();
    static void CRC16(char* array, int wDataLen, char ret[2]);
    static int char2Int(char* buff,int size);
    static bool isNum(QString ch);

    static void sleep();

    static QString getFloat(QVariant number,float accuracy);//根据小数精度获取浮点数,如:getFloat(0.12345,0.001) = 0.123
    static QString getFloat(QVariant number,int decimals);//获取几位小数的浮点数,不使用科学计数法,getFloat(0.12345,3) = 0.123

    static QString getInteger(int a,int fieldWidth = 0,int base = 10);
    static QString getHex(int a,int fieldWidth);
    static QString getHex(int a,int b,int fieldWidth);
    static QString getHex(char a,char b,int fieldWidth);

    static QString Json2QString(QJsonObject obj);
};

#endif // BASE_H
