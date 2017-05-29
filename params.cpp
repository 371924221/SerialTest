#include "params.h"
#include "datafilter.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QVariant>
#include <qDebug>
#include <QtCore/qmath.h>
#include <base.h>
#include <QMutexLocker>
#include "IDeviceManager.h"
Params::Params(QJsonObject jsonInfoStruct):
    m_ModBusAddr(0x01),
    m_TransferState(TransferState_Initial),
    m_pTransferMutex(NULL)
{
    m_pTransferMutex = new QMutex(QMutex::Recursive);
    QString ValueType = jsonInfoStruct["valueType"].toString();
    if(ValueType == "PARAM_VALUE_TYPE_ENUM")
    {
        m_valueType = VALUETYPE_ENUM;
    }
    else if(ValueType == "PARAM_VALUE_TYPE_SHORT")
    {
        m_valueType = VALUETYPE_INT;
    }
    else if(ValueType == "PARAM_VALUE_TYPE_LONG")
    {
        m_valueType = VALUETYPE_LONG;
    }
    else if(ValueType == "PARAM_VALUE_TYPE_FLOAT")
    {
        m_valueType = VALUETYPE_FLOAT;
    }
    else if(ValueType == "PARAM_VALUE_TYPE_STRING")
    {
        m_valueType = VALUETYPE_STRING;
    }
    else if(ValueType == "PARAM_VALUE_TYPE_NONE")
    {
        m_valueType = VALUETYPE_NONE;
    }
    else
    {
        qDebug()<<"unknow type："<<ValueType;
    }



    QString ExecType = jsonInfoStruct["executeType"].toString();
    if(ExecType == "PARAM_RW_TYPE_RO")
    {
        m_execType = EXECTYPE_RO;
    }
    else if(ExecType == "PARAM_RW_TYPE_RW")
    {
        m_execType = EXECTYPE_RW;
    }
    else if(ExecType == "PARAM_RW_TYPE_WO")
    {
        m_execType = EXECTYPE_WO;
    }
    else if(ExecType == "PARAM_RW_TYPE_CALC")
    {
        m_execType = EXECTYPE_CALC;
    }
    else
    {
        qDebug()<<"未知参数类型："<<ExecType;
        m_execType = EXECTYPE_RO;
        //Assert(false);
    }
    qDebug()<<"Params::m_ParamName:"<<m_ParamName;
    m_transString = jsonInfoStruct["string"].toString();

    qDebug()<<"jsonInfoStruct::ParamName:"<<jsonInfoStruct["paramName"].toString();

    m_ParamName = QObject::tr(m_transString.toStdString().c_str());//m_transString.toStdString().c_str();
    qDebug()<<"Params::m_ParamName:"<<m_ParamName;

    QString unitType = jsonInfoStruct["UnitType"].toString();
    if(unitType == "PARAM_UNIT_TYPE_NONE")
    {
        m_ParamUnit = "";
        m_ParamUnitUseOther = "";
    }
    else if(unitType == "PARAM_UNIT_TYPE_SELF")
    {
        m_ParamUnit = " "+jsonInfoStruct["Unit"].toString();
        m_ParamUnitUseOther = "";
    }
    else if(unitType == "PARAM_UNIT_TYPE_OTHER")
    {
        m_ParamUnit = "";
        m_ParamUnitUseOther = jsonInfoStruct["Unit"].toString();
    }

    m_accuracy = jsonInfoStruct["accuracy"].toDouble();

    m_DataLen = jsonInfoStruct["lengh"].toInt();

    m_readCode = jsonInfoStruct["readCode"].toInt();

    m_writeCode = jsonInfoStruct["writeCode"].toInt();

    int address = jsonInfoStruct["address"].toInt();
    address = address>0?address-1:address;

    m_DataAddr[0] = (address & 0xff00) >> 8;
    m_DataAddr[1] = address & 0xff;

    m_protocolType = PROTOCOLTYPE_NORMAL;

    //m_HexQStringAddress("0x%1").arg(address,4,16,'0');
    m_HexQStringAddress = Base::getHex(address,4);

    if(jsonInfoStruct.contains("displayFormat") && !jsonInfoStruct["displayFormat"].isNull())
    {
        m_displayFormat = jsonInfoStruct["displayFormat"].toString();

        qDebug()<<"Params::m_displayFormat:"<<m_displayFormat;
    }
    if(jsonInfoStruct.contains("displayArg") && !jsonInfoStruct["displayArg"].isNull()
            && jsonInfoStruct.contains("displayWidth") && !jsonInfoStruct["displayWidth"].isNull()
            && jsonInfoStruct.contains("displayBase") && !jsonInfoStruct["displayBase"].isNull())
    {
        //"\"displayFormat\":\"0x%1\","+//
        //"\"displayWidth\":2,"+//固定宽度
        //"\"displayBase\":16,"+//进制
        m_displayArg = jsonInfoStruct["displayArg"].toString();
        m_displayWidth = jsonInfoStruct["displayWidth"].toInt();
        m_displayBase = jsonInfoStruct["displayBase"].toInt();
    }

}

