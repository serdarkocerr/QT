#include "dao.h"
#include <QDebug>

Dao::Dao(QObject *parent) : QObject(parent)
{
    qDebug()<<"Dao - constructor --begin";
    qDebug()<<"Dao - constructor --end";
}
Dao &Dao::instance()
{
    qDebug()<<"Dao instance   --begin";
    static Dao singleton;
    qDebug()<<"Dao instance   --end";
    return singleton;
}

Dao::~Dao()
{
    qDebug()<<"Dao - Deconstructor --begin";
    qDebug()<<"Dao - Deconstructor --end";
}

