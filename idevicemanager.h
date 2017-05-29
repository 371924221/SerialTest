#ifndef IDEVICEMANAGER_H
#define IDEVICEMANAGER_H
#include <QObject>
#include "Params.h"
#include <QMap>
#include <QThread>
#include <QSerialPort>
#include <QTime>
class IDeviceManager : public QThread
{
    Q_OBJECT
public:
    explicit IDeviceManager();
    ~IDeviceManager();

    static IDeviceManager* getInstance();

    typedef struct{
        char address;
        int index;
        int timeCost;
        QString paramName;
    }TimeOutInfo;
    typedef struct{
        char address;
        QList<Params*> paramList;
        QList<QByteArray> cmdList;
        int currentIndex;
    }AddressDevice;
    void startTest(QSerialPort * SerialPort,QList<char> &AddressList,QList<Params*> &ParamsList,int iCmdTimeout,int iCmdInterval);
    void stopTest();

Q_SIGNALS:
    void on_SerialTestSignal(QList<IDeviceManager::TimeOutInfo>*,QList<IDeviceManager::TimeOutInfo>*,bool);
private slots:
    void error();
private:
    QMutex m_TransferMutex;
    bool mbLoop;
    QByteArray m_tempBuff;
    static IDeviceManager *mInstance;
    int mTimeout;
    int mInterval;
    QSerialPort *mpDestSerial;


    QList<AddressDevice> mAddressDeviceList;
    QTime mTime;

    const AddressDevice *mpCurrentDevice;


    QList<TimeOutInfo> mFalseInfoList;
    QList<TimeOutInfo> mTrueInfoList;

    bool mThreadStart;

protected:
    void run();
};

#endif // IDEVICEMANAGER_H
