#include "sinifwidget.h"
#include "ui_sinifwidget.h"
#include <QDebug>
#include <QInputDialog>
SinifWidget::SinifWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SinifWidget),
    mdl(nullptr)
{
    qDebug()<<" SinifWidget::SinifWidget(QWidget *parent)  Constructor--begin  ";
    ui->setupUi(this);
    connect(ui->sinifEklemeButton, &QPushButton::clicked,
            this, &SinifWidget::createSinif);
    connect(ui->sinifSilmeButton, &QPushButton::clicked,
            this, &SinifWidget::deleteSinif);
    qDebug()<<" SinifWidget::SinifWidget(QWidget *parent)  Constructor --end  ";


}

SinifWidget::~SinifWidget()
{
    qDebug()<<" SinifWidget::~SinifWidget()  DEConstructor --begin   ";
    qDebug()<<" SinifWidget::~SinifWidget()  DEConstructor --end   ";

    delete ui;
}

void SinifWidget::setModel(SinifModel *model)
{
    qDebug()<<" void SinifWidget::setModel(AlbumModel* model)--begin ";

    mdl = model;
    ui->sinifList->setModel(mdl);
    qDebug()<<" void SinifWidget::setModel(AlbumModel* model)--end  ";

}

void SinifWidget::setSelectionModel(QItemSelectionModel *selectionModel)
{
    qDebug()<<" void SinifWidget::setSelectionModel(QItemSelectionModel* selectionModel)--begin line 44 ";

    ui->sinifList->setSelectionModel(selectionModel);
    qDebug()<<" void SinifWidget::setSelectionModel(QItemSelectionModel* selectionModel)--end line 47 ";

}

void SinifWidget::createSinif()
{
    qDebug()<<" void SinifWidget::createSinif()--begin  ";

    if(!mdl) {
        return;
    }

    bool ok;
    QString snfName = QInputDialog::getText(this,
                                              "Yeni sinif olustur",
                                              "Bir isim belirle",
                                              QLineEdit::Normal,
                                              "Yeni Sinif",
                                              &ok);

    if (ok && !snfName.isEmpty()) {
        sinif snf(snfName);
        QModelIndex createdIndex = mdl->addSinif(snf);
        ui->sinifList->setCurrentIndex(createdIndex);
    }
    qDebug()<<" void SinifWidget::createSinif()--end  ";
}

void SinifWidget::deleteSinif()
{
     qDebug()<<" void SinifWidget::deleteSinif()--begin  ";
   QModelIndex currIndex = ui->sinifList->currentIndex();
    mdl->removeRow(currIndex.row());
    qDebug()<<" void SinifWidget::deleteSinif()--end  ";

}
