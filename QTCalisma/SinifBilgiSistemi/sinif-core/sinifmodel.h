#ifndef SINIFCORE_H
#define SINIFCORE_H

#include "sinif-core_global.h"
#include "sinif.h"
#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>

class SINIFCORESHARED_EXPORT SinifModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
    };
    SinifModel(QObject* parent = 0);


    // QAbstractItemModel interface
public:
    QModelIndex addSinif(const sinif& s);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override; 
    QHash<int, QByteArray> roleNames() const override;
   // QVariant callResetAlbumBeforeData(const QModelIndex &index, int role);

private :
    std::unique_ptr<std::vector<std::unique_ptr<sinif>>> siniflar;

};

#endif // sinifModel