QByteArray Params::getDataSendCmd(QVariant value)
{
    if(m_RWType == Cmd_ReadParam)
    {
        return getReadDataSendCmd(value);
    }
    else if(m_RWType == Cmd_WriteParam)
    {
        return getWriteDataSendCmd(m_tmpValue);
    }
}

QByteArray Params::getReadDataSendCmd(QVariant value)
{
    qDebug()<<"getReadDataSendCmd:"+m_ParamName;
    if(m_execType == EXECTYPE_WO || m_execType == EXECTYPE_CALC)
    {
        //只写参数，直接返回
        qDebug()<<"getReadDataSendCmd: write or calcu only!";
        return NULL;
    }
    char DataLen[2] = {m_DataLen/256,m_DataLen%256};
    QByteArray buff;

    char readcrcdata[8] = {m_ModBusAddr,m_readCode,
            m_DataAddr[0],m_DataAddr[1],
            DataLen[0],DataLen[1],0,0};

    char crc16[2];
    Base::CRC16(readcrcdata,6,crc16);
    QString string;
    //qDebug()<<"crc: "+string("0x%1 0x%2").arg(crc16[0],2,16,'0').arg(crc16[1],2,16,'0');
    qDebug()<<"crc: "+Base::getHex(crc16[0],crc16[1],2);
    buff.append(m_ModBusAddr);
    buff.append(m_readCode);
    buff.append(m_DataAddr[0]);
    buff.append(m_DataAddr[1]);
    buff.append(DataLen[0]);
    buff.append(DataLen[1]);

    buff.append(crc16[0]);
    buff.append(crc16[1]);
    qDebug()<<"read buff: "+buff.toHex();

    return buff;
}


//返回:<0 出错；=0 正确校验(包含buffer包含CheckList的情况);>0 差多少个字符
int Params::CheckDataList(QByteArray& buff)
{
    char crc16[2];
    int len = buff.length();
    qDebug()<<"Param: CRC CHECK";
    if(len < 4)
    {
        return 1;
    }
    len -= 2;
    Base::CRC16(buff.data(),len,crc16);
    QString string;
    if(crc16[0] == buff.at(len)
            && crc16[1] == buff.at(len+1))
    {
        qDebug()<<"CheckRecvData,crc result:true!";
        return 0;
    }
    qDebug()<<"CheckDataList data sprintf CRC16: "+string.sprintf("0x%02x 0x%02x",buff.at(len),buff.at(len+1));
    qDebug()<<"CheckDataList data CRC16: "+Base::getHex(buff.at(len),buff.at(len+1),2);
    qDebug()<<"CheckDataList calcu sprintf CRC16: "+string.sprintf("0x%02x 0x%02x",crc16[0],crc16[1]);
    qDebug()<<"CheckDataList calcu CRC16: "+Base::getHex(crc16[0],crc16[1],2);
    qDebug()<<"CheckRecvData,crc result:false!";
    return -1;
}

/**
        //得到将要返回的数据格式，未知数以'*'代替.
        //入参：
        //iRWMod:读还是写
        //nNum:值
        //buffer:数组形式返回的数据格式，可以有多个，一般是一个。特意为ENCODE2定义的多个参数
        //返回：无意义
     * */
