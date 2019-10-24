#include "ogrenci.h"

Ogrenci::Ogrenci(const QString &ogrenciName):
    ogrenciIsim(ogrenciName),
    ogrenciYas(-1),
    sinifNumarasi(-1)
{

}

QString Ogrenci::getOgrenciIsim()
{
    return ogrenciIsim;
}

void Ogrenci::setOgrenciIsim(QString isim)
{
    ogrenciIsim = isim;
}

void Ogrenci::setOgrenciYas(int yas)
{
    ogrenciYas = yas;
}

int Ogrenci::getOgrenciYas()
{
    return ogrenciYas;
}

int Ogrenci::getSinifNumarasi()
{
    return sinifNumarasi;
}

void Ogrenci::setOgrencisSinifNumarasi(int sinifNum)
{
    sinifNumarasi = sinifNum;
}
