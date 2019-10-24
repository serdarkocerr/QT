#include "AlbumModel.h"
#include <QDebug>
using namespace std;

AlbumModel::AlbumModel(QObject* parent) :
    QAbstractListModel(parent),
    mDb(DatabaseManager::instance()),
    mAlbums(mDb.albumDao.albums())
{
    qDebug()<<"AlbumModel::AlbumModel(QObject* parent)  Constructor-begin  line 10";
    qDebug()<<"AlbumModel::AlbumModel(QObject* parent)  Constructor-begin  line 11";

}

QModelIndex AlbumModel::addAlbum(const Album& album)
{
    qDebug()<<"QModelIndex AlbumModel::addAlbum(const Album& album)-begin line 14";
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Album> newAlbum(new Album(album));
    mDb.albumDao.addAlbum(*newAlbum);
    mAlbums->push_back(move(newAlbum));
    endInsertRows();
    return index(rowIndex, 0);
    qDebug()<<"QModelIndex AlbumModel::addAlbum(const Album& album)-end line 25";

}

int AlbumModel::rowCount(const QModelIndex& parent) const
{
    qDebug()<<" int AlbumModel::rowCount(const QModelIndex& parent) const -- begin line 31";

    Q_UNUSED(parent);
    qDebug()<<" int AlbumModel::rowCount(const QModelIndex& parent) const -- end line 32";

    return mAlbums->size();
}

QVariant AlbumModel::data(const QModelIndex& index, int role) const
{
    qDebug()<<" QVariant AlbumModel::data(const QModelIndex& index, int role) const -- begin line 41";

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const Album& album = *mAlbums->at(index.row());

    switch (role) {
        case Roles::IdRole:
        qDebug()<<" QVariant AlbumModel::data(const QModelIndex& index, int role) const -- end  -- Roles:IdRole line 50 ";
            return album.id();

        case Roles::NameRole:
        case Qt::DisplayRole:
        qDebug()<<" QVariant AlbumModel::data(const QModelIndex& index, int role) const -- end  -- Roles:IdRole OR Qt::DisplayRole: line 55 ";
            return album.name();

        default:
        qDebug()<<" QVariant AlbumModel::data(const QModelIndex& index, int role) const -- end  -- default line 59 ";

            return QVariant();
    }
}

bool AlbumModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    qDebug()<<" bool AlbumModel::setData(const QModelIndex& index, const QVariant& value, int role) --begin line 67";

    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    Album& album = *mAlbums->at(index.row());
    album.setName(value.toString());
    mDb.albumDao.updateAlbum(album);
    emit dataChanged(index, index);
    qDebug()<<" bool AlbumModel::setData(const QModelIndex& index, const QVariant& value, int role) --end  line 77";

    return true;
}

bool AlbumModel::removeRows(int row, int count, const QModelIndex& parent)
{
    qDebug()<<" bool AlbumModel::removeRows(int row, int count, const QModelIndex& parent) -- begin line 71";

    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const Album& album = *mAlbums->at(row + countLeft);
        mDb.albumDao.removeAlbum(album.id());
    }
    mAlbums->erase(mAlbums->begin() + row,
                  mAlbums->begin() + row + count);
    endRemoveRows();
    qDebug()<<" bool AlbumModel::removeRows(int row, int count, const QModelIndex& parent) -- end line 101";

    return true;
}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    qDebug()<<" QHash<int, QByteArray> AlbumModel::roleNames() const--begin  line 108";

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    qDebug()<<" QHash<int, QByteArray> AlbumModel::roleNames() const--end  line 113";

    return roles;
}

bool AlbumModel::isIndexValid(const QModelIndex& index) const
{
    qDebug()<<" bool AlbumModel::isIndexValid(const QModelIndex& index) const -- begin line 120";
    qDebug()<<" bool AlbumModel::isIndexValid(const QModelIndex& index) const -- end line 121";

    return index.isValid() && index.row() < rowCount();
}

