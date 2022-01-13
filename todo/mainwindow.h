#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "task.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  void  updateToDoLabel();

public slots:
    void addTask(bool b);
    void removeTask(task* t);
    void statusChanged(bool b);
private:
    Ui::MainWindow *ui;
    QVector<task*> mTasks;
};

#endif // MAINWINDOW_H
