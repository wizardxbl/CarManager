#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <win_qextserialport.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void OpenComPort(void);
    void CloseComPort(void);
    
private:
    Ui::MainWindow *ui;
    Win_QextSerialPort *ComPort;
    qint8 state;
    QDir Dir;
    QFile DataFile;
    qint32 BigCarNumber;
    qint32 SmartCarNumber;
    qint32 SumCarNumber;

public slots:
    void ButtonClicked(void);
    void TimerUpDate(void);
    void ComPortReady(void);
};

#endif // MAINWINDOW_H
