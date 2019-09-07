#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    decode_flag = 0;
    readflag    = 1;
    lineLength  = 0;
    serialPort = new QSerialPort(this);
    seial_scanTimer = new QTimer(this);
    seial_scanTimer->start(10);

    serial_showdataTimer = new QTimer(this);
    serial_showdataTimer->start(15);

    connect(seial_scanTimer, &QTimer::timeout, this, &MainWindow::onTimerUpdate);
    connect(serial_showdataTimer, &QTimer::timeout, this, &MainWindow::serial_showdataTimerUpdate);
    view = new aboutbox(this);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete  view;
}

void MainWindow::scanPort()
{
    bool sync = false;
    QComboBox *box = ui->COMselect;
    QVector<QSerialPortInfo> vec;
//扫描可用端口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        // 检测端口列表变更
        QString str = info.portName() + " (" + info.description() + ")";
        if (box->findText(str) == -1) {
            sync = true;
        }
        vec.append(info);
    }

    if (sync || box->count() != vec.count()) {
        int len = 0;
        QFontMetrics fm(box->font());
        QString text = box->currentText();
        bool edited = box->findText(text) == -1; // only edit enable (linux)
        box->clear();
        for (int i = 0; i < vec.length(); ++i) {
            QString name = vec[i].portName() + " (" + vec[i].description() + ")";
            box->addItem(name);
            int width = fm.boundingRect(name).width(); // 计算字符串的像素宽度
            if (width > len) { // 统计最长字符串
                len = width;
            }
        }
        // 设置当前选中的端口
        if (!text.isEmpty() && (box->findText(text) != -1 || edited)) {
            box->setCurrentText(text);
        } else {
            box->setCurrentIndex(0);
        }
        #if defined(Q_OS_LINUX)
                box->lineEdit()->setCursorPosition(0);
        #endif
        // 自动控制下拉列表宽度
        box->view()->setMinimumWidth(len + 16);
    }
}

QVariant MainWindow::getBaudRate()
{
//    std::cout <<QVariant(ui->baudselct->currentText()).toInt() << std::endl;
    return QVariant(ui->baudselct->currentText());
}

void MainWindow::onTimerUpdate()
{

    QByteArray temp;
//    std::cout << "time update" << std::endl;
    MainWindow::getBaudRate();
    MainWindow::configSerialPort();
    if (serialPort->isOpen())
    {
        if (readflag == 1){
             raw_data = serialPort->readAll();
        }
        decode_flag = 0;
        if (raw_data.length() != -1){
         vector = FrameDecoder.frameDecode(raw_data);
        }
        decode_flag = 1;
     }
}

void MainWindow::serial_showdataTimerUpdate()
{
//       std::cout<<char(canframe.ID)<< std::endl;
    if (decode_flag == 1)
    {
       // readflag = 0;
    for(auto canframe:vector)
    {

    QString status ;
       if (canframe.status == '0')
       {
           status = "初始化";
       }
       if (canframe.status == '1')
       {
            status = "未连接";
       }
       if (canframe.status == '4')
       {
           status = "停止";
       }
       if (canframe.status == '5')
       {
            status = "正在运行";
       }

        if (canframe.ID == '1')
        {
            ui->NODE11->clear();
            ui->NODE21->clear();
            ui->NODE31->clear();
            ui->NODE11->insertPlainText(QString("节点1ID: %1").arg(char(canframe.ID)));
            ui->NODE21->insertPlainText(QString("节点1状态: %1").arg(status));
            ui->NODE31->insertPlainText(QString("节点1数据: %1").arg(canframe.value));
        }
        if (canframe.ID == '2')
        {
            ui->NODE12->clear();
            ui->NODE22->clear();
            ui->NODE32->clear();
            ui->NODE12->insertPlainText(QString("节点2ID： %1").arg(char(canframe.ID)));
            ui->NODE22->insertPlainText(QString("节点2状态: %1").arg(status));
            ui->NODE32->insertPlainText(QString("节点2数据: %1").arg(canframe.value));

        }
        if (canframe.ID == '3')
        {
             ui->NODE13->clear();
             ui->NODE23->clear();
             ui->NODE33->clear();
             ui->NODE13->insertPlainText(QString("节点3ID： %1").arg(char(canframe.ID)));
             ui->NODE23->insertPlainText(QString("节点3状态: %1").arg(status));
             ui->NODE33->insertPlainText(QString("节点3数据: %1").arg(canframe.value));

        }
        if (canframe.ID == '4')
        {
             ui->NODE14->clear();
             ui->NODE24->clear();
             ui->NODE34->clear();
             ui->NODE14->insertPlainText(QString("节点4ID： %1").arg(char(canframe.ID)));
             ui->NODE24->insertPlainText(QString("节点4状态: %1").arg(status));
             ui->NODE34->insertPlainText(QString("节点4数据: %1").arg(canframe.value));

        }
        if (canframe.ID == '5')
        {
            ui->NODE15->clear();
            ui->NODE25->clear();
            ui->NODE35->clear();
            ui->NODE15->insertPlainText(QString("节点5ID： %1").arg(char(canframe.ID)));
            ui->NODE25->insertPlainText(QString("节点5状态: %1").arg(status));
            ui->NODE35->insertPlainText(QString("节点5数据: %1").arg(canframe.value));

        }
        if (canframe.ID == '6')
        {
            ui->NODE16->clear();
            ui->NODE26->clear();
            ui->NODE36->clear();
            ui->NODE16->insertPlainText(QString("节点6ID： %1").arg(char(canframe.ID)));
            ui->NODE26->insertPlainText(QString("节点6状态: %1").arg(status));
            ui->NODE36->insertPlainText(QString("节点6数据: %1").arg(canframe.value));
        }
  }
    readflag = 1;
}
}

