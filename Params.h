#ifndef PARAMS_H
#define PARAMS_H
#include <QJsonObject>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <QVariant>
#include <QMutex>
class Params
{

public:
    Params(QJsonObject jsonInfoStruct);
    typedef enum{
        PROTOCOLTYPE_NORMAL = 0,//一般是读写码+长度+地址
    }protocolType;
    typedef enum{
        VALUETYPE_ENUM,
        VALUETYPE_INT,
        VALUETYPE_LONG,
        VALUETYPE_FLOAT,
        VALUETYPE_STRING,
        VALUETYPE_NONE,
    }valueType;
    typedef enum{
        EXECTYPE_RW,
        EXECTYPE_RO,
        EXECTYPE_WO,
        EXECTYPE_CALC
    }execType;

    typedef enum{
        Cmd_ReadParam,
        Cmd_WriteParam,
    }RWType;

    typedef struct{
        int iCode;
        QString sString;
    }sTable;
    //写参数时的刷新函数
    virtual bool updateValue(QJsonObject jsonValue) = 0;
    //读参数时的刷新函数
    virtual bool updateValue(QByteArray &byteArray) = 0;
    virtual bool setTmpValue(QVariant value) = 0;
    virtual bool isModifying() = 0;//是否处于待修改状态
    inline void setDefault(){m_tmpValue = m_value;}
    inline void setRW(RWType iRwType){m_RWType = iRwType;}
    virtual QByteArray getDataSendCmd(QVariant value);

    virtual int CheckDataList(QByteArray& buff);

    inline QVariant getValue(){return m_value;}
    virtual QString getDisplayString() = 0;
    virtual QString getMouseMoveHintString() = 0;//鼠标悬停时弹出的tips
    QString getDisplayUnit();

    //for enum
    virtual void getDisplayStringArray(std::vector<sTable> &QStringArray){QStringArray.clear();return;}
    virtual QStringList getDisplayStringList(){return QStringList();}
    virtual int getDisplayStringIndex(){return 0;}
    virtual int getCodeByIndex(int index){return 0;}

    virtual QJsonObject getTransObjectString() = 0;
    virtual bool isValueValid(QVariant value){return false;}

    typedef enum{
        TransferState_Initial,
        TransferState_SentThenReceive,
        TransferState_ReceivedThenSend,
    }TransferState;
    QMutex *m_pTransferMutex;
    void setTransferState(TransferState state);
    TransferState getTransferState();
protected:
    virtual QByteArray getReadDataSendCmd(QVariant value);

    virtual QByteArray getWriteDataSendCmd(QVariant value);

public:
    QString m_transString;//用于传输的参数名
    QString m_ParamName;//显示的参数名
    protocolType m_protocolType;//协议类型
    execType m_execType;//参数读写类型
    valueType m_valueType;//参数类型
    QVariant m_value;//获取的值
    QVariant m_minValue;//最小值
    QVariant m_maxValue;//最大值
    QVariant m_tmpValue;//临时值
    float     m_accuracy;//精度
    char     m_DataLen;
    char    m_readCode;
    char    m_writeCode;
    char    m_ModBusAddr;//流量计MODBUS地址,固定为01
    char    m_DataAddr[2];
    QString m_HexQStringAddress;//16进制的字符串形式的参数地址
    RWType m_RWType;//执行的读写类型

    QString    m_displayFormat;

    QString m_displayArg;
    int m_displayWidth;
    int m_displayBase;
protected:
    QString m_ParamUnit;//显示的单位
    QString m_ParamUnitUseOther;//依赖其他参数的单位显示

    TransferState m_TransferState;
    QByteArray getCheckData(QVariant nNum);

};

