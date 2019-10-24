#ifndef OGRENCIWIDGET_H
#define OGRENCIWIDGET_H

#include <QWidget>
#include <QIdentityProxyModel>
#include <QHash>
#include <QPixmap>
#include <QItemSelectionModel>
#include "ogrencimodel.h"
#include "sinifmodel.h"
namespace Ui {
class OgrenciWidget;
}

class OgrenciWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OgrenciWidget(QWidget *parent = nullptr);
    ~OgrenciWidget();

    void setOgrenciModel(OgrenciModel* ogrModel);
    void setOgrenciSelectionModel(QItemSelectionModel* ogrenciSelectionModel);

    //Sinif listesine tiklandiginda set ediliyor.
     void setSinifModel(SinifModel* model);
    void setSinifSelectionModel(QItemSelectionModel* selectionModel);
    void clearUi();
    void loadSinif(const QModelIndex& albumIndex);
        OgrenciModel* ogrenciModel() const;

public slots:
    void createOgrenci();
    void deleteOgrenci();
//    void reloadOgrenciIsimleri();
//    void generateOgrenciIsimleri(const QModelIndex& startIndex, int count);

private:
    Ui::OgrenciWidget *ui;
    OgrenciModel* oModel;//ogrenciModel
    QItemSelectionModel* oSelectionModel;//ogrenciSelection
    SinifModel* sModel;//sinifModel
    QItemSelectionModel* sSelectionModel;//sinifSelection

};

#endif // OGRENCIWIDGET_H
