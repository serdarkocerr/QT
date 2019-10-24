#include "sinifogrencicomb.h"
#include "ui_sinifogrencicomb.h"

SinifOgrenciComb::SinifOgrenciComb(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SinifOgrenciComb)
{
    ui->setupUi(this);
}

SinifOgrenciComb::~SinifOgrenciComb()
{
    delete ui;
}

void SinifOgrenciComb::setSinifModel(SinifModel *sinModel)
{
    ui->sinifwidget->setModel(sinModel);
    ui->ogrencwidget->setSinifModel(sinModel);
}

void SinifOgrenciComb::setSinifSelectionModel(QItemSelectionModel *sinifSelectionModel)
{
    ui->sinifwidget->setSelectionModel(sinifSelectionModel);
    ui->ogrencwidget->setSinifSelectionModel(sinifSelectionModel);
}

void SinifOgrenciComb::setOgrenciModel(OgrenciModel *ogrenciModel)
{
    ui->ogrencwidget->setOgrenciModel(ogrenciModel);
    ogrenciModel->reloadOgrenciler(ogrenciModel);

}

void SinifOgrenciComb::setOgrenciSelectionModel(QItemSelectionModel *ogrenciSelectionModel)
{
    ui->ogrencwidget->setOgrenciSelectionModel(ogrenciSelectionModel);
}
