#include "PictureDelegate.h"

#include <QPainter>
#include <QDebug>
const unsigned int BANNER_HEIGHT = 20;
const unsigned int BANNER_COLOR = 0x303030;
const unsigned int BANNER_ALPHA = 200;
const unsigned int BANNER_TEXT_COLOR = 0xffffff;
const unsigned int HIGHLIGHT_ALPHA = 100;

PictureDelegate::PictureDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{
    qDebug()<<"PictureDelegate::PictureDelegate(QObject* parent) : Constructor --begin line 14  ";
    qDebug()<<"PictureDelegate::PictureDelegate(QObject* parent) : Constructor --end line 15  ";
}

void PictureDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    qDebug()<<"void PictureDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const --begin  line 20  ";

    painter->save();

    QPixmap pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
    painter->drawPixmap(option.rect.x(), option.rect.y(), pixmap);

    QRect bannerRect = QRect(option.rect.x(), option.rect.y(), pixmap.width(), BANNER_HEIGHT);
    QColor bannerColor = QColor(BANNER_COLOR);
    bannerColor.setAlpha(BANNER_ALPHA);
    painter->fillRect(bannerRect, bannerColor);

    QString filename = index.model()->data(index, Qt::DisplayRole).toString();
    painter->setPen(BANNER_TEXT_COLOR);
    painter->drawText(bannerRect, Qt::AlignCenter, filename);

    if (option.state.testFlag(QStyle::State_Selected)) {
        QColor selectedColor = option.palette.highlight().color();
        selectedColor.setAlpha(HIGHLIGHT_ALPHA);
        painter->fillRect(option.rect, selectedColor);
    }

    painter->restore();
    qDebug()<<"void PictureDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const --end  line 43  ";

}

QSize PictureDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const
{
    qDebug()<<"QSize PictureDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const --begin line 49  ";

    const QPixmap& pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
    return pixmap.size();
    qDebug()<<"QSize PictureDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const --end line 53  ";
}
