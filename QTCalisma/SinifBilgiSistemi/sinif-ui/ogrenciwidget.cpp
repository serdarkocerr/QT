#include "ogrenciwidget.h"
#include "ui_ogrenciwidget.h"
#include <QModelIndex>
#include <QDebug>
#include <QInputDialog>
#include <QAbstractListModel>

extern QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex());

OgrenciWidget::OgrenciWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OgrenciWidget),
    oModel(nullptr),
    oSelectionModel(nullptr),
    sModel(nullptr),
    sSelectionModel(nullptr)
{
    ui->setupUi(this);

    connect(ui->ogranciEkleButton, &QPushButton::clicked,
            this, &OgrenciWidget::createOgrenci);
    connect(ui->ogrenciSilButton, &QPushButton::clicked,
            this, &OgrenciWidget::deleteOgrenci);

}

OgrenciWidget::~OgrenciWidget()
{
    delete ui;
}

void OgrenciWidget::setSinifModel(SinifModel *model)
{
    qDebug()<<"void OgrenciWidget::setSinifModel(SinifModel *model) --begin  ";
    sModel = model;
    connect(sModel, &QAbstractItemModel::dataChanged, [this] (const QModelIndex &topLeft) {
        if (topLeft == sSelectionModel->currentIndex()) {
            loadSinif(topLeft);
        }
    });
   qDebug()<<"void OgrenciWidget::setSinifModel(SinifModel *model) --end  ";
}

void OgrenciWidget::setSinifSelectionModel(QItemSelectionModel *selectionModel)
{
    qDebug()<<"OgrenciWidget::setSinifSelectionModel(QItemSelectionModel *selectionModel) --begin  ";
    sSelectionModel = selectionModel;
    connect(sSelectionModel, &QItemSelectionModel::selectionChanged, [this](const QItemSelection &selected) {
        if (selected.isEmpty()) {
            clearUi();
            return;
        }
        loadSinif(selected.indexes().first());
    });

    qDebug()<<"OgrenciWidget::setSinifSelectionModel(QItemSelectionModel *selectionModel) --end  ";

}

void OgrenciWidget::setOgrenciModel(OgrenciModel *ogrModel)
{
    qDebug()<<"void OgrenciWidget::setPictureModel(OgrenciModel* ogrModel) --begin  ";

    oModel = ogrModel;
    ui->ogrenciListView->setModel(oModel);
    qDebug()<<"void OgrenciWidget::setPictureModel(OgrenciModel* ogrModel --end   ";


}

void OgrenciWidget::setOgrenciSelectionModel(QItemSelectionModel *ogrenciSelectionModel)
{
    qDebug()<<"void OgrenciWidget::setOgrenciSelectionModel(QItemSelectionModel* ogrenciSelectionModel) --begin  ";
    oSelectionModel = ogrenciSelectionModel;
    ui->ogrenciListView->setSelectionModel(ogrenciSelectionModel);
    qDebug()<<"void OgrenciWidget::setOgrenciSelectionModel(QItemSelectionModel* ogrenciSelectionModel) --end  ";

}

void OgrenciWidget::createOgrenci()
{
    qDebug()<<" void OgrenciWidget::createOgrenci()--begin  ";

    if(!oModel) {
        return;
    }

    bool ok;
    QString ogrencNme = QInputDialog::getText(this,
                                              "Yeni ogrenci olustur",
                                              "Ogrenci Ismi",
                                              QLineEdit::Normal,
                                              "Yeni Ogrenci",
                                              &ok);

    if (ok && !ogrencNme.isEmpty()) {
        Ogrenci ogrencii(ogrencNme);
        ogrencii.setOgrencisSinifNumarasi(sSelectionModel->currentIndex().row());
        QModelIndex createdIndex = oModel->addOgrenci(ogrencii);
        ui->ogrenciListView->setCurrentIndex(createdIndex);
    }
    qDebug()<<" void OgrenciWidget::createOgrenci()--end  ";
}

void OgrenciWidget::deleteOgrenci()
{
     qDebug()<<" void OgrenciWidget::deleteOgrenci()--begin  ";
     QModelIndex currIndex = ui->ogrenciListView->currentIndex();
     oModel->removeRow(currIndex.row());
     qDebug()<<" void OgrenciWidget::deleteOgrenci()--end  ";

}

void OgrenciWidget::clearUi()
{
    qDebug()<<"void OgrenciWidget::clearUi() --begin   ";

    ui->ogrenciSilButton->setVisible(false);
    qDebug()<<"void OgrenciWidget::clearUi() --end line   ";

}
void OgrenciWidget::loadSinif(const QModelIndex& sinifIndex)
{
    qDebug()<<"void OgrenciWidget::loadSinif(const QModelIndex& albumIndex) --begin   ";
    oModel->setSinifNum(sModel->data(sinifIndex,SinifModel::Roles::IdRole).toInt());
 //   oModel->setSinifNum(sModel->callResetAlbumBeforeData(sinifIndex,SinifModel::Roles::IdRole).toInt());



    qDebug()<<"void OgrenciWidget::loadSinif(const QModelIndex& albumIndex) --end   ";

}
OgrenciModel* OgrenciWidget::ogrenciModel() const
{
    return oModel;
}