QByteArray Params::getCheckData(QVariant nNum)
{
    if(m_execType == EXECTYPE_CALC)
    {
        //只写参数，直接返回
        qDebug()<<"getCheckData: calcu only!";
        return NULL;
    }
    //QString CmdsAll;
    QByteArray CmdsAll;
    qDebug()<<"getCheckData this.ParamName:"+this->m_ParamName;
    {

        int len = 0;
        char DataLen = this->m_DataLen * 2;//int为单位
        char crc[3] = {0};
        char array[DataLen+1] = {0};
        switch(m_RWType)
        {
            case  Cmd_ReadParam:
                for(int ii = 0;ii < DataLen;ii++)
                {
                    array[ii] = '*';
                }
                CmdsAll.append((char)m_ModBusAddr);//modbus地址
                len++;
                CmdsAll.append(this->m_readCode);//读取码
                len++;
                CmdsAll.append(DataLen);//获取的数据长度，字节为单位
                len++;
                CmdsAll.append(array);//获取的数据
                len = len + DataLen;

                crc[0] = crc[1] = '*';
                /*byte[] crc = Base.crc16(ret.toByteArray());*/
                CmdsAll.append(crc);
                len = len + 2;

                qDebug()<<"getCheckData,allLen:"<<len;
                break;
            case  Cmd_WriteParam:
                //写入数据的返回数据:
                //写入码06H:和发送数据相同。
                //写入码10H:不相同
                if(this->m_writeCode == 0x06 || this->m_writeCode == 0x05)
                {
                    CmdsAll = getWriteDataSendCmd(m_tmpValue);
                }
                else if(this->m_writeCode == 0x10)
                {
                    CmdsAll.append((char)m_ModBusAddr);//modbus地址
                    len++;
                    CmdsAll.append(this->m_writeCode);//读取码
                    len++;

                    CmdsAll.append(this->m_DataAddr[0]);//读取码
                    CmdsAll.append(this->m_DataAddr[1]);//读取码
                    len = len + 2;

                    char arrayLen[2] = {this->m_DataLen/256,this->m_DataLen%256};

                    //这里经过测试后，转换器返回的长度为0，校验失败。
                    CmdsAll.append(arrayLen,2);//获取的数据
                    len = len + 2;

                    char crc16[2];
                    Base::CRC16(CmdsAll.data(),len,crc16);
                    CmdsAll.append(crc16[0]);
                    CmdsAll.append(crc16[1]);
                    len = len + 2;
                    //OnManagerBlock len:8 [1, 16, 2, 8, 0, 2, -63, -78]
                    //[1, 16, 2, 8, 0, 2, 114]
                    //System. arraycopy(ret.toByteArray(),0,buff,0, ret.length());

                }
                break;
            default:
                break;
        }
        qDebug()<<"CheckRecvData,Data should be Cmds:"+CmdsAll.toHex();
    }

    return CmdsAll;
}

QByteArray Params::getWriteDataSendCmd(QVariant value)
{

    qDebug()<<"getWriteDataSendCmd:"+m_ParamName;
    qDebug()<<"value:"+value.toString();
    if(m_execType == EXECTYPE_RO || m_execType == EXECTYPE_CALC)
    {
        //只写参数，直接返回
        qDebug()<<"getWriteDataSendCmd: read or calcu only!";
        return NULL;
    }
    if(m_execType == EXECTYPE_WO)
    {

    }
    QByteArray buff;
    int DataLen = m_DataLen * 2;//int为单位

    char array[DataLen];
    if(this->m_valueType == VALUETYPE_FLOAT)//4字节浮点数
    {
        float ftmp = value.toFloat();
        char *p = (char *)&ftmp;
        array[0] = p[3];
        array[1] = p[2];
        array[2] = p[1];
        array[3] = p[0];
    }
    else if (this->m_valueType == VALUETYPE_STRING)//不定长字符串
    {
        //待实现
        char *p = (char*)value.toString().toStdString().c_str();
        QByteArray Array = value.toByteArray();
        int len = value.toString().length();
        int ii = 0;
        qDebug()<<"type string:len:"<<len<<" DataLen:"<<DataLen;
        //"write buff: 0110ff0500060ca8100000005f715f6e6f5f616b9e"
        while(*p != 0 && ii != DataLen)
        {
            if(ii < len)
            {
                array[ii] = (char)Array.at(ii);
            }
            else
            {
                array[ii] = '\0';
            }
            ii++;
        }
    }
    else
    {
        int itmp = value.toInt();
        char *p = (char *)&itmp;
        if(DataLen == 2)//2字节，枚举
        {
            array[0] = p[1];
            array[1] = p[0];
        }
        if(DataLen == 4)//4字节，整型
        {
            array[0] = p[3];
            array[1] = p[2];
            array[2] = p[1];
            array[3] = p[0];
        }
        if(DataLen == 0)
        {
            //无数据的纯指令
        }
    }

    int len = 0;
    buff.append(m_ModBusAddr);
    len++;
    buff.append(m_writeCode);
    len++;

    buff.append(m_DataAddr[0]);
    buff.append(m_DataAddr[1]);
    len += 2;
    if(m_writeCode == 0x05)//0x05的写入数据，增加0xff,0x00两个字节
    {
        buff.append(0xff);
        buff.append('0');
        len += 2;
    }
    else if(m_writeCode == 0x10)
    {
        char DataLen[2] = {m_DataLen/256,m_DataLen%256};

        //这里经过测试后，转换器返回的长度为0，校验失败。
        buff.append(DataLen,2);//获取的数据
        len += 2;

        buff.append(m_DataLen * 2);
        len += 1;
    }

    buff.append(array,DataLen);
    len = len + DataLen;

    char crc16[2];
    Base::CRC16(buff.data(),len,crc16);
    QString string;
    //qDebug()<<"crc: "+string.sprintf("0x%02x 0x%02x",crc16[0],crc16[1]);
    qDebug()<<"crc: "+Base::getHex(crc16[0],crc16[1],2);
    buff.append(crc16,2);

    qDebug()<<"write buff: "+buff.toHex();
    return buff;
}

