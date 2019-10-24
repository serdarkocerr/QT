#include "GalleryWidget.h"
#include "ui_GalleryWidget.h"

#include <QItemSelectionModel>
#include <QDebug>
GalleryWidget::GalleryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GalleryWidget)
{
    qDebug()<<"GalleryWidget::GalleryWidget(QWidget *parent) : Constructor --begin line 10  ";

    ui->setupUi(this);
    ui->albumListWidget->setMaximumWidth(250);
    connect(ui->albumWidget, &AlbumWidget::pictureActivated, this, &GalleryWidget::pictureActivated);
    qDebug()<<"GalleryWidget::GalleryWidget(QWidget *parent) : Constructor --end line 15  ";

}

GalleryWidget::~GalleryWidget()
{
    qDebug()<<"GalleryWidget::~GalleryWidget() : DEConstructor --begin line 21  ";
    qDebug()<<"GalleryWidget::~GalleryWidget() : DEConstructor --end line 22  ";

    delete ui;
}

void GalleryWidget::setAlbumModel(AlbumModel* albumModel)
{
    qDebug()<<"void GalleryWidget::setAlbumModel(AlbumModel* albumModel) --begin line 29  ";

    ui->albumListWidget->setModel(albumModel);
    ui->albumWidget->setAlbumModel(albumModel);
    qDebug()<<"void GalleryWidget::setAlbumModel(AlbumModel* albumModel) --end line 33  ";

}

void GalleryWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel)
{
    qDebug()<<"void GalleryWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel) --begin line 39  ";

    ui->albumListWidget->setSelectionModel(albumSelectionModel);
    ui->albumWidget->setAlbumSelectionModel(albumSelectionModel);
    qDebug()<<"void GalleryWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel) --end line 43  ";

}

void GalleryWidget::setPictureModel(ThumbnailProxyModel* pictureModel)
{
    qDebug()<<"void GalleryWidget::setPictureModel(ThumbnailProxyModel* pictureModel) --begin line 49  ";
    ui->albumWidget->setPictureModel(pictureModel);
    qDebug()<<"void GalleryWidget::setPictureModel(ThumbnailProxyModel* pictureModel) --end line 51  ";

}

void GalleryWidget::setPictureSelectionModel(QItemSelectionModel* pictureSelectionModel)
{
    qDebug()<<"void GalleryWidget::setPictureSelectionModel(QItemSelectionModel* pictureSelectionModel) --begin line 57  ";
    ui->albumWidget->setPictureSelectionModel(pictureSelectionModel);
    qDebug()<<"void GalleryWidget::setPictureSelectionModel(QItemSelectionModel* pictureSelectionModel) --end line 59  ";

}
