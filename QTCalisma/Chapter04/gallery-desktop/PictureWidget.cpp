#include "PictureWidget.h"
#include "ui_PictureWidget.h"

#include "PictureModel.h"
#include "ThumbnailProxyModel.h"
#include <QDebug>
PictureWidget::PictureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureWidget),
    mModel(nullptr),
    mSelectionModel(nullptr)
{
    qDebug()<<"PictureWidget::PictureWidget(QWidget *parent) Constructor --begin line 13  ";

    ui->setupUi(this);
    ui->pictureLabel->setMinimumSize(1, 1);

    connect(ui->backButton, &QPushButton::clicked,
        this, &PictureWidget::backToGallery);

    connect(ui->deleteButton, &QPushButton::clicked,
            this, &PictureWidget::deletePicture);

    connect(ui->previousButton, &QPushButton::clicked, [this] () {
        QModelIndex currentModelIndex = mSelectionModel->currentIndex();
        QModelIndex previousModelIndex = mSelectionModel->model()->index(currentModelIndex.row() - 1, 0);
        mSelectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
    });

    connect(ui->nextButton, &QPushButton::clicked, [this] () {
        QModelIndex currentModelIndex = mSelectionModel->currentIndex();
        QModelIndex nextModelIndex = mSelectionModel->model()->index(currentModelIndex.row() + 1, 0);
        mSelectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
    });
    qDebug()<<"PictureWidget::PictureWidget(QWidget *parent) Constructor --end line 35  ";

}

PictureWidget::~PictureWidget()
{
    qDebug()<<"PictureWidget::~PictureWidget()  --begin line 41  ";
    qDebug()<<"PictureWidget::~PictureWidget()  --end line 42  ";

    delete ui;
}

void PictureWidget::setModel(ThumbnailProxyModel* model)
{
    qDebug()<<"void PictureWidget::setModel(ThumbnailProxyModel* model)  --begin line 49  ";
    mModel = model;
    qDebug()<<"void PictureWidget::setModel(ThumbnailProxyModel* model)  --end line 51  ";

}

void PictureWidget::setSelectionModel(QItemSelectionModel* selectionModel)
{
    qDebug()<<"void PictureWidget::setSelectionModel(QItemSelectionModel* selectionModel) --begin  line 57  ";

    mSelectionModel = selectionModel;
    if (!mSelectionModel) {
        return;
    }
    connect(mSelectionModel, &QItemSelectionModel::selectionChanged, this, &PictureWidget::loadPicture);
    qDebug()<<"void PictureWidget::setSelectionModel(QItemSelectionModel* selectionModel) --end  line 64  ";

}

void PictureWidget::resizeEvent(QResizeEvent* event)
{
    qDebug()<<"void PictureWidget::resizeEvent(QResizeEvent* event) --begin line 70  ";

    QWidget::resizeEvent(event);
    updatePicturePixmap();
    qDebug()<<"void PictureWidget::resizeEvent(QResizeEvent* event) --end line 74  ";

}

void PictureWidget::deletePicture()
{
    qDebug()<<"void PictureWidget::deletePicture()  --begin line 80  ";

    // Remove the current picture
    int row = mSelectionModel->currentIndex().row();
    mModel->removeRow(mSelectionModel->currentIndex().row());

    // Try to select the previous picture
    QModelIndex previousModelIndex = mModel->index(row - 1, 0);
    if(previousModelIndex.isValid()) {
        mSelectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
        qDebug()<<"void PictureWidget::deletePicture()  --end line 90  ";

        return;
    }

    // Try to select the next picture
    QModelIndex nextModelIndex = mModel->index(row, 0);
    if(nextModelIndex.isValid()) {
        mSelectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
        qDebug()<<"void PictureWidget::deletePicture()  --end line 99  ";

        return;
    }

    emit backToGallery();
    qDebug()<<"void PictureWidget::deletePicture()  --end line 105  ";

}

void PictureWidget::loadPicture(const QItemSelection& selected)
{
    qDebug()<<"void PictureWidget::loadPicture(const QItemSelection& selected)  --begin line 111  ";

    if (selected.indexes().isEmpty()) {
        ui->nameLabel->setText("");
        ui->pictureLabel->setPixmap(QPixmap());
        ui->deleteButton->setEnabled(false);
        qDebug()<<"void PictureWidget::loadPicture(const QItemSelection& selected)  --end line 117  ";

        return;
    }

    QModelIndex current = selected.indexes().at(0);
    mPixmap = QPixmap(mModel->data(current, PictureModel::Roles::FilePathRole).toString());

    ui->nameLabel->setText(mModel->data(current, Qt::DisplayRole).toString());
    updatePicturePixmap();

    ui->previousButton->setEnabled(current.row() > 0);
    ui->nextButton->setEnabled(current.row() < (mModel->rowCount() - 1));
    ui->deleteButton->setEnabled(true);
    qDebug()<<"void PictureWidget::loadPicture(const QItemSelection& selected)  --end line 131  ";

}

void PictureWidget::updatePicturePixmap()
{
    qDebug()<<"void PictureWidget::updatePicturePixmap()  --begin line 137  ";

    if (mPixmap.isNull()) {
        return;
    }
    ui->pictureLabel->setPixmap(mPixmap.scaled(ui->pictureLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    qDebug()<<"void PictureWidget::updatePicturePixmap()  --end line 143  ";

}
