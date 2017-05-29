#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QButtonGroup>
#include <QMap>
#include "Params.h"
#include <QCheckBox>
#include <QComboBox>
#include <QStandardItemModel>
#include "idevicemanager.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_Open_clicked();

    void on_pushButton_StartTest_clicked();

    void on_SerialTestSignal_get(QList<IDeviceManager::TimeOutInfo>*,QList<IDeviceManager::TimeOutInfo>*,bool);

private:
    Ui::MainWindow *ui;
    QSerialPort *mpDestSerial;
    QList<QSerialPortInfo> mSerialPortInfoList;
    QButtonGroup *mpAddressGroup;

    QButtonGroup *mpCmdGroup;

    QStandardItemModel *mModel;

    QList<char> mAddressList;

    QList<Params*> mParamsList;

    QMap <QCheckBox*,QComboBox*> mParedMap;

    QMap <QString,Params*> mReadParamMap;

    bool mStartTest;

    int mFailedSize;
};

#endif // MAINWINDOW_H
