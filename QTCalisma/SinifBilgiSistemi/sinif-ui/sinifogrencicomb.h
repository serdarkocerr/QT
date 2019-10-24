#ifndef SINIFOGRENCICOMB_H
#define SINIFOGRENCICOMB_H

#include <QWidget>
#include "sinifmodel.h"
#include <QItemSelectionModel>
#include <QAbstractListModel>
namespace Ui {
class SinifOgrenciComb;
}
class SinifWidget;
class OgrenciWidget;
class SinifModel;
class OgrenciModel;

class SinifOgrenciComb : public QWidget
{
    Q_OBJECT

public:
    explicit SinifOgrenciComb(QWidget *parent = nullptr);
    ~SinifOgrenciComb();

    void setSinifModel(SinifModel* sinModel);
    void setSinifSelectionModel(QItemSelectionModel* sinifSelectionModel);

    void setOgrenciModel(OgrenciModel* ogrenciModel);
    void setOgrenciSelectionModel(QItemSelectionModel* ogrenciSelectionModel);

private:
    Ui::SinifOgrenciComb *ui;
};

#endif // SINIFOGRENCICOMB_H
