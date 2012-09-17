#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->DateAndTime->setText(QDateTime::currentDateTime().toString("yyyy-mm-dd hh:mm:ss dddd"));
    QTimer *timer = new QTimer(this);
    ComPort = new Win_QextSerialPort("COM1", QextSerialBase::EventDriven); //定义串口对象，指定串口名和查询模式，事件驱动
    ComPort->open(QIODevice::ReadWrite); //以读写方式打开串口
    ComPort->setBaudRate(BAUD115200); //设置波特率
    ComPort->setDataBits(DATA_8); //设置数据位
    ComPort->setParity(PAR_NONE); //设置无奇偶校验
    ComPort->setStopBits(STOP_1); //设置停止位
    ComPort->setFlowControl(FLOW_OFF); //设置无流控制
    ComPort->setTimeout(500);

    // 建立数据存储的文件夹
    if (Dir.exists("Data")) {
        qDebug() << Dir.currentPath();
        qDebug("Data already exists");
    } else {
        qDebug("Create Data Dir");
        Dir.mkdir("Data");
    }
    /* // write data test
    QFile DataFile("./Data/Log.txt");
    if (!DataFile.open(QIODevice::Append)) {
        qDebug("Open Data File Error!");
    } else {
        qDebug("Open Data File OK!");
    }
    DataFile.write(ui->DateAndTime->text().toLocal8Bit().data());
    DataFile.write("\n");
    DataFile.flush();
    DataFile.close();
    */

    connect(ui->Button, SIGNAL(clicked()), this, SLOT(ButtonClicked()));
    connect(timer, SIGNAL(timeout()),this,SLOT(TimerUpDate()));
    connect(ComPort, SIGNAL(readyRead()),this,SLOT(ComPortReady()));

    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ButtonClicked(void)
{
    static bool flag = true;
    qDebug("Button Clicked");
    if (flag) {
        ui->Button->setText(QString::fromLocal8Bit("暂停"));
        // 发送计数器清零命令
        qDebug("Count Will be Cleared");
        //ComPort->write();//QByteArray &data
        flag = false;
        state = 1;
        qDebug("Now Start ...");
    } else {
        ui->Button->setText(QString::fromLocal8Bit("启动"));
        flag = true;
        state = 0;
        qDebug("Now Pause ...");
        QFile DataFile("./Data/Log.txt");
        if (!DataFile.open(QIODevice::Append)) {
            qDebug("Open Data File Error!");
        } else {
            qDebug("Open Data File OK!");
        }
        DataFile.write(ui->DateAndTime->text().toLocal8Bit().data());
        DataFile.write("\n");
        DataFile.write(ui->BigCar->text().toLocal8Bit().data());
        DataFile.write(" ");
        char strBigCar[32];
        sprintf(strBigCar,"%d",BigCarNumber);
        DataFile.write(strBigCar);
        DataFile.write("\n");
        DataFile.flush();

        DataFile.write(ui->SmartCar->text().toLocal8Bit().data());
        DataFile.write(" ");
        char strSmartCar[32];
        sprintf(strSmartCar,"%d",SmartCarNumber);
        DataFile.write(strSmartCar);
        DataFile.write("\n");
        DataFile.flush();

        DataFile.write(ui->sum->text().toLocal8Bit().data());
        DataFile.write(" ");
        char strSum[32];
        sprintf(strSum,"%d",SumCarNumber);
        DataFile.write(strSum);
        DataFile.write("\n");
        DataFile.flush();

        DataFile.close();
    }

}

void MainWindow::TimerUpDate(void) // 显示系统日期
{
    QDateTime time = QDateTime::currentDateTime();

    QString str=time.toString("yyyy-mm-dd hh:mm:ss dddd");

    ui->DateAndTime->setText(str);

    // 测试 显示数字部分
    if (state == 1) {
        qDebug("running");
        QTime t;
        t = QTime::currentTime();
        qsrand(t.msec()+t.second()*1000);
        BigCarNumber = qrand()%99999;
        SmartCarNumber = qrand()%99999;
        SumCarNumber = BigCarNumber + SmartCarNumber;

        ui->BigCarCount->display(BigCarNumber);
        ui->SmartCarCount->display(SmartCarNumber);
        ui->sumCount->display(SumCarNumber);
    }
    // end
}

void MainWindow::ComPortReady(void)
{
    QByteArray temp = ComPort->readAll(); //读取串口缓冲区所有数据
    qDebug()<<"COM DATA RCV:"<<temp;
}
