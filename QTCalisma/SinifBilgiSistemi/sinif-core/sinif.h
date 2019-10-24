#ifndef SINIF_H
#define SINIF_H

#include "sinif-core_global.h"
#include <QString>
 static  int sinifNumarasiDegeri = 0;
class SINIFCORESHARED_EXPORT sinif
{
public:

    explicit sinif( const QString& sinifName = "");
    void setSinifAdi(QString sinfAdi);
    void setSinifNumarasi(int sinfNumarasi);
   QString getSinifAdi();
   int getSinifNumarasi();
private:
    QString sinifAdi;
    int sinifNumarasi;

};

#endif // SINIF_H
