#include "datafilter.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <qDebug>

DataFilter* DataFilter::m_instance = NULL;
DataFilter::DataFilter()
{
    initStrings();
}

QString DataFilter::getString(QString code)
{
    return /*QObject::tr(code)*/code;
}


DataFilter *DataFilter::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new DataFilter();
    }
    return m_instance;
}

void DataFilter::initStrings()
{
    //TODO 对码
    QString
    //EMFVer[x]x.xx:0000F//北京城博
    //EMFVer[x]x.xx:0000P//杭州盘古
    //EMFVer[x]x.xxP//杭州盘古
    //EMFVer[x]x.xxF//北京城博
    //F和P是厂家信息，同时也是结束符，:后4位是登入密码
    strEncodeStep1 = QString("")+"{" +
            "\"string\":\"strEncodeStep1\","+

            "\"paramName\":\""+QObject::tr("strEncodeStep1")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_ENCODE\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_CONST\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":\"0\","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\","+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"address\":"+QString("0").sprintf("%d",0x0000)+","+
            "\"lengh\":0,"+
            "\"userPermission\":1,"+
            "\"advPermission\":1}";
    //"+getString("EncodeStep1")+"
    QString //这个参数有点特殊，读指令有参数，写指令也有参数
    strEncodeStep2 = QString("")+"{" +
            "\"string\":\"strEncodeStep2\","+

            "\"paramName\":\""+QObject::tr("strEncodeStep2")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_ENCODE\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_CONST_EXTRA\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":9999,"+
            "\"address\":"+QString("0").sprintf("%d",0x0000)+","+
            "\"lengh\":0,"+
            "\"userPermission\":1,"+
            "\"advPermission\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strEncodeStep3 = QString("")+"{" +
            "\"string\":\"strEncodeStep3\","+

            "\"paramName\":\""+QObject::tr("strEncodeStep3")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_ENCODE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_CONST\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":\"0\","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\","+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"address\":"+QString("0").sprintf("%d",0x0000)+","+
            "\"lengh\":0,"+
            "\"userPermission\":1,"+
            "\"advPermission\":1}";

    //TODO 监视
    QString
    strFlowMoment = QString("")+"{" +
            "\"string\":\"strFlowMoment\","+

            "\"paramName\":\""+QObject::tr("strFlowMoment")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":-99999,"+
            "\"max\":99999,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0253)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_OTHER\","+
            "\"Unit\":\"strFlowTotalUnit/strTimeUnit\"}";

    QString
    strFlowForwardTotalH = QString("")+"{" +
            "\"string\":\"strFlowForwardTotalH\","+

            "\"paramName\":\""+QObject::tr("strFlowForwardTotalH")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":9999999999,"+
            "\"address\":"+QString("0").sprintf("%d",0x0309)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_OTHER\","+
            "\"Unit\":\"strFlowTotalUnit\"}";

    QString
    strFlowForwardTotalL = QString("")+"{" +
            "\"string\":\"strFlowForwardTotalL\","+

            "\"paramName\":\""+QObject::tr("strFlowForwardTotalL")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":9999999999,"+
            "\"address\":"+QString("0").sprintf("%d",0x0311)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_OTHER\","+
            "\"Unit\":\"strFlowTotalUnit\"}";

    QString
    strForwardTotalFlow = QString("")+"{" +
            "\"string\":\"strForwardTotalFlow\","+

            "\"paramName\":\""+QObject::tr("strForwardTotalFlow")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_CALC\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":-999999999,"+
            "\"max\":999999999,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0000)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_OTHER\","+
            "\"Unit\":\"strFlowTotalUnit\"}";

    QString
    strFlowBackwardTotalH = QString("")+"{" +
            "\"string\":\"strFlowBackwardTotalH\","+

            "\"paramName\":\""+QObject::tr("strFlowBackwardTotalH")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":9999999999,"+
            "\"address\":"+QString("0").sprintf("%d",0x0313)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_OTHER\","+
            "\"Unit\":\"strFlowTotalUnit\"}";

    QString
    strFlowBackwardTotalL = QString("")+"{" +
            "\"string\":\"strFlowBackwardTotalL\","+

            "\"paramName\":\""+QObject::tr("strFlowBackwardTotalL")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":9999999999,"+
            "\"address\":"+QString("0").sprintf("%d",0x0315)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_OTHER\","+
            "\"Unit\":\"strFlowTotalUnit\"}";

    QString
    strBackwardTotalFlow = QString("")+"{" +
            "\"string\":\"strBackwardTotalFlow\","+

            "\"paramName\":\""+QObject::tr("strBackwardTotalFlow")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_CALC\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":-999999999,"+
            "\"max\":999999999,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0000)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_OTHER\","+
            "\"Unit\":\"strFlowTotalUnit\"}";

    QString
    strTotalFlow = QString("")+"{" +
            "\"string\":\"strTotalFlow\","+

            "\"paramName\":\""+QObject::tr("strTotalFlow")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_CALC\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":-999999999,"+
            "\"max\":999999999,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0000)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_OTHER\","+
            "\"Unit\":\"strFlowTotalUnit\"}";
    QString
    strOutputCurrent = QString("")+"{" +
            "\"string\":\"strOutputCurrent\","+

            "\"paramName\":\""+QObject::tr("strOutputCurrent")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":4,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":4,"+
            "\"max\":20,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0203)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"mA\"}";

    QString
    strVelocity = QString("")+"{" +
            "\"string\":\"strVelocity\","+

            "\"paramName\":\""+QObject::tr("strVelocity")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":9999999,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0255)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"m/s\"}";
    QString
    strPercentageFlow = QString("")+"{" +
            "\"string\":\"strPercentageFlow\","+

            "\"paramName\":\""+QObject::tr("strPercentageFlow")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":9999999,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0257)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"%\"}";
    QString
    strTrafficSamplingValues = QString("")+"{" +
            "\"string\":\"strTrafficSamplingValues\","+

            "\"paramName\":\""+QObject::tr("strTrafficSamplingValues")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_MONITOR\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":-99999,"+
            "\"max\":99999,"+
            "\"accuracy\":0.0001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x3208)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"mV\"}";
    QString
    strStatuscode = QString("")+"{" +
            "\"string\":\"strStatuscode\","+

            "\"paramName\":\""+QObject::tr("strStatuscode")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_SHORT\","+
            "\"defaultValue\":0,"+
            "\"displayFormat\":\"0x%02x\","+//状态码和报警码的显示格式为16进制
            "\"displayArg\":\"0x%1\","+//QString.arg固定格式
            "\"displayWidth\":2,"+//固定宽度
            "\"displayBase\":16,"+//进制
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":255,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0418)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strAlarmCode = QString("")+"{" +
            "\"string\":\"strAlarmCode\","+

            "\"paramName\":\""+QObject::tr("strAlarmCode")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_SHORT\","+
            "\"defaultValue\":0,"+
            "\"displayFormat\":\"0x%02x\","+//状态码和报警码的显示格式为16进制
            "\"displayArg\":\"0x%1\","+//QString.arg固定格式
            "\"displayWidth\":2,"+//固定宽度
            "\"displayBase\":16,"+//进制
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":255,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0419)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    //TODO 参数
    QString
    strDamp = QString("")+"{" +
            "\"string\":\"strDamp\","+

            "\"paramName\":\""+QObject::tr("strDamp")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":9.9,"+
            "\"accuracy\":1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0189)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strDensyty = QString("")+"{" +
            "\"string\":\"strDensyty\","+

            "\"paramName\":\""+QObject::tr("strDensyty")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":99.9,"+
            "\"accuracy\":0.1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x1541)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"g/mL\"}";
    QString
    strAtcDetectionSensitivity = QString("")+"{" +
            "\"string\":\"strAtcDetectionSensitivity\","+

            "\"paramName\":\""+QObject::tr("strAtcDetectionSensitivity")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":99.9,"+
            "\"accuracy\":0.1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x3200)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strUpperAlarm = QString("")+"{" +
            "\"string\":\"strUpperAlarm\","+

            "\"paramName\":\""+QObject::tr("strUpperAlarm")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":100,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":130,"+
            "\"accuracy\":0.1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0211)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"%\"}";
    QString
    strLowerAlarm = QString("")+"{" +
            "\"string\":\"strLowerAlarm\","+

            "\"paramName\":\""+QObject::tr("strLowerAlarm")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":10,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":130,"+
            "\"accuracy\":0.1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0213)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"%\"}";
    QString
    strScaleFlow = QString("")+"{" +
            "\"string\":\"strScaleFlow\","+

            "\"paramName\":\""+QObject::tr("strScaleFlow")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":1000000,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0209)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strSmallSignalElimination = QString("")+"{" +
            "\"string\":\"strSmallSignalElimination\","+

            "\"paramName\":\""+QObject::tr("strSmallSignalElimination")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":9.9,"+
            "\"accuracy\":0.1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0197)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strPulseEquivalent = QString("")+"{" +
            "\"string\":\"strPulseEquivalent\","+

            "\"paramName\":\""+QObject::tr("strPulseEquivalent")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":1,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0.0001,"+
            "\"max\":999999,"+
            "\"accuracy\":0.00001,"+//精度，一般浮点数才有.浮点数时考虑有效位数
            "\"address\":"+QString("0").sprintf("%d",0x1103)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strPulseUnit = QString("")+"{" +
            "\"string\":\"strPulseUnit\","+

            "\"paramName\":\""+QObject::tr("strPulseUnit")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_GENERAL\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":2,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":2,"+//枚举值的校验直接放在下面的中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            //0:L/P	01：mL/P 02：Hz
            "{\"code\":0,\"value\":\"L/P\"}," +
            "{\"code\":1,\"value\":\"mL/P\"}," +
            "{\"code\":2,\"value\":\"Hz\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0044)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strFrequencyScale = QString("")+"{" +
            "\"string\":\"strFrequencyScale\","+

            "\"paramName\":\""+QObject::tr("strFrequencyScale")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_GENERAL\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":2000,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":100,"+
            "\"max\":10000,"+
            "\"accuracy\":1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0223)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"Hz\"}";

    QString
    strBtName = QString("")+"{" +//蓝牙名称这个参数有点特殊，必须在断开情况下才能设置成功，否则蓝牙模块会当作透传数据，直接再返回给手机APP
            "\"string\":\"strBtName\","+

            "\"paramName\":\""+QObject::tr("strBtName")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_GENERAL\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_STRING\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0xff06)+","+
            "\"lengh\":6,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strBtPassword = QString("")+"{" +
            "\"string\":\"strBtPassword\","+

            "\"paramName\":\""+QObject::tr("strBtPassword")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_GENERAL\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_SHORT\","+
            "\"defaultValue\":0,"+
            "\"displayFormat\":\"%04d\","+//
            "\"displayArg\":\"%1\","+//QString.arg固定格式
            "\"displayWidth\":4,"+//固定宽度
            "\"displayBase\":10,"+//进制
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":9999,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0106)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strATCCalibrationFrequency = QString("")+"{" +
            "\"string\":\"strATCCalibrationFrequency\","+

            "\"paramName\":\""+QObject::tr("strATCCalibrationFrequency")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0.0,"+
            "\"max\":20000.0,"+//资料上没写范围
            "\"accuracy\":0.1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x3202)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"Hz\"}";
    QString
    strFullPipeCalibrationFrequency = QString("")+"{" +
            "\"string\":\"strFullPipeCalibrationFrequency\","+

            "\"paramName\":\""+QObject::tr("strFullPipeCalibrationFrequency")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0.0,"+
            "\"max\":20000.0,"+//资料上没写范围
            "\"accuracy\":0.1,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x3204)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"Hz\"}";
    QString
    strPresetZero = QString("")+"{" +
            "\"string\":\"strPresetZero\","+

            "\"paramName\":\""+QObject::tr("strPresetZero")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":-99.999,"+
            "\"max\":99.999,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0295)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"mV\"}";
    QString
    strOutputFreq = QString("")+"{" +
            "\"string\":\"strOutputFreq\","+

            "\"paramName\":\""+QObject::tr("strOutputFreq")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":1,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":5000,"+
            "\"accuracy\":0.001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0229)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"Hz\"}";
    QString
    strExcitationCurrent = QString("")+"{" +
            "\"string\":\"strExcitationCurrent\","+

            "\"paramName\":\""+QObject::tr("strExcitationCurrent")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":1,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":100,"+
            "\"accuracy\":0.00001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x320A)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"A\"}";
    QString
    strSensorCoefficient = QString("")+"{" +
            "\"string\":\"strSensorCoefficient\","+

            "\"paramName\":\""+QObject::tr("strSensorCoefficient")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":1,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0.0001,"+
            "\"max\":99.9999,"+
            "\"accuracy\":0.0001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x3000)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strMeterCoefficient = QString("")+"{" +
            "\"string\":\"strMeterCoefficient\","+

            "\"paramName\":\""+QObject::tr("strMeterCoefficient")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":1,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0.0001,"+
            "\"max\":99.9999,"+
            "\"accuracy\":0.0001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x3008)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strCorrectionCoefficient = QString("")+"{" +
            "\"string\":\"strCorrectionCoefficient\","+

            "\"paramName\":\""+QObject::tr("strCorrectionCoefficient")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":1,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0.0001,"+
            "\"max\":99.9999,"+
            "\"accuracy\":0.0001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x3006)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strConverterCoefficient = QString("")+"{" +
            "\"string\":\"strConverterCoefficient\","+

            "\"paramName\":\""+QObject::tr("strConverterCoefficient")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":9.9999,"+
            "\"accuracy\":0.0001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x3002)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strMeasuredZero = QString("")+"{" +
            "\"string\":\"strMeasuredZero\","+

            "\"paramName\":\""+QObject::tr("strMeasuredZero")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":-99999,"+
            "\"max\":99999,"+
            "\"accuracy\":0.0001,"+//精度，一般浮点数才有
            "\"address\":"+QString("0").sprintf("%d",0x0293)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"mV\"}";
    QString
    strFlowMomentUnit = QString("")+"{" +//
            "\"string\":\"strFlowMomentUnit\","+

            "\"paramName\":\""+QObject::tr("strFlowMomentUnit")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":13,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"min\":0,"+
            "\"max\":255,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":16,\"value\":\"GFM\"}," +
            "{\"code\":17,\"value\":\"L/M\"}," +
            "{\"code\":19,\"value\":\"m3/h\"}," +
            "{\"code\":75,\"value\":\"kg/h\"}," +
            "{\"code\":78,\"value\":\"t/h\"}," +
            "{\"code\":131,\"value\":\"m3/h\"}," +
            "{\"code\":136,\"value\":\"GPH\"}," +
            "{\"code\":138,\"value\":\"L/h\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0042)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strFlowDirect = QString("")+"{" +
            "\"string\":\"strFlowDirect\","+

            "\"paramName\":\""+QObject::tr("strFlowDirect")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":2,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"ForwardPos\"}," +
            "{\"code\":2,\"value\":\"ForwardDou\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0017)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("ForwardPos");
    QObject::tr("ForwardDou");
    QString
    strFlowCurrentDirect = QString("")+"{" +
            "\"string\":\"strFlowCurrentDirect\","+

            "\"paramName\":\""+QObject::tr("strFlowCurrentDirect")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"ForwardPos\"}," +//正向
            "{\"code\":1,\"value\":\"ForwardNeg\"}" +//反向
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0018)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("ForwardPos");
    QObject::tr("ForwardNeg");
    QString
    strPowerSupplyFrequency = QString("")+"{" +
            "\"string\":\"strPowerSupplyFrequency\","+

            "\"paramName\":\""+QObject::tr("strPowerSupplyFrequency")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"50hz\"}," +
            "{\"code\":1,\"value\":\"60hz\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0014)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strContactMod = QString("")+"{" +
            "\"string\":\"strContactMod\","+

            "\"paramName\":\""+QObject::tr("strContactMod")+"\","+//即脉冲输出相位
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"NormalClose\"}," +//常闭
            "{\"code\":1,\"value\":\"NormalOpen\"}" +//常开
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0009)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("NormalClose");
    QObject::tr("NormalOpen");
    QString
    strMODBUSMod = QString("")+"{" +
            "\"string\":\"strMODBUSMod\","+

            "\"paramName\":\""+QObject::tr("strMODBUSMod")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"RTU\"}," +
            "{\"code\":1,\"value\":\"ASC\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0019)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strBaudRate = QString("")+"{" +
            "\"string\":\"strBaudRate\","+

            "\"paramName\":\""+QObject::tr("strBaudRate")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":5,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"1200\"}," +
            "{\"code\":1,\"value\":\"2400\"}," +
            "{\"code\":2,\"value\":\"4800\"}," +
            "{\"code\":3,\"value\":\"9600\"}," +
            "{\"code\":4,\"value\":\"19200\"}," +
            "{\"code\":5,\"value\":\"38400\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x001A)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strDatadBits = QString("")+"{" +
            "\"string\":\"strDatadBits\","+

            "\"paramName\":\""+QObject::tr("strDatadBits")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"7\"}," +
            "{\"code\":1,\"value\":\"8\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x001B)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strCheckWay = QString("")+"{" +
            "\"string\":\"strCheckWay\","+

            "\"paramName\":\""+QObject::tr("strCheckWay")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":2,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"NoCheck\"}," +
            "{\"code\":1,\"value\":\"OddCheck\"}," +
            "{\"code\":2,\"value\":\"ParityCheck\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x001C)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("NoCheck");
    QObject::tr("OddCheck");
    QObject::tr("ParityCheck");
    QString
    strStopBit = QString("")+"{" +
            "\"string\":\"strStopBit\","+

            "\"paramName\":\""+QObject::tr("strStopBit")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"1\"}," +
            "{\"code\":1,\"value\":\"2\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x001D)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strDiameter = QString("")+"{" +
            "\"string\":\"strDiameter\","+

            "\"paramName\":\""+QObject::tr("strDiameter")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_SHORT\","+
            "\"defaultValue\":100,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":3000,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0012)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"mm\"}";
    QString
    strExcitationFrequency = QString("")+"{" +
            "\"string\":\"strExcitationFrequency\","+

            "\"paramName\":\""+QObject::tr("strExcitationFrequency")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":3,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"3.125Hz\"}," +
            "{\"code\":1,\"value\":\"6.25Hz\"}," +
            "{\"code\":2,\"value\":\"12.5Hz\"}," +
            "{\"code\":3,\"value\":\"25Hz\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0011)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strAutomaticGain = QString("")+"{" +
            "\"string\":\"strAutomaticGain\","+

            "\"paramName\":\""+QObject::tr("strAutomaticGain")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":1,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":3,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +//盘古的参数
            "{\"code\":0,\"value\":\"1\"}," +
            "{\"code\":1,\"value\":\"3\"}," +
            "{\"code\":2,\"value\":\"9\"}" +
            "],"+
            "\"table-F\":[" +//北京的参数
            "{\"code\":0,\"value\":\"1\"}," +
            "{\"code\":1,\"value\":\"2\"}," +
            "{\"code\":2,\"value\":\"4\"}," +
            "{\"code\":3,\"value\":\"8\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0013)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strPulseDuty = QString("")+"{" +
            "\"string\":\"strPulseDuty\","+

            "\"paramName\":\""+QObject::tr("strPulseDuty")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":6,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"200ms\"}," +
            "{\"code\":1,\"value\":\"100ms\"}," +
            "{\"code\":2,\"value\":\"50ms\"}," +
            "{\"code\":3,\"value\":\"20ms\"}," +
            "{\"code\":4,\"value\":\"10ms\"}," +
            "{\"code\":5,\"value\":\"50%\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x000A)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strContactInput = QString("")+"{" +
            "\"string\":\"strContactInput\","+

            "\"paramName\":\""+QObject::tr("strContactInput")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"ClearTotal\"}," +
            "{\"code\":1,\"value\":\"undetermined\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0102)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("ClearTotal");
    QObject::tr("undetermined");
    QString
    strAlarmSelect = QString("")+"{" +
            "\"string\":\"strAlarmSelect\","+

            "\"paramName\":\""+QObject::tr("strAlarmSelect")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":2,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"Upperlimit\"}," +
            "{\"code\":1,\"value\":\"lowerlimit\"}," +
            "{\"code\":2,\"value\":\"FlowDirect\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0103)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("Upperlimit");
    QObject::tr("lowerlimit");
    QObject::tr("FlowDirect");
    QString
    strOutCurrentCheck = QString("")+"{" +
            "\"string\":\"strOutCurrentCheck\","+

            "\"paramName\":\""+QObject::tr("strOutCurrentCheck")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":5,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"OFF\"}," +
            "{\"code\":1,\"value\":\"4mA\"}," +
            "{\"code\":2,\"value\":\"8mA\"}," +
            "{\"code\":3,\"value\":\"12mA\"}," +
            "{\"code\":4,\"value\":\"16mA\"}," +
            "{\"code\":5,\"value\":\"20mA\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0104)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strOutFrequcyCheck = QString("")+"{" +
            "\"string\":\"strOutFrequcyCheck\","+

            "\"paramName\":\""+QObject::tr("strOutFrequcyCheck")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":5,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"OFF\"}," +
            "{\"code\":1,\"value\":\"1Hz\"}," +
            "{\"code\":2,\"value\":\"10Hz\"}," +
            "{\"code\":3,\"value\":\"100Hz\"}," +
            "{\"code\":4,\"value\":\"1000Hz\"}," +
            "{\"code\":5,\"value\":\"5000Hz\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0105)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strATMSwitchDetectionFunction = QString("")+"{" +
            "\"string\":\"strATMSwitchDetectionFunction\","+

            "\"paramName\":\""+QObject::tr("strATMSwitchDetectionFunction")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+//枚举值的校验直接放在下面的table中
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"close\"}," +
            "{\"code\":1,\"value\":\"open\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x6001)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strFlowTotalUnit = QString("")+"{" +
            "\"string\":\"strFlowTotalUnit\","+

            "\"paramName\":\""+QObject::tr("strFlowTotalUnit")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":43,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":255,"+
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":40,\"value\":\"gal\"}," +
            "{\"code\":41,\"value\":\"L\"}," +
            "{\"code\":43,\"value\":\"m3\"}," +
            "{\"code\":42,\"value\":\"Impgal\"}," +
            "{\"code\":46,\"value\":\"bbl\"}," +
            "{\"code\":61,\"value\":\"kg\"}," +
            "{\"code\":62,\"value\":\"t\"}," +
            "{\"code\":63,\"value\":\"lb\"}," +
            "{\"code\":64,\"value\":\"STon\"}," +
            "{\"code\":65,\"value\":\"Lton\"}," +
            "{\"code\":112,\"value\":\"ft3\"}," +
            "{\"code\":232,\"value\":\"Mgal\"}," +
            "{\"code\":231,\"value\":\"ml\"}," +
            "{\"code\":233,\"value\":\"a-ft\"}," +
            "{\"code\":60,\"value\":\"g\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0046)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strModbusAddr = QString("")+"{" +
            "\"string\":\"strModbusAddr\","+

            "\"paramName\":\""+QObject::tr("strModbusAddr")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_SHORT\","+
            "\"defaultValue\":1,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":247,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0047)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strExcitationCheck = QString("")+"{" +
            "\"string\":\"strExcitationCheck\","+

            "\"paramName\":\""+QObject::tr("strExcitationCheck")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":2,"+
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"Nodetect\"}," +
            "{\"code\":1,\"value\":\"X2Y\"}," +
            "{\"code\":2,\"value\":\"Y2X\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0100)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("Nodetect");
    QObject::tr("X2Y");
    QObject::tr("Y2X");
    QString
    strPIOInterface = QString("")+"{" +
            "\"string\":\"strPIOInterface\","+

            "\"paramName\":\""+QObject::tr("strPIOInterface")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":3,"+
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"PassivePulse\"}," +
            "{\"code\":1,\"value\":\"ActivePulse\"}," +
            "{\"code\":2,\"value\":\"AlarmOut\"}," +
            "{\"code\":3,\"value\":\"ContactInput\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0101)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("PassivePulse");
    QObject::tr("ActivePulse");
    QObject::tr("AlarmOut");
    QObject::tr("ContactInput");
    QString
    strLanguage = QString("")+"{" +
            "\"string\":\"strLanguage\","+

            "\"paramName\":\""+QObject::tr("strLanguage")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":1,"+
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"ChineseSim\"}," +
            "{\"code\":1,\"value\":\"English\"}," +
            "{\"code\":2,\"value\":\"ChineseTra\"}," +
            "{\"code\":3,\"value\":\"Spanish\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x1359)+","+
            "\"lengh\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QObject::tr("ChineseSim");
    QObject::tr("English");
    QObject::tr("ChineseTra");
    QObject::tr("Spanish");
    QString
    strTimeUnit = QString("")+"{" +
            "\"string\":\"strTimeUnit\","+

            "\"paramName\":\""+QObject::tr("strTimeUnit")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_ENUM\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x06)+","+
            "\"min\":0,"+
            "\"max\":3,"+
            "\"accuracy\":1,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"sec\"}," +
            "{\"code\":1,\"value\":\"min\"}," +
            "{\"code\":2,\"value\":\"hour\"}," +
            "{\"code\":3,\"value\":\"day\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0043)+","+
            "\"lengh\":1,"+
            "\"userPermission\":1,"+
            "\"advPermission\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strConverterNum = QString("")+"{" +
            "\"string\":\"strConverterNum\","+

            "\"paramName\":\""+QObject::tr("strConverterNum")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":12345678,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":99999999,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0048)+","+
            "\"lengh\":2,"+
            "\"userPermission\":1,"+
            "\"advPermission\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strSensorNum = QString("")+"{" +
            "\"string\":\"strSensorNum\","+

            "\"paramName\":\""+QObject::tr("strSensorNum")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_PARAMETER\","+
            "\"executeType\":\"PARAM_RW_TYPE_RW\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_LONG\","+
            "\"defaultValue\":23456789,"+
            "\"readCode\":"+QString("0").sprintf("%d",0x03)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0,"+
            "\"max\":99999999,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0127)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    //TODO 操作
    QString
    strZerocalibration = QString("")+"{" +//持续30秒时间
            "\"string\":\"strZerocalibration\","+

            "\"paramName\":\""+QObject::tr("strZerocalibration")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x05)+","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0005)+","+
            "\"lengh\":0,"+
            "\"userPermission\":1,"+
            "\"advPermission\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strATCcalibration = QString("")+"{" +//持续15秒时间
            "\"string\":\"strATCcalibration\","+

            "\"paramName\":\""+QObject::tr("strATCcalibration")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x05)+","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0910)+","+
            "\"lengh\":0,"+
            "\"userPermission\":1,"+
            "\"advPermission\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";

    QString
    strFullcalibration = QString("")+"{" +//持续15秒时间
            "\"string\":\"strFullcalibration\","+

            "\"paramName\":\""+QObject::tr("strFullcalibration")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x05)+","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0911)+","+
            "\"lengh\":0,"+
            "\"userPermission\":1,"+
            "\"advPermission\":1,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strSaveFactoryParam = QString("")+"{" +
            "\"string\":\"strSaveFactoryParam\","+

            "\"paramName\":\""+QObject::tr("strSaveFactoryParam")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x05)+","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0920)+","+
            "\"lengh\":0,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strRestoreFactoryParam = QString("")+"{" +
            "\"string\":\"strRestoreFactoryParam\","+

            "\"paramName\":\""+QObject::tr("strRestoreFactoryParam")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x05)+","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0921)+","+
            "\"lengh\":0,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    strClearCumulative = QString("")+"{" +
            "\"string\":\"strClearCumulative\","+

            "\"paramName\":\""+QObject::tr("strClearCumulative")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_NONE\","+
            "\"defaultValue\":0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x05)+","+
            "\"min\":0,"+
            "\"max\":0,"+
            "\"accuracy\":1,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0003)+","+
            "\"lengh\":0,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_NONE\","+
            "\"Unit\":\"\"}";
    QString
    str4mACalibration = QString("")+"{" +
            "\"string\":\"str4mACalibration\","+

            "\"paramName\":\""+QObject::tr("str4mACalibration")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":4.0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":3.5,"+
            "\"max\":4.5,"+
            "\"accuracy\":0.001,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x1189)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"mA\"}";
    QString
    str20mACalibration = QString("")+"{" +
            "\"string\":\"str20mACalibration\","+

            "\"paramName\":\""+QObject::tr("str20mACalibration")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":20.0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":19.5,"+
            "\"max\":20.5,"+
            "\"accuracy\":0.001,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x1191)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"mA\"}";
    QString
    str4_20OutTest = QString("")+"{" +
            "\"string\":\"str4_20OutTest\","+

            "\"paramName\":\""+QObject::tr("str4_20OutTest")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":0.0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0.0,"+
            "\"max\":20.0,"+
            "\"accuracy\":0.001,"+//精度
            "\"table\":[" +
            "{\"code\":0,\"value\":\"ExitTest\"}" +
            "],"+
            "\"address\":"+QString("0").sprintf("%d",0x0143)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"mA\"}";
    QObject::tr("ExitTest");
    QString
    strOutputFreqTtest = QString("")+"{" +
            "\"string\":\"strOutputFreqTtest\","+

            "\"paramName\":\""+QObject::tr("strOutputFreqTtest")+"\","+
            "\"usage\":\"PARAM_USAGE_TYPE_EXECUTE\","+
            "\"executeType\":\"PARAM_RW_TYPE_WO\","+
            "\"protocol\":\"PARAM_PROTOCOL_TYPE_NORMAL\","+
            "\"valueType\":\"PARAM_VALUE_TYPE_FLOAT\","+
            "\"defaultValue\":10.0,"+
            "\"readCode\":"+QString("0").sprintf("%d",0xff)+","+
            "\"writeCode\":"+QString("0").sprintf("%d",0x10)+","+
            "\"min\":0.0,"+
            "\"max\":20000.0,"+//资料上没写范围
            "\"accuracy\":0.001,"+//精度
            "\"address\":"+QString("0").sprintf("%d",0x0147)+","+
            "\"lengh\":2,"+
            "\"UnitType\":\"PARAM_UNIT_TYPE_SELF\","+
            "\"Unit\":\"Hz\"}";

    //监视项集合
    //strJsonSource.push_back(strFlowMomentUnit);瞬时流量单位已不用
    strJsonSource.push_back(strFlowMoment);
    strJsonSource.push_back(strFlowForwardTotalH);
    strJsonSource.push_back(strFlowForwardTotalL);
    strJsonSource.push_back(strForwardTotalFlow);
    strJsonSource.push_back(strFlowBackwardTotalH);
    strJsonSource.push_back(strFlowBackwardTotalL);
    strJsonSource.push_back(strBackwardTotalFlow);
    strJsonSource.push_back(strTotalFlow);
    strJsonSource.push_back(strVelocity);
    strJsonSource.push_back(strPercentageFlow);
    strJsonSource.push_back(strTrafficSamplingValues);
    strJsonSource.push_back(strStatuscode);
    strJsonSource.push_back(strAlarmCode);
    //参数项集合
    //strJsonSource.push_backAll(strJsonSource);
    strJsonSource.push_back(strDamp);
    strJsonSource.push_back(strDensyty);
    strJsonSource.push_back(strUpperAlarm);
    strJsonSource.push_back(strAtcDetectionSensitivity);
    strJsonSource.push_back(strLowerAlarm);
    strJsonSource.push_back(strDiameter);
    strJsonSource.push_back(strSensorNum);
    strJsonSource.push_back(strFlowTotalUnit);
    strJsonSource.push_back(strSmallSignalElimination);
    strJsonSource.push_back(strOutputCurrent);
    strJsonSource.push_back(strPulseEquivalent);
    strJsonSource.push_back(strPulseUnit);
    strJsonSource.push_back(strFrequencyScale);
    strJsonSource.push_back(strBtName);
    strJsonSource.push_back(strBtPassword);
    strJsonSource.push_back(strATCCalibrationFrequency);
    strJsonSource.push_back(strFullPipeCalibrationFrequency);
    strJsonSource.push_back(strPresetZero);
    strJsonSource.push_back(strOutputFreq);
    strJsonSource.push_back(strExcitationCurrent);
    strJsonSource.push_back(strSensorCoefficient);
    strJsonSource.push_back(strCorrectionCoefficient);
    strJsonSource.push_back(strMeterCoefficient);
    strJsonSource.push_back(strConverterCoefficient);
    strJsonSource.push_back(strMeasuredZero);
    strJsonSource.push_back(strFlowDirect);
    strJsonSource.push_back(strFlowCurrentDirect);
    strJsonSource.push_back(strMODBUSMod);
    strJsonSource.push_back(strBaudRate);
    strJsonSource.push_back(strDatadBits);
    strJsonSource.push_back(strCheckWay);
    strJsonSource.push_back(strStopBit);
    strJsonSource.push_back(strPowerSupplyFrequency);
    strJsonSource.push_back(strExcitationFrequency);
    strJsonSource.push_back(strAutomaticGain);
    strJsonSource.push_back(strATMSwitchDetectionFunction);
    strJsonSource.push_back(strModbusAddr);
    strJsonSource.push_back(strExcitationCheck);
    strJsonSource.push_back(strPIOInterface);
    strJsonSource.push_back(strPulseDuty);
    strJsonSource.push_back(strContactMod);
    strJsonSource.push_back(strContactInput);
    strJsonSource.push_back(strAlarmSelect);
    strJsonSource.push_back(strLanguage);
    strJsonSource.push_back(strTimeUnit);
    strJsonSource.push_back(strConverterNum);
    strJsonSource.push_back(strScaleFlow);

    //执行指令集合,只写的指令
    strJsonSource.push_back(strZerocalibration);
    strJsonSource.push_back(strATCcalibration);
    strJsonSource.push_back(strFullcalibration);
    strJsonSource.push_back(strSaveFactoryParam);
    strJsonSource.push_back(strRestoreFactoryParam);
    strJsonSource.push_back(strClearCumulative);
    strJsonSource.push_back(str4mACalibration);
    strJsonSource.push_back(str20mACalibration);
    strJsonSource.push_back(str4_20OutTest);
    strJsonSource.push_back(strOutputFreqTtest);
    strJsonSource.push_back(strOutCurrentCheck);
    strJsonSource.push_back(strOutFrequcyCheck);

    //对码
    strJsonSource.push_back(strEncodeStep1);
    strJsonSource.push_back(strEncodeStep2);
    //strJsonSource.push_back(strEncodeStep3);



    for(unsigned int ii = 0;ii < strJsonSource.size();ii++)
    {
        QJsonObject ParamsDescriptionJsonElement;
        qDebug()<<"json字符串："<<strJsonSource[ii];
        QByteArray bytes = strJsonSource[ii].toLatin1();
        QJsonParseError error;
        QJsonDocument json =  QJsonDocument::fromJson(bytes,&error);

        if(error.error != QJsonParseError::NoError)
        {
            qDebug()<<"error："<<error.errorString()<<" offset:"<<error.offset;
            QString errorstart = QString::fromLatin1(bytes);
            errorstart = errorstart.mid(error.offset);
            qDebug()<<"errorstart:"<<errorstart;
        }
        if(json.isObject())
        {
            ParamsDescriptionJsonElement = json.object();
        }
        Params *param = NULL;
        QString ValueType = ParamsDescriptionJsonElement["valueType"].toString();
        QString UseType = ParamsDescriptionJsonElement["usage"].toString();
        QString string = ParamsDescriptionJsonElement["string"].toString();
        if(UseType == "PARAM_USAGE_TYPE_ENCODE")
        {
            //"string":"strEncodeStep1";
            //"string":"strEncodeStep2";
            if(string == "strEncodeStep1")
            {
                m_ParamEncodeStep1 = new ParamEncodeStep1(ParamsDescriptionJsonElement);
                param = m_ParamEncodeStep1;
            }
            else if(string == "strEncodeStep2")
            {
                m_ParamEncodeStep2 = new ParamEncodeStep2(ParamsDescriptionJsonElement);
                param = m_ParamEncodeStep2;
            }
        }
        else
        {
            if(ValueType == "PARAM_VALUE_TYPE_ENUM")
            {
                param = new ParamEnum(ParamsDescriptionJsonElement);
            }
            else if(ValueType == "PARAM_VALUE_TYPE_SHORT"
                    ||ValueType == "PARAM_VALUE_TYPE_LONG")
            {
                param = new ParamInteger(ParamsDescriptionJsonElement);
            }
            else if(ValueType == "PARAM_VALUE_TYPE_FLOAT")
            {
                param = new ParamFloat(ParamsDescriptionJsonElement);
            }
            else if(ValueType == "PARAM_VALUE_TYPE_STRING")
            {
                param = new ParamString(ParamsDescriptionJsonElement);
            }
            else if(ValueType == "PARAM_VALUE_TYPE_NONE")
            {
                param = new ParamNone(ParamsDescriptionJsonElement);
            }
        }
        qDebug()<<"m_ParamName:"<<param->m_ParamName<<" usage类型："<<UseType;
        qDebug()<<"m_ParamName:"<<param->m_ParamName<<" 参数类型："<<ValueType;
        m_NameParamMap.insert(std::pair<QString,Params*>(param->m_ParamName,param));
        m_StringParamMap.insert(std::pair<QString,Params*>(param->m_transString,param));
        m_AllParamName.push_back(param->m_transString);


        qDebug()<<"参数地址："<<param->m_HexQStringAddress;
        Params *oneParam = NULL;
        if(m_AddressParamMap.size() != 0)
        {
            std::map<QString, Params*>::const_iterator mi; //注意QMap<int, int>::const_iterator 报错
            mi = m_AddressParamMap.find(param->m_HexQStringAddress);
            if(mi != m_AddressParamMap.end())
            {
                oneParam = mi->second;
            }
        }
        qDebug()<<"参数名称："<<param->m_ParamName;
        if(oneParam != NULL)
        {
            qDebug()<<"参数地址重复:"<<oneParam->m_transString<<" 和:"<<param->m_transString;
        }
        else
        {
            m_AddressParamMap.insert(std::pair<QString,Params*>(param->m_HexQStringAddress,param));
        }
    }


}
Params *DataFilter::getParamByName(QString ParamName)
{
    return m_NameParamMap.at(ParamName);
    //return m_NameParamMap.find(ParamName).;
}

Params *DataFilter::getParamByTransString(QString TransString)
{
    return m_StringParamMap.at(TransString);
}

ParamEncodeStep1 *DataFilter::getParamEncodeStep1()
{
    return m_ParamEncodeStep1;
}

ParamEncodeStep2 *DataFilter::getParamEncodeStep2()
{
    return m_ParamEncodeStep2;
}

void DataFilter::getAllParamsName(std::vector <QString> & NameSet)
{
    NameSet = m_AllParamName;
}


