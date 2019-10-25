#ifndef DAO_H
#define DAO_H

#include <QObject>

class Dao : public QObject
{
    Q_OBJECT

public:
    static Dao& instance();
       ~Dao();

protected:
    explicit Dao(QObject *parent = nullptr);
    Dao& operator=(const Dao& rhs);
signals:
    void signalForCallingManagerSlot();
public slots:
};

#endif // DAO_H
