#include "AlbumDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Album.h"
#include "DatabaseManager.h"
#include <QDebug>
using namespace std;

AlbumDao::AlbumDao(QSqlDatabase& database) :
    mDatabase(database)
{
    qDebug()<<"AlbumDao::AlbumDao(QSqlDatabase& database) Construtor --begin line 15";
    qDebug()<<"AlbumDao::AlbumDao(QSqlDatabase& database) Construtor --end line 16";

}

void AlbumDao::init() const
{
    qDebug()<<"void AlbumDao::init() const --begin line 22";

    if (!mDatabase.tables().contains("albums")) {
        QSqlQuery query(mDatabase);
        query.exec("CREATE TABLE albums (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
        DatabaseManager::debugQuery(query);
    }
    qDebug()<<"void AlbumDao::init() const --end line 29";

}

void AlbumDao::addAlbum(Album& album) const
{
    qDebug()<<"void void AlbumDao::addAlbum(Album& album) const --begin line 35";

    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO albums (name) VALUES (:name)");
    query.bindValue(":name", album.name());
    query.exec();
    album.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
    qDebug()<<"void void AlbumDao::addAlbum(Album& album) const --end line 43";

}

void AlbumDao::updateAlbum(const Album& album)const
{
    qDebug()<<"void AlbumDao::updateAlbum(const Album& album)const --begin line 49";

    QSqlQuery query(mDatabase);
    query.prepare("UPDATE albums SET name = (:name) WHERE id = (:id)");
    query.bindValue(":name", album.name());
    query.bindValue(":id", album.id());
    query.exec();
    DatabaseManager::debugQuery(query);
    qDebug()<<"void AlbumDao::updateAlbum(const Album& album)const --end line 57";

}

void AlbumDao::removeAlbum(int id) const
{
    qDebug()<<"void AlbumDao::removeAlbum(int id) const --begin line 63";

    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM albums WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
    qDebug()<<"void AlbumDao::removeAlbum(int id) const --end line 70";

}

unique_ptr<vector<unique_ptr<Album>>> AlbumDao::albums() const
{
    qDebug()<<"unique_ptr<vector<unique_ptr<Album>>> AlbumDao::albums() const --begin line 76";

    QSqlQuery query("SELECT * FROM albums", mDatabase);
    query.exec();
    unique_ptr<vector<unique_ptr<Album>>> list(new vector<unique_ptr<Album>>());
    while(query.next()) {
        unique_ptr<Album> album(new Album());
        album->setId(query.value("id").toInt());
        album->setName(query.value("name").toString());
        list->push_back(move(album));
    }
    qDebug()<<"unique_ptr<vector<unique_ptr<Album>>> AlbumDao::albums() const --end line 87";

    return list;
}
