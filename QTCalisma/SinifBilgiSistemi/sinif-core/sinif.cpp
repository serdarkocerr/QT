#include "sinif.h"

sinif::sinif(const QString& sinifName):
    sinifAdi(sinifName),
    sinifNumarasi(sinifNumarasiDegeri++)
{

}

void sinif::setSinifAdi(QString sinfAdi)
{
    sinifAdi = sinfAdi;
}

void sinif::setSinifNumarasi(int sinfNumarasi)
{
    sinifNumarasi = sinfNumarasi;
}

QString sinif::getSinifAdi()
{
    return sinifAdi;
}

int sinif::getSinifNumarasi()
{
    return sinifNumarasi;
}
