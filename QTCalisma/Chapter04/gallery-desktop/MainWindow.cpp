#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QStackedWidget>
#include <QItemSelectionModel>

#include "GalleryWidget.h"
#include "PictureWidget.h"
#include "AlbumModel.h"
#include "PictureModel.h"
#include "ThumbnailProxyModel.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mGalleryWidget(new GalleryWidget(this)),
    mPictureWidget(new PictureWidget(this)),
    mStackedWidget(new QStackedWidget(this))
{
    qDebug()<<"MainWindow::MainWindow(QWidget *parent) : Constructor --begin line 20  ";

    ui->setupUi(this);

    AlbumModel* albumModel = new AlbumModel(this);
    QItemSelectionModel* albumSelectionModel = new QItemSelectionModel(albumModel, this);
    mGalleryWidget->setAlbumModel(albumModel);
    mGalleryWidget->setAlbumSelectionModel(albumSelectionModel);

    PictureModel* pictureModel = new PictureModel(*albumModel, this);
    ThumbnailProxyModel* thumbnailModel = new ThumbnailProxyModel(this);
    thumbnailModel->setSourceModel(pictureModel);

    QItemSelectionModel* pictureSelectionModel = new QItemSelectionModel(thumbnailModel, this);
    mGalleryWidget->setPictureModel(thumbnailModel);
    mGalleryWidget->setPictureSelectionModel(pictureSelectionModel);
    mPictureWidget->setModel(thumbnailModel);
    mPictureWidget->setSelectionModel(pictureSelectionModel);

    connect(mGalleryWidget, &GalleryWidget::pictureActivated,
            this, &MainWindow::displayPicture);

    connect(mPictureWidget, &PictureWidget::backToGallery,
            this, &MainWindow::displayGallery);

    mStackedWidget->addWidget(mGalleryWidget);
    mStackedWidget->addWidget(mPictureWidget);
    displayGallery();

    setCentralWidget(mStackedWidget);


    qDebug()<<"MainWindow::MainWindow(QWidget *parent) : Constructor --end line 51  ";

}

MainWindow::~MainWindow()
{
    qDebug()<<"MainWindow::~MainWindow() : DEConstructor --begin line 57  ";
    qDebug()<<"MainWindow::~MainWindow() : DEConstructor --end line 58  ";
    delete ui;
}

void MainWindow::displayGallery()
{
    qDebug()<<"void MainWindow::displayGallery()  --begin line 64  ";
    mStackedWidget->setCurrentWidget(mGalleryWidget);
    qDebug()<<"void MainWindow::displayGallery()  --end line 66  ";

}

void MainWindow::displayPicture(const QModelIndex& /*index*/)
{
    qDebug()<<"void MainWindow::displayPicture(const QModelIndex& /*index*/)  --begin line 72  ";
    mStackedWidget->setCurrentWidget(mPictureWidget);
    qDebug()<<"void MainWindow::displayPicture(const QModelIndex& /*index*/)  --end line 74  ";

}
