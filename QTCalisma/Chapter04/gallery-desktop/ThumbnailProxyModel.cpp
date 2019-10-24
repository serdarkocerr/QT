#include "ThumbnailProxyModel.h"

#include "PictureModel.h"
#include <QDebug>
const unsigned int THUMBNAIL_SIZE = 350;

ThumbnailProxyModel::ThumbnailProxyModel(QObject* parent) :
    QIdentityProxyModel(parent),
    mThumbnails()
{
    qDebug()<<"ThumbnailProxyModel::ThumbnailProxyModel(QObject* parent) - Constructor --begin  line 11  ";
    qDebug()<<"ThumbnailProxyModel::ThumbnailProxyModel(QObject* parent) - Constructor --end  line 12  ";
}

QVariant ThumbnailProxyModel::data(const QModelIndex& index, int role) const
{
    qDebug()<<"QVariant ThumbnailProxyModel::data(const QModelIndex& index, int role) const --begin  line 17  ";

    if (role != Qt::DecorationRole) {
        qDebug()<<"QVariant ThumbnailProxyModel::data(const QModelIndex& index, int role) const --end  line 20  ";
        return QIdentityProxyModel::data(index, role);
    }

    QString filepath = sourceModel()->data(index, PictureModel::Roles::FilePathRole).toString();
    qDebug()<<"QVariant ThumbnailProxyModel::data(const QModelIndex& index, int role) const --end  line 25  ";
    return *mThumbnails[filepath];
}

void ThumbnailProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    qDebug()<<"void ThumbnailProxyModel::setSourceModel(QAbstractItemModel* sourceModel)--begin  line 31  ";

    QIdentityProxyModel::setSourceModel(sourceModel);
    if (!sourceModel) {
        return;
    }

    connect(sourceModel, &QAbstractItemModel::modelReset, [this] {
        reloadThumbnails();
    });

    connect(sourceModel, &QAbstractItemModel::rowsInserted, [this](const QModelIndex& parent, int first, int last) {
        generateThumbnails(index(first, 0), last - first + 1);
    });

    qDebug()<<"void ThumbnailProxyModel::setSourceModel(QAbstractItemModel* sourceModel)--end  line 45  ";

}

PictureModel* ThumbnailProxyModel::pictureModel() const
{
    qDebug()<<"PictureModel* ThumbnailProxyModel::pictureModel() const --begin  line 51  ";
    qDebug()<<"PictureModel* ThumbnailProxyModel::pictureModel() const --end  line 52  ";

    return static_cast<PictureModel*>(sourceModel());
}

void ThumbnailProxyModel::reloadThumbnails()
{
    qDebug()<<"void ThumbnailProxyModel::reloadThumbnails() --begin line 59  ";

    qDeleteAll(mThumbnails);
    mThumbnails.clear();
    generateThumbnails(index(0, 0), rowCount());
    qDebug()<<"void ThumbnailProxyModel::reloadThumbnails() --end line 64  ";

}

void ThumbnailProxyModel::generateThumbnails(const QModelIndex& startIndex, int count)
{
    qDebug()<<"void ThumbnailProxyModel::generateThumbnails(const QModelIndex& startIndex, int count) --begin line 70  ";

    if (!startIndex.isValid()) {
        return;
    }

    const QAbstractItemModel* model = startIndex.model();
    int lastIndex = startIndex.row() + count;
    for(int row = startIndex.row(); row < lastIndex; row++) {
        QString filepath = model->data(model->index(row, 0), PictureModel::Roles::FilePathRole).toString();
        QPixmap pixmap(filepath);
        auto thumbnail = new QPixmap(pixmap
                                     .scaled(THUMBNAIL_SIZE, THUMBNAIL_SIZE,
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
        mThumbnails.insert(filepath, thumbnail);
    }
    qDebug()<<"void ThumbnailProxyModel::generateThumbnails(const QModelIndex& startIndex, int count) --end line 87  ";

}
