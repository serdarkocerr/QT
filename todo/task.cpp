#include "task.h"
#include "ui_task.h"
#include <QInputDialog>

task::task(QString& name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::task)
{
    ui->setupUi(this);
    setName(name);

    connect(ui->editButton, SIGNAL(clicked(bool)), this,SLOT(rename(bool )));
    connect(ui->removeButton, SIGNAL(clicked(bool)),this,SLOT(removeSlot(bool)));
    connect(ui->checkBox, SIGNAL(toggled(bool)),this,SLOT(checkboxSlot(bool)));
}

task::~task()
{
    delete ui;
}

void task::setName(const QString &name)
{
    //this->name = name;
    ui->checkBox->setText(name);
}

bool task::isCompleted() const
{

    return ui->checkBox->isChecked();
}

QString task::name() const
{
    return ui->checkBox->text();
}


void task::rename(bool b)
{
    bool ok;
    QString value = QInputDialog::getText(this,
                                          tr("Edit task"),
                                          tr("Task name"),
                                          QLineEdit::Normal,
                                          this->name(),&ok);
    if(ok && !value.isEmpty()){
        setName(value);
    }

}

void task::removeSlot(bool b)
{
    emit removed(this);
}

void task::checkboxSlot(bool b)
{
    ui->checkBox->setChecked(b);
    QFont font(ui->checkBox->font());
    font.setStrikeOut(b);
    ui->checkBox->setFont(font);
    emit statusChangedSignal(b);
    this->isComplete = b;
}


