#ifndef TASK_H
#define TASK_H

#include <QWidget>

namespace Ui {
class task;
}

class task : public QWidget
{
    Q_OBJECT
    
public:
    explicit task(QString& name, QWidget *parent = 0);
    ~task();
    void setName(const QString& name);
    bool isCompleted() const;
    QString name() const;
    bool isComplete=false;

public slots:
    void rename(bool b);
    void removeSlot(bool b);
    void checkboxSlot(bool b);
signals:
    void removed(task* t);
    void statusChangedSignal(bool b);
private:
    Ui::task *ui;
};

#endif // TASK_H
