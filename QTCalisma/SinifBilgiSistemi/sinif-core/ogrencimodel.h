#ifndef OGRENCIMODEL_H
#define OGRENCIMODEL_H
#include "sinif-core_global.h"
#include <QAbstractListModel>
#include "ogrenci.h"
#include <vector>
#include <memory>

class SINIFCORESHARED_EXPORT OgrenciModel : public QAbstractListModel
{
    Q_OBJECT
public:


    enum Roles {
        UrlRole = Qt::UserRole + 1,
        FilePathRole
    };

    OgrenciModel(QObject* parent = 0);

    QModelIndex addOgrenci(const Ogrenci& ogrenci);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
//    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    QHash<int, QByteArray> roleNames() const override;

    void setSinifNum(int sinifNum);
    void reloadOgrenciler(OgrenciModel *ogrenciModel);
    void realoadingTOgrenciler();
private:
    //QVector<Ogrenci*> ogrenciler;
    std::unique_ptr<std::vector<std::unique_ptr<Ogrenci>>> ogrenciler;
    std::unique_ptr<std::vector<std::unique_ptr<Ogrenci>>> butunOgrenciler;

     void loadOgrenciler(int sinifNum);
     int sinifNumb;

};

#endif // OGRENCIMODEL_H
