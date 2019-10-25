#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"on_pushButton_clicked -- begin.";
    //Singelton classlar construct edilmesi icin asagida instancelari cagriliyor.
    Dao::instance();
    Manager::instance();

    //butona basinca 5 sanide bir calisan periyodik job kuruluyor ve signali emit ediyor her 5 saniyede bir.
    QTimer *timer = new QTimer(this);
    /*Lambda ile signal slot kurma*/
    connect(timer,&QTimer::timeout, &Dao::instance(), []{
        emit Dao::instance().signalForCallingManagerSlot();
    });
    timer->start(5000);//5 sec
    qDebug()<<"on_pushButton_clicked -- end.";
}
