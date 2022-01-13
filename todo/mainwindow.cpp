#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "task.h"
#include <QInputDialog>
#include <QDebug>
#include <vector>
#include <iostream>

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mTasks()
{
    ui->setupUi(this);
    //connect(ui->btnAddTask,&QPushButton::clicked(bool), this, addTask(bool));
    connect(ui->btnAddTask, SIGNAL(clicked(bool)),this,SLOT(addTask(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTask(bool b)
{
    qDebug() << "Adding new task";

    bool ok ;
    QString name = QInputDialog::getText(this,
                                         tr("Add task"),
                                         tr(" Task name"),
                                         QLineEdit::Normal,
                                         tr("Untitled task"), &ok);
    if(ok && !name.isEmpty()){

        qDebug() << "Adding new task";

        task* tsk = new task(name);
        connect(tsk,SIGNAL(removed(task*)),this,SLOT(removeTask(task *)));
        connect( tsk,SIGNAL(statusChangedSignal(bool)),this,SLOT(statusChanged(bool)) );
        mTasks.append(tsk);
        ui->taskLayout->addWidget(tsk);
    }
    updateToDoLabel();


}

void MainWindow::removeTask(task* t)
{
    QVector<task*>::iterator it;

    try {
        for(it=mTasks.begin();it!=mTasks.end();it++){
            if((*it) == t ){
                qDebug()<< "remove Task "<<t->name();
                mTasks.erase(it);
                break;
            }
        }
        ui->taskLayout->removeWidget(t);
        delete t;

    } catch (exception &e) {
        e.what();
    }
    updateToDoLabel();




}
void MainWindow::statusChanged(bool b)
{
    updateToDoLabel();
}

void MainWindow::updateToDoLabel(){
    int completedCount = 0;

    for(int i = 0; i< mTasks.size(); i++){
        if(mTasks.at(i)->isCompleted())
            completedCount++;
    }
    int todoCnt = mTasks.size()-completedCount;
    ui->result->setText(QString("Status : %1 todo / %2 completed").arg(todoCnt).arg(completedCount));

}
