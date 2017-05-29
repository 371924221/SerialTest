#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "datafilter.h"
#include "idevicemanager.h"
#include "base.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mpDestSerial = new QSerialPort(this);
/*
界面可设置:超时时间，收到后延时发送下一条指令的延时时间，地址，命令
报告：超时错误报告(时间，地址，命令)
*/
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
        qDebug() << "System Location: " << info.systemLocation();
        ui->comboBox_SerialList->addItem(info.portName());
        if(info.description() == "USB Serial Port")
        {
            QString USBSerialPort = info.portName();
            ui->comboBox_SerialList->setCurrentText(USBSerialPort);
        }
        mSerialPortInfoList.append(info);
    }


    mpAddressGroup = new QButtonGroup(this);
    mpAddressGroup->setExclusive(false);
    mpAddressGroup->addButton(ui->checkBox_1,1);
    mpAddressGroup->addButton(ui->checkBox_2,2);
    mpAddressGroup->addButton(ui->checkBox_3,3);
    mpAddressGroup->addButton(ui->checkBox_4,4);
    mpAddressGroup->addButton(ui->checkBox_5,5);

    mpCmdGroup = new QButtonGroup(this);
    mpCmdGroup->setExclusive(false);
    mpCmdGroup->addButton(ui->checkBox_CMD_1,0);
    mpCmdGroup->addButton(ui->checkBox_CMD_2,1);
    mpCmdGroup->addButton(ui->checkBox_CMD_3,2);
    mpCmdGroup->addButton(ui->checkBox_CMD_4,3);
    mpCmdGroup->addButton(ui->checkBox_CMD_5,4);

    //瞬时流量
    Params*param = DataFilter::getInstance()->getParamByTransString("strFlowMoment");
    if(param != NULL)
    {
        mReadParamMap.insert(param->m_ParamName,param);
    }
    //输出电流
    param = DataFilter::getInstance()->getParamByTransString("strOutputCurrent");
    if(param != NULL)
    {
        mReadParamMap.insert(param->m_ParamName,param);
    }
    //口径
    param = DataFilter::getInstance()->getParamByTransString("strDiameter");
    if(param != NULL)
    {
        mReadParamMap.insert(param->m_ParamName,param);
    }
    //语言
    param = DataFilter::getInstance()->getParamByTransString("strLanguage");
    if(param != NULL)
    {
        mReadParamMap.insert(param->m_ParamName,param);
    }
    //传感器编号
    param = DataFilter::getInstance()->getParamByTransString("strSensorNum");
    if(param != NULL)
    {
        mReadParamMap.insert(param->m_ParamName,param);
    }

    QStringList ParamNameList;
    QList<QString> Qlist = mReadParamMap.keys();
    foreach (QString paramName, Qlist) {
        ParamNameList.append(paramName);
    }

    ui->comboBox_Param_1->addItems(ParamNameList);
    param = DataFilter::getInstance()->getParamByTransString("strFlowMoment");
    ui->comboBox_Param_1->setCurrentText(param->m_ParamName);
    mParedMap.insert(ui->checkBox_CMD_1,ui->comboBox_Param_1);

    ui->comboBox_Param_2->addItems(ParamNameList);
    param = DataFilter::getInstance()->getParamByTransString("strOutputCurrent");
    ui->comboBox_Param_2->setCurrentText(param->m_ParamName);
    mParedMap.insert(ui->checkBox_CMD_2,ui->comboBox_Param_2);

    ui->comboBox_Param_3->addItems(ParamNameList);
    param = DataFilter::getInstance()->getParamByTransString("strDiameter");
    ui->comboBox_Param_3->setCurrentText(param->m_ParamName);
    mParedMap.insert(ui->checkBox_CMD_3,ui->comboBox_Param_3);

    ui->comboBox_Param_4->addItems(ParamNameList);
    param = DataFilter::getInstance()->getParamByTransString("strLanguage");
    ui->comboBox_Param_4->setCurrentText(param->m_ParamName);
    mParedMap.insert(ui->checkBox_CMD_4,ui->comboBox_Param_4);

    ui->comboBox_Param_5->addItems(ParamNameList);
    param = DataFilter::getInstance()->getParamByTransString("strSensorNum");
    ui->comboBox_Param_5->setCurrentText(param->m_ParamName);
    mParedMap.insert(ui->checkBox_CMD_5,ui->comboBox_Param_5);

    mStartTest = false;

    IDeviceManager::getInstance();

    connect(IDeviceManager::getInstance(),SIGNAL(on_SerialTestSignal(QList<IDeviceManager::TimeOutInfo>*,QList<IDeviceManager::TimeOutInfo>*,bool)),
                                           this,SLOT(on_SerialTestSignal_get(QList<IDeviceManager::TimeOutInfo>*,QList<IDeviceManager::TimeOutInfo>*,bool)));

    QTableView *view = ui->tableView_failed;

    mModel = new QStandardItemModel();
    view->setModel(mModel);
    view->setEditTriggers(QTableView::NoEditTriggers);

    view->horizontalHeader()->setStretchLastSection(true);//最后一个填充最后的空白位置
    view->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选中一行
    //view->verticalHeader()->setVisible(false);

    mModel->setHorizontalHeaderItem(0, new QStandardItem("地址"));
    mModel->setHorizontalHeaderItem(1, new QStandardItem("索引"));
    mModel->setHorizontalHeaderItem(2, new QStandardItem("指令"));

    view->setColumnWidth(0,50);
    view->setColumnWidth(1,50);
    view->setColumnWidth(2,250);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Open_clicked()
{
    if(ui->comboBox_SerialList->count() == 0)
    {
        QMessageBox::warning(this,"警告","本机没有串口设备");
        mpDestSerial = NULL;
        return;
    }
    int index = ui->comboBox_SerialList->currentIndex();
    const QSerialPortInfo &CurrentPort = mSerialPortInfoList.at(index);
    mpDestSerial->setPort(CurrentPort);
    if(mpDestSerial->open(QIODevice::ReadWrite))
    {
        qDebug() << "mpDestSerial->open(QIODevice::ReadWrite)";
        mpDestSerial->setBaudRate(QSerialPort::Baud9600);
        mpDestSerial->setParity(QSerialPort::NoParity);
        mpDestSerial->setDataBits(QSerialPort::Data8);
        mpDestSerial->setStopBits(QSerialPort::OneStop);
        mpDestSerial->setFlowControl(QSerialPort::NoFlowControl);

        mpDestSerial->clearError();
        mpDestSerial->clear();
    }
    else
    {
        QMessageBox::warning(this,"警告","串口打开失败");
        mpDestSerial = NULL;
    }
}