QString Params::getDisplayUnit()
{
    qDebug()<<"getDisplayUnit,m_ParamName:"+this->m_ParamName;
    qDebug()<<"getDisplayUnit,m_ParamUnit:"+m_ParamUnit;
    qDebug()<<"getDisplayUnit,m_ParamUnitUseOther:"+m_ParamUnitUseOther;
    if(m_ParamUnit.length() != 0)
    {
        return m_ParamUnit;
    }
    else if(m_ParamUnitUseOther.length() != 0)
    {
        QStringList part;
        QString ret = " ";
        if(m_ParamUnitUseOther.contains("/"))
        {
            part = m_ParamUnitUseOther.split("/");

        }
        else
        {
            part.push_back(m_ParamUnitUseOther);
        }
        for(int ii = 0; ii < part.length();ii++)
        {
            if(ii != 0)
            {
                ret += "/";
            }
            ret += DataFilter::getInstance()->getParamByTransString(part[ii])->getDisplayString();
        }
        qDebug()<<"m_ParamUnitUseOther:"+ret;
        return ret;
    }
    else
    {
        return QString();
    }
}

void Params::setTransferState(TransferState state)
{
    QMutexLocker locker(m_pTransferMutex);
    m_TransferState = state;
    return;
}

Params::TransferState Params::getTransferState()
{
    QMutexLocker locker(m_pTransferMutex);
    return m_TransferState;
}

QByteArray ParamEncodeStep1::getDataSendCmd(QVariant value)
{
    return getReadDataSendCmd(value);
}

bool ParamEncodeStep1::updateValue(QByteArray &byteArray)
{
    qDebug()<<"ParamEncodeStep1::updateValue: "+byteArray;

    //EMF?
    //EMFVer0.02:0000P15
    QByteArray ver = byteArray.mid(6,4);
    QByteArray password = byteArray.mid(11,4);
    QByteArray typeChar = byteArray.mid(15,1);
    setStep1Result(ver,password,typeChar.at(0));

    return true;
}


void ParamEncodeStep1::setStep1Result(
        QString version,
        QString password,
        char typeChar)
{
    mVersion = version;
    mPassword = password;
    switch(typeChar)
    {
        case 'P':
            mType = "PANGU";
            break;
        case 'F':
            mType = "CHENBO";
            break;
        case 'H':
        case 'Z':
        default:
            mType = "ZHENHUA";
            break;
    }
}

int ParamEncodeStep1::CheckDataList(QByteArray& buff)
{
    qDebug()<<"ParamEncodeStep1: "+buff;

    //EMF?
    //EMFVer0.02:0000P15
    qDebug()<<" Encode Step1: "+buff;
    if(buff.startsWith("EMFVer") && buff.contains(':') && buff.length() >= 15)
    {
        return 0;
    }

    return -1;
}


QByteArray ParamEncodeStep1::getReadDataSendCmd(QVariant value)
{
    qDebug()<<"ParamEncodeStep1 getReadDataSendCmd:"+m_ParamName;
    QByteArray buff;

    buff = "EMF?";
    qDebug()<<" ParamEncodeStep1 buff: "+buff;

    return buff;
}

QByteArray ParamEncodeStep2::getReadDataSendCmd(QVariant value)
{
    qDebug()<<"ParamEncodeStep2 getReadDataSendCmd:"+m_ParamName;
    QByteArray buff;

    //现在只有对码2能用到，暂时写死
    //默认为0000
    //但是界面可配置！
    //EMF%04d
    QString keyStr = "EMF"+m_tmpValue.toString();
    buff = keyStr.toLatin1();
    qDebug()<<"ParamEncodeStep2 buff: "+buff;

    return buff;
}


ParamEncodeStep2::ParamEncodeStep2(QJsonObject jsonInfoStruct)
    :Params(jsonInfoStruct),mbResult(false)
{
    m_value = "0000";
    m_tmpValue = m_value;
    m_minValue = "0000";
    m_maxValue = "9999";
}

