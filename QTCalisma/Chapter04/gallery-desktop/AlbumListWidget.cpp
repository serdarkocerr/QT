#include "AlbumListWidget.h"
#include "ui_AlbumListWidget.h"

#include <QInputDialog>

#include "AlbumModel.h"

#include <QDebug>
AlbumListWidget::AlbumListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumListWidget),
    mAlbumModel(nullptr)
{
    qDebug()<<" AlbumListWidget::AlbumListWidget(QWidget *parent)  Constructor--begin line 14 ";

    ui->setupUi(this);

    connect(ui->createAlbumButton, &QPushButton::clicked,
            this, &AlbumListWidget::createAlbum);
    qDebug()<<" AlbumListWidget::AlbumListWidget(QWidget *parent)  Constructor --end line 20 ";

}

AlbumListWidget::~AlbumListWidget()
{
    qDebug()<<" AlbumListWidget::~AlbumListWidget()  DEConstructor --begin  line 26 ";
    qDebug()<<" AlbumListWidget::~AlbumListWidget()  DEConstructor --end  line 27 ";

    delete ui;
}

void AlbumListWidget::setModel(AlbumModel* model)
{
    qDebug()<<" void AlbumListWidget::setModel(AlbumModel* model)--begin line 34 ";

    mAlbumModel = model;
    ui->albumList->setModel(mAlbumModel);
    qDebug()<<" void AlbumListWidget::setModel(AlbumModel* model)--end line 38 ";

}

void AlbumListWidget::setSelectionModel(QItemSelectionModel* selectionModel)
{
    qDebug()<<" void AlbumListWidget::setSelectionModel(QItemSelectionModel* selectionModel)--begin line 44 ";

    ui->albumList->setSelectionModel(selectionModel);
    qDebug()<<" void AlbumListWidget::setSelectionModel(QItemSelectionModel* selectionModel)--end line 47 ";

}

void AlbumListWidget::createAlbum()
{
    qDebug()<<" void AlbumListWidget::createAlbum()--begin line 53 ";

    if(!mAlbumModel) {
        return;
    }

    bool ok;
    QString albumName = QInputDialog::getText(this,
                                              "Create a new Album",
                                              "Choose an name",
                                              QLineEdit::Normal,
                                              "New album",
                                              &ok);

    if (ok && !albumName.isEmpty()) {
        Album album(albumName);
        QModelIndex createdIndex = mAlbumModel->addAlbum(album);
        ui->albumList->setCurrentIndex(createdIndex);
    }
    qDebug()<<" void AlbumListWidget::createAlbum()--end line 72 ";

}