bool MainWindow::openSerialPort()
{
    QString SeialPortname = ui->COMselect->currentText().section(' ',0, 0);
    serialPort->setPortName(SeialPortname);
    if(serialPort->open(QIODevice::ReadWrite))
    {
        ui->COMselect->setEnabled(false);
        std::cout<<"open succed ！"<<std::endl;
        QMessageBox suc(QMessageBox::NoIcon,
                        tr("Cool"),
                        tr("打开端口成功\n"),
                        QMessageBox::Ok, this);
        suc.exec();
        return true;
    }
    QMessageBox err(QMessageBox::Critical,
                    tr("Error"),
                    tr("无法端口串口！\n"),
                    QMessageBox::Cancel, this);
    err.exec();

    return false;
}

void MainWindow::closeSerialPort()
{
    serialPort->close();
    ui->COMselect->setEnabled(true);
    QMessageBox clo(QMessageBox::Information,
                    tr("Close"),
                    tr("关闭端口成功\n"),
                    QMessageBox::Ok, this);
    clo.exec();

}


void MainWindow::configSerialPort()
{
    MainWindow::scanPort();
    serialPort->setBaudRate(QVariant(ui->baudselct->currentText()).toInt());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
}


void MainWindow::on_actionConnect_triggered()
{
    bool Flagopenport = MainWindow::openSerialPort();
//    std::cout << Flagopenport << std::endl;
}


void MainWindow::on_actionEXIT_triggered()
{
    MainWindow::closeSerialPort();

}




void MainWindow::on_Open_clicked()
{
    QString Node = QVariant(ui->selctnodebox->currentText()).toString();

    if (Node ==  "NODE1")
    {
        //node_ctr(2,4)
        cout << (Node ==  "NODE1")<< endl;
//        char cmd[17] = {'n','o','d','e','_','c','t','r','(','2',',','0','x','0','2',')','\'};
        char *cmd = "node_ctr(1,0x01)\r\n";

        qint64 s =  serialPort->write(cmd);
        cout<< s << endl;
    }
    if (Node ==  "NODE2")
    {
                char *cmd = "node_ctr(2,0x01)\r\n";

                qint64 s =  serialPort->write(cmd);


    }
    if (Node ==  "NODE3")
    {
                char *cmd = "node_ctr(3,0x01)\r\n";

                qint64 s =  serialPort->write(cmd);

    }
    if (Node ==  "NODE4")
    {
                char *cmd = "node_ctr(4,0x01)\r\n";

                qint64 s =  serialPort->write(cmd);

    }
    if (Node ==  "NODE5")
    {
                char *cmd = "node_ctr(5,0x01)\r\n";

                qint64 s =  serialPort->write(cmd);

    }
     std::cout<<"pressed " << std::endl;

}

void MainWindow::on_CloseNODE_clicked()
{
//     std::cout<<QVariant(ui ->currentText()).toInt() << std::endl;

    QString Node = QVariant(ui->selctnodebox->currentText()).toString();

    if (Node ==  "NODE1")
    {
        //node_ctr(2,4)
        cout << (Node ==  "NODE1")<< endl;
//        char cmd[17] = {'n','o','d','e','_','c','t','r','(','2',',','0','x','0','2',')','\'};
        char *cmd = "node_ctr(1,0x02)\r\n";

        qint64 s =  serialPort->write(cmd);
        cout<< s << endl;
    }
    if (Node ==  "NODE2")
    {
                char *cmd = "node_ctr(2,0x02)\r\n";

                qint64 s =  serialPort->write(cmd);


    }
    if (Node ==  "NODE3")
    {
                char *cmd = "node_ctr(3,0x02)\r\n";

                qint64 s =  serialPort->write(cmd);

    }
    if (Node ==  "NODE4")
    {
                char *cmd = "node_ctr(4,0x02)\r\n";

                qint64 s =  serialPort->write(cmd);

    }
    if (Node ==  "NODE5")
    {
                char *cmd = "node_ctr(5,0x02)\r\n";

                qint64 s =  serialPort->write(cmd);

    }

}

void MainWindow::on_actionAbout_triggered()
{
    view->show();
}