class ParamEnum : public Params
{
public:
    ParamEnum(QJsonObject jsonInfoStruct);
    bool updateValue(QJsonObject jsonValue);
    bool updateValue(QByteArray &byteArray);
    bool setTmpValue(QVariant value);
    QJsonObject getTransObjectString();
    void getDisplayStringArray(std::vector<sTable> &QStringArray);
    QStringList getDisplayStringList();
    int getDisplayStringIndex();
    QString getDisplayString();
    int getCodeByIndex(int index);
    QString getMouseMoveHintString();
    void UpdateParamUnitTable(QJsonArray valstrArray);
    bool isValueValid(QVariant value);
    bool isModifying();
private:
    void UpdateParamTable(QJsonArray valstrArray);
    std::vector<sTable> m_StringArraySet;
    std::map<QString,QVariant> m_StringArrayMap;
};

class ParamInteger : public Params
{
public:
    ParamInteger(QJsonObject jsonInfoStruct);
    QJsonObject getTransObjectString();
    bool updateValue(QJsonObject jsonValue);
    bool updateValue(QByteArray &byteArray);
    bool setTmpValue(QVariant value);
    QString getDisplayString();
    QString getMouseMoveHintString();
    bool isValueValid(QVariant value);
    bool isModifying();
};

class ParamFloat : public Params
{
public:
    ParamFloat(QJsonObject jsonInfoStruct);
    QJsonObject getTransObjectString();
    bool updateValue(QJsonObject jsonValue);
    bool updateValue(QByteArray &byteArray);
    bool setTmpValue(QVariant value);
    QString getDisplayString();
    QString getMouseMoveHintString();
    bool isValueValid(QVariant value);
    bool isModifying();
};

class ParamString : public Params
{
public:
    ParamString(QJsonObject jsonInfoStruct);
    QJsonObject getTransObjectString();
    bool updateValue(QJsonObject jsonValue);
    bool updateValue(QByteArray &byteArray);
    bool setTmpValue(QVariant value);
    QString getDisplayString();
    QString getMouseMoveHintString();
    bool isModifying();
};

class ParamNone : public Params
{
public:
    bool updateValue(QJsonObject jsonValue){jsonValue.count();return false;}
    bool updateValue(QByteArray &byteArray){return false;}
    bool setTmpValue(QVariant value){value.isValid();return false;}
    ParamNone(QJsonObject jsonInfoStruct);
    QJsonObject getTransObjectString();
    QString getDisplayString();
    QString getMouseMoveHintString();
    bool isModifying(){return false;}
private:

};

class ParamEncodeStep1 : public Params
{
public:
    ParamEncodeStep1(QJsonObject jsonInfoStruct):Params(jsonInfoStruct){}
    QJsonObject getTransObjectString(){}
    bool updateValue(QJsonObject jsonValue){return true;}
    bool updateValue(QByteArray &byteArray);
    bool setTmpValue(QVariant value){return true;}
    QString getDisplayString(){}
    QString getMouseMoveHintString(){}
    bool isValueValid(QVariant value){return true;}
    bool isModifying(){}
    int CheckDataList(QByteArray& buff);

    void setStep1Result(
            QString version,
            QString password,
            char typeChar);

    QString getVersion(){return mVersion;}
    QString getPassword(){return mPassword;}
    QString getType(){return mType;}

    QByteArray getDataSendCmd(QVariant value);
    QByteArray getReadDataSendCmd(QVariant value);
private:
    QString mVersion;
    QString mPassword;
    QString mType;
};

class ParamEncodeStep2 : public Params
{
public:
    ParamEncodeStep2(QJsonObject jsonInfoStruct);
    QJsonObject getTransObjectString(){}
    bool updateValue(QJsonObject jsonValue){return true;}
    bool updateValue(QByteArray &byteArray);
    bool setTmpValue(QVariant value);
    QString getDisplayString(){}
    QString getMouseMoveHintString(){}
    bool isValueValid(QVariant value){return true;}
    bool isModifying(){return false;}
    int CheckDataList(QByteArray& buff);

    void setStep2Result(bool result){mbResult = result;}
    bool getResult(){return mbResult;}
    QByteArray getDataSendCmd(QVariant value);
    QByteArray getReadDataSendCmd(QVariant value);
private:
    bool mbResult;
};


#endif // PARAMS_H
