#include "PictureDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Picture.h"
#include "DatabaseManager.h"
#include <QDebug>
using namespace std;

PictureDao::PictureDao(QSqlDatabase& database) :
    mDatabase(database)
{
    qDebug()<<" PictureDao::PictureDao(QSqlDatabase& database)  Constructor -- begin line 15";
    qDebug()<<" PictureDao::PictureDao(QSqlDatabase& database)  Constructor -- end line 16";


}

void PictureDao::init() const
{
    qDebug()<<" void PictureDao::init() const --begin  line 23";

    if (!mDatabase.tables().contains("pictures")) {
        QSqlQuery query(mDatabase);
        query.exec(QString("CREATE TABLE pictures")
        + " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        + "album_id INTEGER, "
        + "url TEXT)");
        DatabaseManager::debugQuery(query);
    }
    qDebug()<<" void PictureDao::init() const --end  line 33";

}

void PictureDao::addPictureInAlbum(int albumId, Picture& picture) const
{
    qDebug()<<" void PictureDao::addPictureInAlbum(int albumId, Picture& picture) const --begin line 39";

    QSqlQuery query(mDatabase);
    query.prepare(QString("INSERT INTO pictures")
        + " (album_id, url)"
        + " VALUES ("
        + ":album_id, "
        + ":url"
        + ")");
    query.bindValue(":album_id", albumId);
    query.bindValue(":url", picture.fileUrl());
    query.exec();
    DatabaseManager::debugQuery(query);
    picture.setId(query.lastInsertId().toInt());
    picture.setAlbumId(albumId);
    qDebug()<<" void PictureDao::addPictureInAlbum(int albumId, Picture& picture) const --end line 54";

}

void PictureDao::removePicture(int id) const
{
    qDebug()<<" void PictureDao::removePicture(int id) const --begin line 60";

    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM pictures WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
    qDebug()<<" void PictureDao::removePicture(int id) const --end line 67";

}

void PictureDao::removePicturesForAlbum(int albumId) const
{
    qDebug()<<" void PictureDao::removePicturesForAlbum(int albumId) const --begin line 73";

    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM pictures WHERE album_id = (:album_id)");
    query.bindValue(":album_id", albumId);
    query.exec();
    DatabaseManager::debugQuery(query);
    qDebug()<<" void PictureDao::removePicturesForAlbum(int albumId) const --end line 79";

}

unique_ptr<vector<unique_ptr<Picture>>> PictureDao::picturesForAlbum(int albumId) const
{
    qDebug()<<" unique_ptr<vector<unique_ptr<Picture>>> PictureDao::picturesForAlbum(int albumId) const --begin line 86";

    QSqlQuery query(mDatabase);
    query.prepare("SELECT * FROM pictures WHERE album_id = (:album_id)");
    query.bindValue(":album_id", albumId);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Picture>>> list(new vector<unique_ptr<Picture>>());
    while(query.next()) {
        unique_ptr<Picture> picture(new Picture());
        picture->setId(query.value("id").toInt());
        picture->setAlbumId(query.value("album_id").toInt());
        picture->setFileUrl(query.value("url").toString());
        list->push_back(move(picture));
    }
    qDebug()<<" unique_ptr<vector<unique_ptr<Picture>>> PictureDao::picturesForAlbum(int albumId) const --end line 101";

    return list;
}
