#include "idevicemanager.h"
#include "DataFilter.h"
#include <QJsonParseError>
#include <QJsonArray>
#include "Base.h"
#include <QSysInfo>
#include <QTime>
#include <qDebug>
IDeviceManager *IDeviceManager::mInstance = NULL;
#define COUNTTIMEOUT 30
IDeviceManager::IDeviceManager():
    mbLoop(true)
    //,m_TransferMutex(QMutex::Recursive)
    ,mThreadStart(false)
{
    qRegisterMetaType<QList<IDeviceManager::TimeOutInfo>*>("QList<IDeviceManager::TimeOutInfo>*");
    start();
}


IDeviceManager* IDeviceManager::getInstance()
{
    if(mInstance == NULL)
    {
        mInstance = new IDeviceManager;
    }
    return mInstance;
}

void IDeviceManager::error()
{

    quit();
}

void IDeviceManager::run()
{
    //循环主体
    while(mbLoop)
    {
        if(!mThreadStart)
        {
            msleep(100);
            continue;
        }
        mTrueInfoList.clear();
        mFalseInfoList.clear();
        emit on_SerialTestSignal(&mTrueInfoList,&mFalseInfoList,true);
        while(mThreadStart)
        {
            for(int ii = 0;ii < mAddressDeviceList.size();ii++)
            {

                if(!mThreadStart)
                {
                    break;
                }
                mpCurrentDevice = &mAddressDeviceList.at(ii);

                for(int index = 0;index < mpCurrentDevice->cmdList.size();index++)
                {

                    if(!mThreadStart)
                    {
                        break;
                    }
                    const QByteArray &array = mpCurrentDevice->cmdList.at(index);
                    this->mpDestSerial->write(array);
                    mTime.start();//将当前系统时间赋值给mTime

                    bool bSuccess = false;
                    TimeOutInfo info;
                    info.address = mpCurrentDevice->address;
                    info.index = index;
                    Params* param = mpCurrentDevice->paramList.at(index);
                    info.paramName = param->m_ParamName;
                    int timeoutCostMs = 0;
                    while(true)
                    {

                        if(this->mpDestSerial->waitForReadyRead(this->mTimeout - timeoutCostMs))
                        {
                            timeoutCostMs = mTime.elapsed();
                            QByteArray qba = mpDestSerial->readAll();

                            int ret = param->CheckDataList((m_tempBuff.append(qba)));
                            if(ret == -1)//校验后完全错误
                            {
                                qDebug()<<"数据完全对不上:"<<m_tempBuff.toHex();
                                m_tempBuff.clear();
                                if(timeoutCostMs >= this->mTimeout)
                                {
                                    //超时
                                    break;
                                }
                                continue;
                            }
                            else if(ret > 0)//校验后差ret个，没有接收完整;
                            {
                                qDebug()<<"差 个数据:"<<ret;
                                if(timeoutCostMs >= this->mTimeout)
                                {
                                    //超时
                                    break;
                                }
                                continue;
                            }
                            else if(ret == 0)
                            {
                                qDebug()<<"数据获取成功";
                                m_tempBuff.clear();
                                bSuccess = true;
                                msleep(this->mInterval);//不是很合理
                                //mTime.start();//将当前系统时间赋值给mTime
                                break;
                            }
                        }
                        else
                        {
                            //超时
                            timeoutCostMs = mTime.elapsed();
                            info.timeCost = timeoutCostMs;
                            break;
                        }
                    }
                    if(bSuccess)
                    {
                        mTrueInfoList.append(info);
                    }
                    else
                    {
                        mFalseInfoList.append(info);
                    }
                    emit on_SerialTestSignal(&mTrueInfoList,&mFalseInfoList,mThreadStart);
                }
            }
        }
        emit on_SerialTestSignal(&mTrueInfoList,&mFalseInfoList,false);
    }
}

IDeviceManager::~IDeviceManager()
{}


void IDeviceManager::startTest(QSerialPort * SerialPort,QList<char> &AddressList,QList<Params*> &ParamsList,int iCmdTimeout,int iCmdInterval)
{

    m_TransferMutex.lock();
    mpDestSerial = SerialPort;
    mTimeout = iCmdTimeout;
    mInterval = iCmdInterval;
    mAddressDeviceList.clear();
    for(QList<char>::ConstIterator addressIter = AddressList.constBegin();addressIter != AddressList.constEnd();addressIter++)
    {
        char address = *addressIter;
        AddressDevice device;
        qDebug() << "地址："<<address;
        device.address = address;
        for(QList<Params*>::ConstIterator iter = ParamsList.constBegin();iter != ParamsList.constEnd();iter++)
        {
            Params *param = *iter;
            qDebug() << "添加参数："<<param->m_ParamName;
            param->setRW(Params::Cmd_ReadParam);
            param->m_ModBusAddr = address;
            QByteArray qba = param->getDataSendCmd(QVariant());

            device.paramList.append(param);
            device.cmdList.append(qba);
            qDebug() << "添加指令："<<qba;
        }
        device.currentIndex = 0;
        mAddressDeviceList.append(device);
    }
    m_TransferMutex.unlock();
    mThreadStart = true;
}
void IDeviceManager::stopTest()
{
    mThreadStart = false;
}

