#ifndef SINIFWIDGET_H
#define SINIFWIDGET_H

#include <QWidget>
#include "sinifmodel.h"
#include <QItemSelectionModel>
namespace Ui {
class SinifWidget;
}

class SinifWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SinifWidget(QWidget *parent = 0);
    ~SinifWidget();

    void setModel(SinifModel* model);
    void setSelectionModel(QItemSelectionModel* selectionModel);

private slots:
    void createSinif();
    void deleteSinif();

private:

private:
    Ui::SinifWidget *ui;
    SinifModel* mdl;

};

#endif // SINIFWIDGET_H
