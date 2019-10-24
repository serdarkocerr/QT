#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ogrencimodel.h"
#include "sinifmodel.h"
#include <QItemSelectionModel>
namespace Ui {
class MainWindow;
}
class SinifOgrenciComb;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
     SinifOgrenciComb* mSinifOgrenciCombt;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