void MainWindow::on_pushButton_StartTest_clicked()
{
    if(mpDestSerial == NULL || !mpDestSerial->isOpen())
    {
        QMessageBox::warning(this,"警告","串口设备不存在");
        return;
    }
    if(!mpDestSerial->isOpen())
    {
        QMessageBox::warning(this,"警告","串口设备打开失败");
        return;
    }

    if(mStartTest)
    {
        IDeviceManager::getInstance()->stopTest();
        ui->pushButton_StartTest->setText("开始测试");
        ui->pushButton_StartTest->setEnabled(false);
        mStartTest = false;
        return;
    }

    bool bOK = false;
    int iCmdTimeout = 1000;
    int iCmdInterval = 100;
    QString context = ui->lineEdit_TimeOut->text();
    iCmdTimeout = context.toInt(&bOK);
    if(!bOK || iCmdTimeout <= 0)
    {
        QMessageBox::warning(this,"警告","请输出正确的超时时间(ms)");
        return;
    }
    context = ui->lineEdit_Interval->text();
    iCmdInterval = context.toInt(&bOK);
    if(!bOK || iCmdInterval < 0)
    {
        QMessageBox::warning(this,"警告","请输出正确的间隔时间(ms)");
        return;
    }

    QList<QAbstractButton*> AddressList = mpAddressGroup->buttons();
    foreach (QAbstractButton*pAddressCheckBox, AddressList) {
        if(((QCheckBox*)pAddressCheckBox)->isChecked())
        {
            int address = mpAddressGroup->id(pAddressCheckBox);
            mAddressList.append((char)address);
        }
    }
    if(mAddressList.size() == 0)
    {
        QMessageBox::warning(this,"警告","请至少选择一个地址");
        return;
    }

    mParamsList.clear();
    QList<QAbstractButton*> CmdList = mpCmdGroup->buttons();
    foreach (QAbstractButton*pAddressCheckBox, CmdList) {
        QCheckBox* pCheckBox = (QCheckBox*)pAddressCheckBox;
        if(pCheckBox->isChecked())
        {
            QMap <QCheckBox*,QComboBox*>::const_iterator iter = mParedMap.find(pCheckBox);
            if(iter != mParedMap.end())
            {
                QComboBox* pComboBox = iter.value();
                if(pComboBox != NULL)
                {
                    QString ParamName = pComboBox->currentText();
                    QMap <QString,Params*>::iterator iter = mReadParamMap.find(ParamName);
                    if(iter != mReadParamMap.end())
                    {
                        Params* param = *iter;
                        if(param != NULL)
                        {
                            mParamsList.append(param);
                        }
                    }
                }
            }
        }
    }
    if(mParamsList.size() == 0)
    {
        QMessageBox::warning(this,"警告","请至少选择一条指令");
        return;
    }
    mFailedSize = 0;
    mModel->clear();
    ui->pushButton_StartTest->setText("停止测试");
    ui->pushButton_StartTest->setEnabled(false);
    mStartTest = true;
    IDeviceManager::getInstance()->startTest(mpDestSerial,mAddressList,mParamsList,iCmdTimeout,iCmdInterval);
}

void MainWindow::on_SerialTestSignal_get(QList<IDeviceManager::TimeOutInfo>*SuccessInfo,QList<IDeviceManager::TimeOutInfo>*FailedInfo,bool bThreadStart)
{
    if(mStartTest && bThreadStart)
    {
        ui->pushButton_StartTest->setEnabled(true);
    }
    else if(!mStartTest && !bThreadStart)
    {
        ui->pushButton_StartTest->setEnabled(true);
    }


    if(FailedInfo->size() != 0 && mModel->rowCount() != FailedInfo->size())
    {

        for(int row = mModel->rowCount();row < FailedInfo->size();row++)
        {
            IDeviceManager::TimeOutInfo info = FailedInfo->at(row);
            mModel->setItem(row, 0, new QStandardItem(info.address));
            mModel->setItem(row, 1, new QStandardItem(info.index));
            mModel->setItem(row, 2, new QStandardItem(info.paramName));
        }
    }
    ui->label_failed->setText(QVariant(FailedInfo->size()).toString());
    ui->label_successed->setText(QVariant(SuccessInfo->size()).toString());
    if(FailedInfo->size()+SuccessInfo->size() == 0)
    {
        ui->label_rate->setText("");
    }
    else
    {
        ui->label_rate->setText(Base::getFloat(QVariant(100.0f*FailedInfo->size()/(FailedInfo->size()+SuccessInfo->size())),2)+"%");
    }

}
