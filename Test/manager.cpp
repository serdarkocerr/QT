#include "manager.h"
#include <QtDebug>
#include <QObject>
Manager::Manager(QObject *parent): QObject(parent),
    daoCl(Dao::instance())
{
    qDebug()<<"Manager constructor -- begin";

    /*iki sekildede signal slot mekanizmasi kurulabilir.*/
    //QObject::connect(&daoCl,&Dao::signalForCallingManagerSlot,this,&Manager::slotForBeTriggredFromDaoSignal);
    QObject::connect(&Dao::instance(),&Dao::signalForCallingManagerSlot,this,&Manager::slotForBeTriggredFromDaoSignal);

    qDebug()<<"Manager constructor -- end";
}

void Manager::slotForBeTriggredFromDaoSignal()
{
    qDebug()<<"Manager slotForBeTriggredFromDaoSignal -- begin";
    qDebug()<<"Manager slotForBeTriggredFromDaoSignal -- end";
}
Manager &Manager::instance()
{
    qDebug()<<"Manager - instance --begin";
     static Manager singleton;
    qDebug()<<"Dao - instance --end";
    return singleton;
}

Manager::~Manager()
{
    qDebug()<<"Manager - Deconstructor --begin";
    qDebug()<<"Manager - Deconstructor --end";
}
