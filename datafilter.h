#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <stdlib.h>
#include <stdio.h>
#include <params.h>
#include <QMap>
class DataFilter
{
public:
    DataFilter();
    static DataFilter *getInstance();
    Params *getParamByName(QString ParamName);
    Params *getParamByTransString(QString TransString);
    QStringList getParamArray(int paramIndex,int subIndex);
    void getAllParamsName(std::vector <QString> & NameSet);

    ParamEncodeStep1 *getParamEncodeStep1();
    ParamEncodeStep2 *getParamEncodeStep2();
public:
    static DataFilter *m_instance;
    std::vector <QString> strJsonSource;
private:
    QString getString(QString code);
    void initStrings();

    std::vector <QString> m_AllParamName;
    std::map <QString,Params*> m_NameParamMap;
    std::map <QString,Params*> m_StringParamMap;
    std::map <QString,Params*> m_AddressParamMap;

    ParamEncodeStep1 * m_ParamEncodeStep1;
    ParamEncodeStep2 * m_ParamEncodeStep2;
};

#endif // DATAFILTER_H
