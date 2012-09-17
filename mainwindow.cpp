#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->DateAndTime->setText(QDateTime::currentDateTime().toString("yyyy-mm-dd hh:mm:ss dddd"));
    QTimer *timer = new QTimer(this);
    ComPort = new Win_QextSerialPort("COM1", QextSerialBase::EventDriven); //���崮�ڶ���ָ���������Ͳ�ѯģʽ���¼�����
    ComPort->open(QIODevice::ReadWrite); //�Զ�д��ʽ�򿪴���
    ComPort->setBaudRate(BAUD115200); //���ò�����
    ComPort->setDataBits(DATA_8); //��������λ
    ComPort->setParity(PAR_NONE); //��������żУ��
    ComPort->setStopBits(STOP_1); //����ֹͣλ
    ComPort->setFlowControl(FLOW_OFF); //������������
    ComPort->setTimeout(500);

    // �������ݴ洢���ļ���
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
        ui->Button->setText(QString::fromLocal8Bit("��ͣ"));
        // ���ͼ�������������
        qDebug("Count Will be Cleared");
        //ComPort->write();//QByteArray &data
        flag = false;
        state = 1;
        qDebug("Now Start ...");
    } else {
        ui->Button->setText(QString::fromLocal8Bit("����"));
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

void MainWindow::TimerUpDate(void) // ��ʾϵͳ����
{
    QDateTime time = QDateTime::currentDateTime();

    QString str=time.toString("yyyy-mm-dd hh:mm:ss dddd");

    ui->DateAndTime->setText(str);

    // ���� ��ʾ���ֲ���
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
    QByteArray temp = ComPort->readAll(); //��ȡ���ڻ�������������
    qDebug()<<"COM DATA RCV:"<<temp;
}
