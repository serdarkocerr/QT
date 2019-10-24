#include "sinifmodel.h"
#include "sinif.h"
#include <vector>
#include <memory>
using namespace std;
#include <QDebug>


SinifModel::SinifModel(QObject *parent) : QAbstractListModel (parent),
    siniflar(new vector<unique_ptr<sinif>>())
{
    qDebug()<<"SinifModel::SinifModel(QObject* parent)  Constructor-begin   ";
    qDebug()<<"SinifModel::SinifModel(QObject* parent)  Constructor-begin   ";

}

QModelIndex SinifModel::addSinif(const sinif &s)
{
    qDebug()<<"QModelIndex SinifModel::addSinif(const sinif& sinif)-begin  ";
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<sinif> newsinif(new sinif(s));
    siniflar->push_back(move(newsinif));
    endInsertRows();
    return index(rowIndex, 0);
    qDebug()<<"QModelIndex SinifModel::addSinif(const sinif& sinif)-end  ";
}

int SinifModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return siniflar->size();
}

QVariant SinifModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<" QVariant SinifModel::data(const QModelIndex& index, int role) const -- begin  27";

     sinif& sinif = *siniflar->at(index.row());

    switch (role) {
        case Roles::IdRole:
        qDebug()<<" QVariant SinifModel::data(const QModelIndex& index, int role) const -- end  -- Roles:IdRole  33 ";
            return sinif.getSinifNumarasi();

        case Roles::NameRole:
        case Qt::DisplayRole:
        qDebug()<<" QVariant SinifModel::data(const QModelIndex& index, int role) const -- end  -- Roles:IdRole OR Qt::DisplayRole:  38 ";
            return sinif.getSinifAdi();

        default:
        qDebug()<<" QVariant SinifModel::data(const QModelIndex& index, int role) const -- end  -- default  42 ";

            return QVariant();
    }
}
bool SinifModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug()<<" bool SinifModel::setData(const QModelIndex& index, const QVariant& value, int role) --begin  49";

    sinif& sinif = *siniflar->at(index.row());
    sinif.setSinifAdi(value.toString());
    emit dataChanged(index, index);
    qDebug()<<" bool SinifModel::setData(const QModelIndex& index, const QVariant& value, int role) --end   54";

    return true;
}

bool SinifModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug()<<" bool SinifModel::removeRows(int row, int count, const QModelIndex& parent) -- begin  61";

    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);

    siniflar->erase(siniflar->begin() + row,
                  siniflar->begin() + row + count);
    endRemoveRows();
    qDebug()<<" bool SinifModel::removeRows(int row, int count, const QModelIndex& parent) -- end  74";

    return true;

}

QHash<int, QByteArray> SinifModel::roleNames() const
{
    qDebug()<<" QHash<int, QByteArray> SinifModel::roleNames() const--begin  line 108";

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    qDebug()<<" QHash<int, QByteArray> SinifModel::roleNames() const--end  line 113";

    return roles;
}

//QVariant SinifModel::callResetAlbumBeforeData(const QModelIndex &index, int role)
//{

//     qDebug()<<"void SinifModel::callResetAlbumBeforeData --begin ";

//        beginResetModel();
//        QVariant returnVal = data(  index,  role);
//        endResetModel();
//        qDebug()<<"void SinifModel::callResetAlbumBeforeData --end ";

//        return returnVal;

//}