bool ParamEncodeStep2::setTmpValue(QVariant value)
{
    m_tmpValue = value.toString();
    qDebug()<<"ParamEncodeStep2::setTmpValue.m_tmpValue:"<<m_tmpValue
           <<"m_value:"<<m_value;
    return true;
}
bool ParamEncodeStep2::updateValue(QByteArray &byteArray)
{
    qDebug()<<"ParamEncodeStep2::updateValue: "+byteArray;

    //EMF0000
    //EMFON或EMFERR
    qDebug()<<" Encode Step2: "+byteArray;
    bool result = false;
    if(byteArray == "EMFON")
    {
        result = true;
    }
    else if(byteArray == "EMFERR")
    {
        result = false;
    }
    qDebug()<<" Encode Step2,result: "+result;
    setStep2Result(result);

    return true;
}

QByteArray ParamEncodeStep2::getDataSendCmd(QVariant value)
{
    return getReadDataSendCmd(value);
}

int ParamEncodeStep2::CheckDataList(QByteArray& buff)
{
    //EMF0000
    //EMFON或EMFERR
    qDebug()<<" Encode Step2: "+buff;
    if(buff == "EMFON" || buff == "EMFERR")
    {
        return 0;
    }
    return -1;
}

ParamEnum::ParamEnum(QJsonObject jsonInfoStruct)
    :Params(jsonInfoStruct)
{
    QJsonArray valstrArray = jsonInfoStruct["table"].toArray();

    UpdateParamUnitTable(valstrArray);
    m_value = jsonInfoStruct["defaultValue"].toInt();
    m_tmpValue = m_value;
    m_minValue = jsonInfoStruct["min"].toInt();
    m_maxValue = jsonInfoStruct["max"].toInt();
}

void ParamEnum::UpdateParamUnitTable(QJsonArray valstrArray)
{
    if(valstrArray.size()!= 0)
    {
        qDebug()<<"UpdateParamUnitTable:"<<m_ParamName;
        for(int ii = 0; ii < valstrArray.size();ii++)
        {
            QJsonObject obj = valstrArray.at(ii).toObject();
            sTable elem;
            elem.iCode = obj["code"].toInt();
            elem.sString = QObject::tr(obj["value"].toString().toStdString().c_str());//这里得到的tr过的string，会变成??，不能用，所以想另外的办法
            qDebug()<<"elem.iCode:"<<elem.iCode<<" elem.sString:"<<elem.sString;
            m_StringArraySet.push_back(elem);
            m_StringArrayMap.insert(std::pair<QString,QVariant>(elem.sString,QVariant(elem.iCode)));
        }
    }
    else
    {
        qWarning()<<"m_ParamName:"<<m_ParamName<<" valstrArray.size():"<<valstrArray.size();
    }
}

QJsonObject ParamEnum::getTransObjectString()
{
    QJsonObject elem;
    elem.insert("name",m_transString);
    elem.insert("value",m_tmpValue.toInt());
    return elem;
}

bool ParamEnum::updateValue(QJsonObject jsonValue)
{
    m_value = m_tmpValue;
    qDebug()<<"updateValue:"<<m_value.toString();
    return true;
}

bool ParamEnum::updateValue(QByteArray& byteArray)
{
    //枚举型
    //表示离散型
    //长度为2字节
    /*
"readMessage hex：0103020003f8450103020000b844"
"getCheckData this.ParamName:MODBUS模式"
getCheckData,allLen: 7
"CheckRecvData,Cmds:0103022a2a2a2a"
CheckDataList:true!
char2Int,size: 2
char2Int,ret: 3
updateValue(byteArray): 3
//页面切换时，上一次的数据作为了这一次的数据来解析了，是不对的。
*/
    int len = byteArray[2];
    int off = 3;//从index=3开始读数据
    QByteArray value(byteArray.mid(off,len));
    m_value = Base::char2Int(value.data(),len);
    m_tmpValue = m_value;//
    qDebug()<<"updateValue(byteArray):"<<m_value.toInt();
    return true;
}

//只要和真实值不同，就要返回true
bool ParamEnum::setTmpValue(QVariant value)
{

    m_tmpValue = value.toInt();

    if(!isValueValid(m_tmpValue))
    {
        m_tmpValue = m_value;
    }
    qDebug()<<"ParamEnum::setTmpValue.m_tmpValue:"<<m_tmpValue
           <<"m_value:"<<m_value;

    if(m_value.toInt() == m_tmpValue.toInt())
    {
        return false;
    }
    return true;
}

void ParamEnum::getDisplayStringArray(std::vector<sTable> &QStringArray)
{
    QStringArray = m_StringArraySet;
}

