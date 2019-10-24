#include "PictureModel.h"

#include "Album.h"
#include "DatabaseManager.h"
#include "AlbumModel.h"
#include <QDebug>
using namespace std;

PictureModel::PictureModel(const AlbumModel& albumModel, QObject* parent) :
    QAbstractListModel(parent),
    mDb(DatabaseManager::instance()),
    mAlbumId(-1),
    mPictures(new vector<unique_ptr<Picture>>())
{
    qDebug()<<"PictureModel::PictureModel(const AlbumModel& albumModel, QObject* parent) Constructor --begin line 15";

    connect(&albumModel, &AlbumModel::rowsRemoved,
            this, &PictureModel::deletePicturesForAlbum);

    qDebug()<<"PictureModel::PictureModel(const AlbumModel& albumModel, QObject* parent) Constructor --end line 20";

}

QModelIndex PictureModel::addPicture(const Picture& picture)
{
    qDebug()<<"QModelIndex PictureModel::addPicture(const Picture& picture) --begin line 26";

    int rows = rowCount();
    beginInsertRows(QModelIndex(), rows, rows);
    unique_ptr<Picture>newPicture(new Picture(picture));
    mDb.pictureDao.addPictureInAlbum(mAlbumId, *newPicture);
    mPictures->push_back(move(newPicture));
    endInsertRows();
    qDebug()<<"QModelIndex PictureModel::addPicture(const Picture& picture) --end line 34";

    return index(rows, 0);
}

int PictureModel::rowCount(const QModelIndex& /*parent*/) const
{
    qDebug()<<"int PictureModel::rowCount(const QModelIndex& /*parent*/) const --begin line 41";
    qDebug()<<"int PictureModel::rowCount(const QModelIndex& /*parent*/) const --end line 42";

    return mPictures->size();
}

QVariant PictureModel::data(const QModelIndex& index, int role) const
{
    qDebug()<<"QVariant PictureModel::data(const QModelIndex& index, int role) const --begin line 49";

    if (!isIndexValid(index)) {
        return QVariant();
    }

    const Picture& picture = *mPictures->at(index.row());
    switch (role) {
        case Qt::DisplayRole:
        qDebug()<<"QVariant PictureModel::data(const QModelIndex& index, int role) const Qt::DisplayRole --end line 58";

            return picture.fileUrl().fileName();
            break;

        case Roles::UrlRole:
        qDebug()<<"QVariant PictureModel::data(const QModelIndex& index, int role) const Roles::UrlRole --end line 64";

            return picture.fileUrl();
            break;

        case Roles::FilePathRole:
        qDebug()<<"QVariant PictureModel::data(const QModelIndex& index, int role) const Roles::FilePathRole --end line 70";

            return picture.fileUrl().toLocalFile();
            break;


        default:
        qDebug()<<"QVariant PictureModel::data(const QModelIndex& index, int role) const default --end line 77";

            return QVariant();
    }
}

bool PictureModel::removeRows(int row, int count, const QModelIndex& parent)
{
    qDebug()<<"bool PictureModel::removeRows(int row, int count, const QModelIndex& parent) --begin line 85";

    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while(countLeft--) {
        const Picture& picture = *mPictures->at(row + countLeft);
        mDb.pictureDao.removePicture(picture.id());
    }
    mPictures->erase(mPictures->begin() + row,
                    mPictures->begin() + row + count);
    endRemoveRows();

    qDebug()<<"bool PictureModel::removeRows(int row, int count, const QModelIndex& parent) --end line 104";

    return true;
}

QHash<int, QByteArray> PictureModel::roleNames() const
{
    qDebug()<<"QHash<int, QByteArray> PictureModel::roleNames() const --begin line 111";

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    roles[Roles::FilePathRole] = "filepath";
    roles[Roles::UrlRole] = "url";
    qDebug()<<"QHash<int, QByteArray> PictureModel::roleNames() const --end line 117";

    return roles;
}

void PictureModel::setAlbumId(int albumId)
{
    qDebug()<<"void PictureModel::setAlbumId(int albumId) --begin line 124";

    beginResetModel();
    mAlbumId = albumId;
    loadPictures(mAlbumId);
    endResetModel();
    qDebug()<<"void PictureModel::setAlbumId(int albumId) --end line 130";

}

void PictureModel::clearAlbum()
{
    qDebug()<<"void PictureModel::clearAlbum() --begin line 136";
    qDebug()<<"void PictureModel::clearAlbum() --end line 137";

    setAlbumId(-1);
}

void PictureModel::deletePicturesForAlbum()
{
    qDebug()<<"void PictureModel::deletePicturesForAlbum() --begin line 144";

    mDb.pictureDao.removePicturesForAlbum(mAlbumId);
    clearAlbum();
    qDebug()<<"void PictureModel::deletePicturesForAlbum() --end line 148";

}

void PictureModel::loadPictures(int albumId)
{
    qDebug()<<"void PictureModel::loadPictures(int albumId) --begin line 154";

    if (albumId <= 0) {
        mPictures.reset(new vector<unique_ptr<Picture>>());
        return;
    }
    mPictures = mDb.pictureDao.picturesForAlbum(albumId);
    qDebug()<<"void PictureModel::loadPictures(int albumId) --end line 161";

}

bool PictureModel::isIndexValid(const QModelIndex& index) const
{
    qDebug()<<"bool PictureModel::isIndexValid(const QModelIndex& index) const --begin line 167";

    if (index.row() < 0
            || index.row() >= rowCount()
            || !index.isValid()) {
        qDebug()<<"bool PictureModel::isIndexValid(const QModelIndex& index) const return false --end line 172";

        return false;
    }
    qDebug()<<"bool PictureModel::isIndexValid(const QModelIndex& index) const return true--end line 176";

    return true;
}
