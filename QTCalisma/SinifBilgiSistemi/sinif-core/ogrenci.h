#ifndef OGRENCI_H
#define OGRENCI_H
#include <QString>
#include "sinif-core_global.h"
#include <QObject>

class SINIFCORESHARED_EXPORT Ogrenci
{
public:
   explicit  Ogrenci(const QString& ogrenciName = "");
    QString getOgrenciIsim();
    void setOgrenciIsim(QString isim);
    void setOgrenciYas(int yas);
    int getOgrenciYas();
    int getSinifNumarasi();//FK
    void setOgrencisSinifNumarasi(int sinifNum);

private:
    QString ogrenciIsim;
    int ogrenciYas;
    int sinifNumarasi;

};

#endif // OGRENCI_H