QStringList ParamEnum::getDisplayStringList()
{
    QStringList list;
    for(unsigned int ii = 0; ii < m_StringArraySet.size();ii++)
    {
        list<<(m_StringArraySet[ii].sString+getDisplayUnit());
    }
    return list;
}

int ParamEnum::getCodeByIndex(int index)
{
    return m_StringArraySet[index].iCode;
}

int ParamEnum::getDisplayStringIndex()
{
    for(unsigned int ii = 0; ii < m_StringArraySet.size();ii++)
    {
        if(m_StringArraySet[ii].iCode == m_value.toInt())
        {
            return ii;
        }
    }
    qWarning()<<"getDisplayStringIndex m_ParamName:"<<m_ParamName;
    qWarning()<<"ParamEnum:error NOT a code:"<<m_value.toInt();
    sTable table;
    qWarning()<<"ParamEnum:m_StringArraySet.size():"<<m_StringArraySet.size();
    for(unsigned int ii = 0; ii < m_StringArraySet.size();ii++)
    {
        table = m_StringArraySet.at(ii);
        qWarning()<<"ParamEnum:code:"
                 <<table.iCode
                <<" value:"<<table.sString;
    }
    return 0;
}

bool ParamEnum::isValueValid(QVariant value)
{
    qDebug()<<"ParamEnum::isValueValid value:"<<value;
    for(unsigned int ii = 0; ii < m_StringArraySet.size();ii++)
    {
        qDebug()<<"m_StringArraySet["<<ii<<"].iCode:"<<m_StringArraySet[ii].iCode;
        if(m_StringArraySet[ii].iCode == value.toInt())
        {
            return true;
        }
    }
    qDebug()<<"ParamEnum::isValueValid false";
    return false;
}

bool ParamEnum::isModifying()
{
    qDebug()<<"m_ParamName:"<<m_ParamName<<" ParamEnum::isModifying tmp:"
           <<m_tmpValue.toInt()<<" value:"<<m_value.toInt();
    return m_tmpValue.toInt() != m_value.toInt();
}

QString ParamEnum::getDisplayString()
{
    QString ret;
    for(unsigned int ii = 0; ii < m_StringArraySet.size();ii++)
    {
        if(m_StringArraySet[ii].iCode == m_value.toInt())
        {
            ret = m_StringArraySet[ii].sString;
        }
    }
    ret += getDisplayUnit();
    return ret;
}

QString ParamEnum::getMouseMoveHintString()
{
    QString ret;
    for(unsigned int ii = 0; ii < m_StringArraySet.size();ii++)
    {
        if(m_StringArraySet[ii].iCode == m_value.toInt())
        {
            ret = m_StringArraySet[ii].sString;
        }
    }
    ret += getDisplayUnit();
    if(m_tmpValue.toInt() != m_value.toInt())
    {
        ret += " <- ";
        for(unsigned int ii = 0; ii < m_StringArraySet.size();ii++)
        {
            if(m_StringArraySet[ii].iCode == m_tmpValue.toInt())
            {
                ret += m_StringArraySet[ii].sString;
            }
        }
        ret += getDisplayUnit();
    }
    return ret;
}

ParamInteger::ParamInteger(QJsonObject jsonInfoStruct)
    :Params(jsonInfoStruct)
{
    m_value = jsonInfoStruct["defaultValue"].toInt();
    m_tmpValue = m_value;
    m_minValue = jsonInfoStruct["min"].toInt();
    m_maxValue = jsonInfoStruct["max"].toInt();
}

bool ParamInteger::isValueValid(QVariant value)
{

    if(m_minValue.toInt() > value.toInt()
            || m_maxValue.toInt() < value.toInt())
    {
        return false;
    }

    return true;
}

QJsonObject ParamInteger::getTransObjectString()
{
    QJsonObject elem;
    elem.insert("name",m_transString);
    elem.insert("value",m_tmpValue.toInt());
    return elem;
}
bool ParamInteger::updateValue(QJsonObject jsonValue)
{
    m_value = m_tmpValue;
    qDebug()<<"updateValue:"<<m_value.toString();
    return true;
}

