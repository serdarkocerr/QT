#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include "dao.h"
class Manager:public QObject
{
    Q_OBJECT
public:
    static Manager& instance();
       ~Manager();
    Dao& daoCl;
protected:
   explicit  Manager(QObject *parent = nullptr);

public slots:
    void slotForBeTriggredFromDaoSignal();


};

#endif // MANAGER_H
