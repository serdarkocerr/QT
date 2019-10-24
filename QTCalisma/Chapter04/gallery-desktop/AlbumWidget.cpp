#include "AlbumWidget.h"
#include "ui_AlbumWidget.h"

#include <QInputDialog>
#include <QFileDialog>

#include "AlbumModel.h"
#include "PictureModel.h"
#include "ThumbnailProxyModel.h"

#include "PictureDelegate.h"
#include <QDebug>
AlbumWidget::AlbumWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumWidget),
    mAlbumModel(nullptr),
    mAlbumSelectionModel(nullptr),
    mPictureModel(nullptr),
    mPictureSelectionModel(nullptr)
{
    qDebug()<<" AlbumWidget::AlbumWidget(QWidget *parent) :  Constructorline --begin 21  ";

    ui->setupUi(this);
    clearUi();

    ui->thumbnailListView->setSpacing(5);
    ui->thumbnailListView->setResizeMode(QListView::Adjust);
    ui->thumbnailListView->setFlow(QListView::LeftToRight);
    ui->thumbnailListView->setWrapping(true);
    ui->thumbnailListView->setItemDelegate(new PictureDelegate(this));

    connect(ui->thumbnailListView, &QListView::doubleClicked,
            this, &AlbumWidget::pictureActivated);

    connect(ui->deleteButton, &QPushButton::clicked,
            this, &AlbumWidget::deleteAlbum);

    connect(ui->editButton, &QPushButton::clicked,
            this, &AlbumWidget::editAlbum);

    connect(ui->addPicturesButton, &QPushButton::clicked,
            this, &AlbumWidget::addPictures);

    qDebug()<<" AlbumWidget::AlbumWidget(QWidget *parent) :  Constructorline --end 44  ";

}

AlbumWidget::~AlbumWidget()
{
    qDebug()<<" AlbumWidget::~AlbumWidget --begin line 50  ";
    qDebug()<<" AlbumWidget::~AlbumWidget --end line 51  ";
    delete ui;
}

void AlbumWidget::setAlbumModel(AlbumModel* albumModel)
{
    qDebug()<<" void AlbumWidget::setAlbumModel(AlbumModel* albumModel) --begin line 57  ";

    mAlbumModel = albumModel;

    connect(mAlbumModel, &QAbstractItemModel::dataChanged, [this] (const QModelIndex &topLeft) {
        if (topLeft == mAlbumSelectionModel->currentIndex()) {
            loadAlbum(topLeft);
        }
    });
    qDebug()<<" void AlbumWidget::setAlbumModel(AlbumModel* albumModel) --end line 66  ";

}

void AlbumWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel)
{
    qDebug()<<"void AlbumWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel) --begin line 72  ";

    mAlbumSelectionModel = albumSelectionModel;

    connect(mAlbumSelectionModel, &QItemSelectionModel::selectionChanged, [this](const QItemSelection &selected) {
        if (selected.isEmpty()) {
            clearUi();
            return;
        }
        loadAlbum(selected.indexes().first());
    });
    qDebug()<<"void AlbumWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel) --end line 83  ";

}

void AlbumWidget::setPictureModel(ThumbnailProxyModel* pictureModel)
{
    qDebug()<<"void AlbumWidget::setPictureModel(ThumbnailProxyModel* pictureModel) --begin line 89  ";

    mPictureModel = pictureModel;
    ui->thumbnailListView->setModel(pictureModel);
    qDebug()<<"void AlbumWidget::setPictureModel(ThumbnailProxyModel* pictureModel) --end line 93  ";

}

void AlbumWidget::setPictureSelectionModel(QItemSelectionModel* selectionModel)
{
    qDebug()<<"void AlbumWidget::setPictureSelectionModel(QItemSelectionModel* selectionModel --begin line 99  ";

    ui->thumbnailListView->setSelectionModel(selectionModel);
    qDebug()<<"void AlbumWidget::setPictureSelectionModel(QItemSelectionModel* selectionModel --end line 102  ";

}

void AlbumWidget::deleteAlbum()
{
    qDebug()<<"void AlbumWidget::deleteAlbum() --begin line 108  ";

    if (mAlbumSelectionModel->selectedIndexes().isEmpty()) {
        return;
    }
    int row = mAlbumSelectionModel->currentIndex().row();
    mAlbumModel->removeRow(row);

    // Try to select the previous album
    QModelIndex previousModelIndex = mAlbumModel->index(row - 1, 0);
    if(previousModelIndex.isValid()) {
        mAlbumSelectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
        qDebug()<<"void AlbumWidget::deleteAlbum() --end line 120  ";

        return;
    }

    // Try to select the next album
    QModelIndex nextModelIndex = mAlbumModel->index(row, 0);
    if(nextModelIndex.isValid()) {
        mAlbumSelectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
        qDebug()<<"void AlbumWidget::deleteAlbum() --end line 129  ";

        return;
    }
}

void AlbumWidget::editAlbum()
{
    qDebug()<<"void AlbumWidget::editAlbum() --begin line 137  ";

    if (mAlbumSelectionModel->selectedIndexes().isEmpty()) {
        return;
    }
    QModelIndex currentAlbumIndex = mAlbumSelectionModel->selectedIndexes().first();
    QString oldAlbumName = mAlbumModel->data(currentAlbumIndex, AlbumModel::Roles::NameRole).toString();

    bool ok;
    QString newName = QInputDialog::getText(this,
                                            "Album's name",
                                            "Change Album name",
                                            QLineEdit::Normal,
                                            oldAlbumName,
                                            &ok);

    if (ok && !newName.isEmpty()) {
        mAlbumModel->setData(currentAlbumIndex, newName, AlbumModel::Roles::NameRole);
    }
    qDebug()<<"void AlbumWidget::editAlbum() --end line 156  ";

}

void AlbumWidget::addPictures()
{
    qDebug()<<"void AlbumWidget::addPictures() --begin line 162  ";

    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "Add pictures",
                                                          QDir::homePath(),
                                                          "Picture files (*.jpg *.png)");
    if (!filenames.isEmpty()) {
        QModelIndex lastModelIndex;
        for (auto filename : filenames) {
            Picture picture(filename);
            lastModelIndex = mPictureModel->pictureModel()->addPicture(picture);
        }
        ui->thumbnailListView->setCurrentIndex(lastModelIndex);
    }
    qDebug()<<"void AlbumWidget::addPictures() --end line 176  ";

}

void AlbumWidget::clearUi()
{
    qDebug()<<"void AlbumWidget::clearUi() --begin line 182  ";

    ui->albumName->setText("");
    ui->deleteButton->setVisible(false);
    ui->editButton->setVisible(false);
    ui->addPicturesButton->setVisible(false);
    qDebug()<<"void AlbumWidget::clearUi() --end line 188  ";

}

void AlbumWidget::loadAlbum(const QModelIndex& albumIndex)
{
    qDebug()<<"void AlbumWidget::loadAlbum(const QModelIndex& albumIndex) --begin line 194  ";

    mPictureModel->pictureModel()->setAlbumId(mAlbumModel->data(albumIndex, AlbumModel::Roles::IdRole).toInt());
    ui->albumName->setText(mAlbumModel->data(albumIndex, Qt::DisplayRole).toString());
    ui->deleteButton->setVisible(true);
    ui->editButton->setVisible(true);
    ui->addPicturesButton->setVisible(true);
    qDebug()<<"void AlbumWidget::loadAlbum(const QModelIndex& albumIndex) --end line 201  ";

}