bool ParamInteger::updateValue(QByteArray& byteArray)
{
    if(m_execType == EXECTYPE_CALC)
    {
        qDebug()<<"CALC";
        if(m_transString == "strForwardTotalFlow")
        {
            Params *ToTalH = DataFilter::getInstance()->getParamByTransString("strFlowForwardTotalH");
            Params *ToTalL = DataFilter::getInstance()->getParamByTransString("strFlowForwardTotalL");
            if(ToTalH == NULL || ToTalL == NULL)
            {
                qDebug()<<"can NOT found ";
                return false;
            }
            m_value = ToTalH->m_value.toInt()*10000000+ToTalL->m_value.toInt();
        }
        else if(m_transString == "strBackwardTotalFlow")
        {
            Params *ToTalH = DataFilter::getInstance()->getParamByTransString("strFlowBackwardTotalH");
            Params *ToTalL = DataFilter::getInstance()->getParamByTransString("strFlowBackwardTotalL");
            if(ToTalH == NULL || ToTalL == NULL)
            {
                qDebug()<<"can NOT found ";
                return false;
            }
            m_value = ToTalH->m_value.toInt()*10000000+ToTalL->m_value.toInt();
        }
        else if(m_transString == "strTotalFlow")
        {
            Params *ForwardToTalH = DataFilter::getInstance()->getParamByTransString("strFlowForwardTotalH");
            Params *ForwardToTalL = DataFilter::getInstance()->getParamByTransString("strFlowForwardTotalL");
            Params *BackwardToTalH = DataFilter::getInstance()->getParamByTransString("strFlowBackwardTotalH");
            Params *BackwardToTalL = DataFilter::getInstance()->getParamByTransString("strFlowBackwardTotalL");
            if(ForwardToTalH == NULL || ForwardToTalL == NULL
                    ||BackwardToTalH == NULL || BackwardToTalL == NULL)
            {
                qDebug()<<"can NOT found ";
                return false;
            }
            int Forward = ForwardToTalH->m_value.toInt()*10000000+ForwardToTalL->m_value.toInt();
            int Backward = BackwardToTalH->m_value.toInt()*10000000+BackwardToTalL->m_value.toInt();
            m_value = Forward - Backward;
        }
        else
        {
            qDebug()<<"m_transString NOT found";
            return false;
        }

        m_tmpValue = m_value;//
        return true;
    }
    //整型
    //长度为4或2字节
    int len = byteArray[2];
    int off = 3;//从index=3开始读数据
    QByteArray value = byteArray.mid(off,len);
    m_value = Base::char2Int(value.data(),len);

    m_tmpValue = m_value;//
    qDebug()<<"updateValue(byteArray):"<<m_value.toInt();
    return true;
}

bool ParamInteger::setTmpValue(QVariant value)
{
    m_tmpValue = value.toInt();
    qDebug()<<"ParamInteger::setTmpValue.m_tmpValue:"<<m_tmpValue
           <<"m_value:"<<m_value;
    if(!isValueValid(m_tmpValue))
    {
        m_tmpValue = m_value;
    }
    if(m_value == m_tmpValue.toInt())
    {
        return false;
    }
    return true;
}

bool ParamInteger::isModifying()
{
    return m_tmpValue.toInt() != m_value.toInt();
}

QString ParamInteger::getDisplayString()
{
    QString ret;
    if(m_displayArg != NULL && m_displayArg.length() != 0)
    {
        ret = m_displayArg.arg(m_value.toInt(),m_displayWidth,m_displayBase,QLatin1Char('0'));
        //ret = QString::number(m_value.toFloat(), 'f', decimals);
    }
    else
    {
        ret = QString::number(m_value.toInt());
    }
    ret += getDisplayUnit();
    qDebug()<<"getDisplayString:"+ret;
    return ret;
}

QString ParamInteger::getMouseMoveHintString()
{
    QString ret,tmpret;
    if(m_displayFormat != NULL && m_displayFormat.length() != 0)
    {
        //ret(m_displayFormat.toStdString().c_str()).arg(m_value.toInt());
        ret = m_displayArg.arg(m_value.toInt(),m_displayWidth,m_displayBase,QLatin1Char('0'));
        //tmpret(m_displayFormat.toStdString().c_str()).arg(m_tmpValue.toInt());
        tmpret = m_displayArg.arg(m_tmpValue.toInt(),m_displayWidth,m_displayBase,QLatin1Char('0'));
    }
    else
    {
        ret = QString::number(m_value.toInt());
        tmpret = QString::number(m_tmpValue.toInt());
    }
    ret += getDisplayUnit();
    if(m_value.toInt() != m_tmpValue.toInt())
    {
        ret = ret + " <- "+tmpret+getDisplayUnit();
    }

    qDebug()<<"getMouseMoveHintString:"+ret;
    return  ret;
}

