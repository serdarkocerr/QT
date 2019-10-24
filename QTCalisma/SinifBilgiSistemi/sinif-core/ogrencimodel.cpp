#include "ogrencimodel.h"
#include <vector>
#include <memory>
using namespace std;
#include <QDebug>
OgrenciModel::OgrenciModel(QObject *parent) : QAbstractListModel (parent),
ogrenciler(new vector<unique_ptr<Ogrenci>>()),
butunOgrenciler(new vector<unique_ptr<Ogrenci>>())
{

}

QModelIndex OgrenciModel::addOgrenci(const Ogrenci &ogrenci)
{
    qDebug()<<"QModelIndex OgrenciModel::addOgrenci(const Ogrenci& ogrenci) --begin ";

    int rows = rowCount();
    beginInsertRows(QModelIndex(), rows, rows);
    unique_ptr<Ogrenci> newOgrenci(new Ogrenci(ogrenci));  
    unique_ptr<Ogrenci> butunOgrenci(new Ogrenci(newOgrenci->getOgrenciIsim()));
    butunOgrenci->setOgrenciYas(newOgrenci->getOgrenciYas());
    butunOgrenci->setOgrencisSinifNumarasi(newOgrenci->getSinifNumarasi());
    ogrenciler->push_back(move(newOgrenci));
    butunOgrenciler->push_back(move(butunOgrenci));
    endInsertRows();
    qDebug()<<"QModelIndex OgrenciModel::addOgrenci(const Ogrenci& ogrenci) --end ";

    return index(rows, 0);

}

int OgrenciModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ogrenciler->size();
}

QVariant OgrenciModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<"QVariant OgrenciModel::data(const QModelIndex& index, int role) const --begin  ";

     Ogrenci& ogr = *ogrenciler->at(index.row());
    switch (role) {
        case Qt::DisplayRole:
        qDebug()<<"QVariant PictureModel::data(const QModelIndex& index, int role) const Qt::DisplayRole --end line 58";

            return ogr.getOgrenciIsim();
            break;
        default:
        qDebug()<<"QVariant PictureModel::data(const QModelIndex& index, int role) const default --end line 77";

            return QVariant();
}
}
//bool OgrenciModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{

//}

bool OgrenciModel::removeRows(int row, int count, const QModelIndex &parent)
{

    qDebug()<<"bool OgrenciModel::removeRows(int row, int count, const QModelIndex& parent) --begin ";

    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    ogrenciler->erase(ogrenciler->begin() + row,
                    ogrenciler->begin() + row + count);
    endRemoveRows();

    qDebug()<<"bool OgrenciModel::removeRows(int row, int count, const QModelIndex& parent) --end ";


}

void OgrenciModel::setSinifNum(int sinifNum)
{
    qDebug()<<"void OgrenciModel::setSinifNum(int sinifNum) --begin ";

    beginResetModel();
    sinifNumb = sinifNum;
    loadOgrenciler(sinifNumb);
    endResetModel();
    qDebug()<<"void PictureModel::setAlbumId(int albumId) --end ";
}

void OgrenciModel::reloadOgrenciler(OgrenciModel *ogrenciModel)
{
    connect(ogrenciModel, &QAbstractItemModel::modelReset, [this] {
        realoadingTOgrenciler();
    });

    connect(ogrenciModel, &QAbstractItemModel::rowsInserted, [this](const QModelIndex& parent, int first, int last) {
      //  generateThumbnails(index(first, 0), last - first + 1);
        realoadingTOgrenciler();
    });

}



void OgrenciModel::loadOgrenciler(int sinifNum)
{

    qDebug()<<"void OgrenciModel::loadOgrenciler(int sinifNum) --begin ";

    if (sinifNum <= 0) {
        ogrenciler.reset(new vector<unique_ptr<Ogrenci>>());
        return;
    }
   // mPictures = mDb.pictureDao.picturesForAlbum(albumId);

    qDebug()<<"void PictureModel::loadPictures(int albumId) --end ";

}
QHash<int, QByteArray> OgrenciModel::roleNames() const
{
    qDebug()<<"QHash<int, QByteArray> PictureModel::roleNames() const --begin line 111";

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    roles[Roles::FilePathRole] = "filepath";
    roles[Roles::UrlRole] = "url";
    qDebug()<<"QHash<int, QByteArray> PictureModel::roleNames() const --end line 117";

    return roles;
}

void OgrenciModel::realoadingTOgrenciler()
{

    qDebug()<<"void OgrenciModel::reloadTOgrenciler() --begin   ";
    std::unique_ptr<std::vector<std::unique_ptr<Ogrenci>>> ogrencilerYedek;
//    ogrencilerYedek = std::move(ogrenciler);
//    ogrenciler->clear();
//    ogrenciler = std::move(ogrencilerYedek);

}
