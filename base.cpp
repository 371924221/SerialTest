#include "base.h"
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <qmath.h>
#include <QJsonObject>
#include <QJsonDocument>
Base::Base()
{


}

//通过方法函数实现：

void Base::CRC16(char* pchMsg, int wDataLen,char ret[2])
{
    int CRC = 0x0000ffff;
    int POLYNOMIAL = 0x0000a001;
    int i, j;

    if (wDataLen == 0)
    {
        return;
    }
    for (i = 0; i < wDataLen; i++)
    {
        CRC ^= ((int)pchMsg[i] & 0x000000ff);
        for (j = 0; j < 8; j++)
        {
            if ((CRC & 0x00000001) != 0)
            {
                CRC >>= 1;
                CRC ^= POLYNOMIAL;
            }
            else
            {
                CRC >>= 1;
            }
        }
        //System.out.println(Integer.toHexString(CRC));
    }

    ret[0] = (char)(CRC & 0xff);
    ret[1] = (char)((CRC >> 8) & 0xff);

    return;
}
int Base::char2Int(char* buff,int size)
{
    int ret = 0;
    qDebug()<<"char2Int,size:"<<size;
    if(size <= 0 || size > 4)
    {
        return 0;
    }
    if(size == 1)
    {
        ret = (buff[0]&0xff);
    }
    else if(size == 2)
    {
        ret+=((buff[1]&0xff)+((buff[0]&0xff)<<8));
    }
    else if(size == 3)
    {
        ret+=((buff[2]&0xff)+((buff[1]&0xff)<<8)+((buff[0]&0xff)<<16));
    }
    else if(size == 4)
    {
        qDebug()<<"char2Int,buff[0]:"<<QString().sprintf("0x%02x",buff[0]);
        qDebug()<<"char2Int,buff[1]:"<<QString().sprintf("0x%02x",buff[1]);
        qDebug()<<"char2Int,buff[2]:"<<QString().sprintf("0x%02x",buff[2]);
        qDebug()<<"char2Int,buff[3]:"<<QString().sprintf("0x%02x",(buff[3]&0xff));
        ret+=((buff[3]&0xff)+((buff[2]&0xff)<<8)+((buff[1]&0xff)<<16)+((buff[0]&0xff)<<24));
    }
    qDebug()<<"char2Int,ret:"<<ret;
    return ret;
}

void Base::sleep()
{
        QTime t;
        t.start();
        while(t.elapsed()<1000)
        {
            QCoreApplication::processEvents();
        }
}

bool Base::isNum(QString ch){
    QString num[]={"1","2","3","4","5","6","7","8","9","0"};
    for(int i=0;i<10;i++){
        if(ch.compare(num[i])==0)return true;
    }return false;
}

QString Base::getFloat(QVariant number,float accuracy)//根据小数精度获取浮点数,如:getFloat(0.12345,0.001) = 0.123
{
    //qDebug()<<"ParamFloat::format:"<<format;
    //ret.sprintf(format.toStdString().c_str(),m_value.toFloat());使用sprintf会死机,20170315
    int decimals = qCeil(1/accuracy - 0.5);
    decimals = qCeil(qLn(decimals)/qLn(10) - 0.5);//将1000转化为3
    return QString::number(number.toFloat(), 'f', decimals);
}
QString Base::getFloat(QVariant number,int decimals)//获取几位小数的浮点数,不使用科学计数法,getFloat(0.12345,3) = 0.123
{
    return QString::number(number.toFloat(), 'f', decimals);
}

//a:数字
//fieldWidth：最小宽度，默认为0
//base:进制，2,8,10,16等。
//默认以' '补齐
QString Base::getInteger(int a,int fieldWidth,int base)
{
    //QString QString::arg(int a, int fieldWidth = 0, int base = 10, QChar fillChar = QLatin1Char( ' ' ));
    return QString("%1").arg(a,fieldWidth,base);
}

QString Base::getHex(int a,int fieldWidth)
{
    //QString QString::arg(int a, int fieldWidth = 0, int base = 10, QChar fillChar = QLatin1Char( ' ' ));
    return QString("0x%1").arg(a,fieldWidth,16,'0');
}

QString Base::getHex(int a,int b,int fieldWidth)
{
    //QString QString::arg(int a, int fieldWidth = 0, int base = 10, QChar fillChar = QLatin1Char( ' ' ));
    return QString("0x%1 0x%2").arg(a,fieldWidth,16,'0').arg(b,fieldWidth,16,'0');
}

QString Base::getHex(char a,char b,int fieldWidth)
{
    //QString QString::arg(int a, int fieldWidth = 0, int base = 10, QChar fillChar = QLatin1Char( ' ' ));
    return getHex((int)a,(int)b,fieldWidth);
}

QString Base::Json2QString(QJsonObject obj)
{
    QJsonDocument document;
    document.setObject(obj);
    QByteArray byte_array = document.toJson();
    QString text = QString::fromLatin1(byte_array);
    return text;
}