ParamFloat::ParamFloat(QJsonObject jsonInfoStruct)
    :Params(jsonInfoStruct)
{
    m_value = jsonInfoStruct["defaultValue"].toDouble();
    m_tmpValue = m_value;
    m_minValue = jsonInfoStruct["min"].toDouble();
    m_maxValue = jsonInfoStruct["max"].toDouble();
}
bool ParamFloat::isValueValid(QVariant value)
{

    if(m_minValue.toInt() > value.toInt()
            || m_maxValue.toInt() < value.toInt())
    {
        return false;
    }

    return true;
}
QJsonObject ParamFloat::getTransObjectString()
{
    QJsonObject elem;
    elem.insert("name",m_transString);
    elem.insert("value",m_tmpValue.toFloat());
    return elem;
}
bool ParamFloat::updateValue(QJsonObject jsonValue)
{
    m_value = m_tmpValue;
    qDebug()<<"updateValue:"<<m_value.toString();
    return true;
}

bool ParamFloat::updateValue(QByteArray& byteArray)
{
    //表示浮点型
    //长度为4字节
    //表示浮点型
    //长度为4字节
    int len = byteArray[2];
    int off = 3;//从index=3开始读数据
    qDebug()<<"ParamFloat updateValue(byteArray)";
    //需要反一下
    float tmp;
    char *p = (char*)&tmp;
    for(int ii = 3;ii >= 0;--ii)
    {
        p[3-ii] = byteArray[ii+off];
    }

    m_value = tmp;
    m_tmpValue = m_value;

    qDebug()<<"ParamFloat updateValue(byteArray):"<<m_value.toString();
    return true;
}

bool ParamFloat::isModifying()
{
    return m_tmpValue.toFloat() != m_value.toFloat();
}

bool ParamFloat::setTmpValue(QVariant value)
{
    m_tmpValue = value.toFloat();
    qDebug()<<"ParamFloat::setTmpValue.m_tmpValue:"<<m_tmpValue
           <<"m_value:"<<m_value;
    if(!isValueValid(m_tmpValue))
    {
        m_tmpValue = m_value;
    }
    if(m_value.toFloat() == m_tmpValue.toFloat())
    {
        return false;
    }
    return true;
}
QString ParamFloat::getDisplayString()
{
    QString ret;
    ret = Base::getFloat(m_value,m_accuracy);
    ret += getDisplayUnit();
    qDebug()<<"ParamFloat::getDisplayString:"<<ret;
    return  ret;
}
QString ParamFloat::getMouseMoveHintString()
{
    QString ret;
    ret = Base::getFloat(m_value,m_accuracy);
    ret += getDisplayUnit();
    if(m_value.toFloat() != m_tmpValue.toFloat())
    {
        ret += " <- " + Base::getFloat(m_value,m_accuracy);
        ret += getDisplayUnit();
    }
    return  ret;
}

ParamString::ParamString(QJsonObject jsonInfoStruct)
    :Params(jsonInfoStruct)
{
    m_value = "";
    m_tmpValue = m_value;
}

QJsonObject ParamString::getTransObjectString()
{
    QJsonObject elem;
    elem.insert("name",m_transString);
    elem.insert("value",m_tmpValue.toString());
    return elem;
}

bool ParamString::updateValue(QJsonObject jsonValue)
{
    m_value = m_tmpValue;
    qDebug()<<"updateValue:"<<m_value.toString();
    return true;
}
bool ParamString::updateValue(QByteArray& byteArray)
{
    //表示字符串型
    //长度不一定
    int len = byteArray[2];
    int off = 3;//从index=3开始读数据
    m_value = byteArray.mid(off,len);
    m_tmpValue = m_value;
    qDebug()<<"updateValue(byteArray):"<<m_value.toString();
    return true;
}
bool ParamString::setTmpValue(QVariant value)
{
    m_tmpValue = value.toString();
    qDebug()<<"ParamString::setTmpValue.m_tmpValue:"<<m_tmpValue
           <<"m_value:"<<m_value;
    if(m_value.toString() == value.toString())
    {
        return false;
    }
    return true;
}

bool ParamString::isModifying()
{
    return m_tmpValue.toString() != m_value.toString();
}

QString ParamString::getDisplayString()
{
    return  m_value.toString();
}

QString ParamString::getMouseMoveHintString()
{
    QString ret;
    if(m_value.toString() != m_tmpValue.toString())
    {
        ret = m_value.toString()+" <- "+m_tmpValue.toString();
    }
    else
    {
        ret = m_value.toString();
    }
    return  ret;
}

ParamNone::ParamNone(QJsonObject jsonInfoStruct)
    :Params(jsonInfoStruct)
{

}

QJsonObject ParamNone::getTransObjectString()
{
    QJsonObject elem;
    elem.insert("name",m_transString);
    elem.insert("value",0);
    return elem;
}

QString ParamNone::getDisplayString()
{
    return  QObject::tr("dbClicktoExec");
}

QString ParamNone::getMouseMoveHintString()
{
    return  QObject::tr("dbClicktoExec");
}


