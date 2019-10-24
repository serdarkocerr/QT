#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sinifmodel.h"
#include <QDebug>
#include "sinifogrencicomb.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mSinifOgrenciCombt(new SinifOgrenciComb(this))
{
    ui->setupUi(this);

    SinifModel *sinifModel = new  SinifModel(this);
    QItemSelectionModel* sinifSelectionModel = new QItemSelectionModel(sinifModel, this);
    mSinifOgrenciCombt->setSinifModel(sinifModel);
    mSinifOgrenciCombt->setSinifSelectionModel(sinifSelectionModel);

    OgrenciModel *ogrenciModel = new OgrenciModel(this);
    QItemSelectionModel* ogrenciSelectionModel = new QItemSelectionModel(ogrenciModel, this);
    mSinifOgrenciCombt->setOgrenciModel(ogrenciModel);
    mSinifOgrenciCombt->setOgrenciSelectionModel(ogrenciSelectionModel);

    setCentralWidget(mSinifOgrenciCombt);

}

MainWindow::~MainWindow()
{
    delete ui;
}

