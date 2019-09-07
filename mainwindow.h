#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QMessageBox>  //显示警告
#include <QByteArray>

#include <QTableWidget>

#include "framedecode.h"
#include <QStandardItemModel>

#include "aboutbox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    aboutbox *view;

    FrameDecode FrameDecoder;
    QByteArray raw_data;
    QByteArray canframe_buffer;

    char buf[1024];
    char decode_flag;
    char readflag;
    qint64 lineLength;

//    struct CANFrame{
//        char ID;
//        char status;
//        char DLC;
//        QString value;
//    };


    QString ABOUT_TITLE = "工业现场智能检测系统终端";
    QString ABOUT_STRING = "工业现场智能检测系统终端 \n";

    FrameDecode::CANFrame canframe;
//    QVector<CANFrame> raw_vector;
    QVector<FrameDecode::CANFrame> vector;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void scanPort();
    QVariant getBaudRate();
    bool openSerialPort();
    void closeSerialPort();
    void configSerialPort();

private slots:
    void on_actionConnect_triggered();
    void on_actionEXIT_triggered();



    void on_Open_clicked();

    void on_CloseNODE_clicked();

    void on_actionAbout_triggered();

private:
     QSerialPort *serialPort;
     QTimer *seial_scanTimer;
     QTimer *serial_showdataTimer;
     void onTimerUpdate();
     void serial_showdataTimerUpdate();
};

#endif // MAINWINDOW_H
